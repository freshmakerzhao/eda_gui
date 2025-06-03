/**
  ******************************************************************************
  * @file           : InfoWidget.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/18
  ******************************************************************************
  */
#include "InfoWidget.h"
#include "LogWidget.h"
#include "utils/json.hpp"
#include "MessageWidget.h"
#include "component/timing/TimingWidget.h"
#include "component/power/PowerWidget.h"
#include "component/ioports/IOportsTreeWidget.h"
#include "utils/ProjectManager.h"
#include "entity/Project.h"

InfoWidget *InfoWidget::instance(QWidget *parent)
{
    static InfoWidget *_instance = nullptr;
    if (!_instance) {
        _instance = new InfoWidget(parent);
    }
    return _instance;
}

void InfoWidget::setCurrentPage(int index) {
    tabWidget->setCurrentIndex(index);
}

void InfoWidget::updateSynthItem(const QString synthPath,
                                 const QString status,
                                 const QString startTime,
                                 const QString Elapsed,
                                 const QString partName){
    initSummary("synth"); // 初始化数据
    // 解析资源使用报告
    QFile file(synthPath + "/synth_stat.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();

    nlohmann::json j = nlohmann::json::parse(jsonData.toStdString());
    for (auto& [key, value] : j.items()) {
        auto it = keyMapSynth.find(key);
        if (it != keyMapSynth.end()) {
            *(it->second) += value.get<int>();
        }
    }

    lut6NumSynth = lutNumSynth - muxf6NumSynth; // LUT6
    bramNumSynth = fifo18NumSynth + (ranb18NumSynth+1)/2 + ranb36NumSynth; // BRAM

    runsModel->item(0, 1)->setText(status); // Status
    runsModel->item(0, 2)->setText(QString::number(lut6NumSynth)); // LUT6
    runsModel->item(0, 3)->setText(QString::number(ffNumSynth)); // ff
    runsModel->item(0, 4)->setText(QString::number(bramNumSynth)); // BRAM
    runsModel->item(0, 5)->setText(QString::number(dspNumSynth)); // dsp
    runsModel->item(0, 6)->setText(QString::number(carry4NumSynth)); // carry4
    runsModel->item(0, 7)->setText(startTime); // 开始时间
    runsModel->item(0, 8)->setText(Elapsed); // 持续时间
    runsModel->item(0, 9)->setText(partName); // 封装名称

    nlohmann::json outputJson;
    outputJson["status"] = status.toStdString();
    outputJson["LUT6"] = lut6NumSynth;
    outputJson["ff"] = ffNumSynth;
    outputJson["BRAM"] = bramNumSynth;
    outputJson["dsp"] = dspNumSynth;
    outputJson["carry4"] = carry4NumSynth;
    outputJson["startTime"] = startTime.toStdString();
    outputJson["Elapsed"] = Elapsed.toStdString();
    outputJson["partName"] = partName.toStdString();

    // Convert JSON object to QString
    QString jsonString = QString::fromStdString(outputJson.dump(4));

    // Write to gen_run.json file
    QFile outFile(synthPath + "/gen_run.json");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream outStream(&outFile);
    outStream << jsonString;
    outFile.close();
    // Display_Synth_Usage
}

void InfoWidget::updateImplItem(const QString& implPath,
                                const QString& status,
                                const QString& startTime,
                                const QString& Elapsed,
                                const QString& partName){
    initSummary("impl"); // 初始化数据
//    QFile file(implPath + "/pb_type_count.json");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        return;// 文件打开失败
//    }
//    QString jsonData = file.readAll();
//    file.close();
//    nlohmann::json j = nlohmann::json::parse(jsonData.toStdString());

//    for (auto& [key, value] : j.items()) {
//        auto itImpl = keyMapImpl.find(key);
//        if (itImpl != keyMapImpl.end()) {
//            *(itImpl->second) += value.get<int>();
//        }
//    }

    runsModel->item(1, 1)->setText(status); // Status
    runsModel->item(1, 2)->setText(QString::number(lut6NumSynth)); // LUT6
    runsModel->item(1, 3)->setText(QString::number(ffNumSynth)); // ff
    runsModel->item(1, 4)->setText(QString::number(bramNumSynth)); // BRAM
//    runsModel->item(1, 4)->setText(QString::number(bramNumImpl)); // BRAM
//    runsModel->item(1, 5)->setText(QString::number(dspNumImpl)); // dsp
    runsModel->item(1, 5)->setText(QString::number(dspNumSynth)); // dsp
    runsModel->item(1, 6)->setText(QString::number(carry4NumSynth)); // carry
    runsModel->item(1, 7)->setText(startTime); // 开始时间
    runsModel->item(1, 8)->setText(Elapsed); // 持续时间
    runsModel->item(1, 9)->setText(partName); // 封装名称

    nlohmann::json outputJson;
    outputJson["status"] = status.toStdString();
    outputJson["LUT6"] = lut6NumSynth;
    outputJson["ff"] = ffNumSynth;
    outputJson["BRAM"] = bramNumSynth;
//    outputJson["BRAM"] = bramNumImpl;
//    outputJson["dsp"] = dspNumImpl;
    outputJson["dsp"] = dspNumSynth;
    outputJson["carry4"] = carry4NumSynth;
    outputJson["startTime"] = startTime.toStdString();
    outputJson["Elapsed"] = Elapsed.toStdString();
    outputJson["partName"] = partName.toStdString();

    // Convert JSON object to QString
    QString jsonString = QString::fromStdString(outputJson.dump(4));

    // Write to gen_run.json file
    QFile outFile(implPath + "/gen_run.json");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream outStream(&outFile);
    outStream << jsonString;
    outFile.close();
}

void InfoWidget::resetInfoWidget()
{
    initDesignRunsView();
    MessageWidget::instance()->clearMessage();
    LogWidget::instance()->clearLog();
}

void InfoWidget::initDesignRunsView(const QString &prjPath)
{
    const QString synthPath = prjPath + "/runs/synth";
    const QString implPath = prjPath + "/runs/impl";
    // 配置表格数据
    // ---------- synth ---------
    runsModel->setItem(0, 0, new QStandardItem(QString("synth")));
    runsModel->setItem(0, 1, new QStandardItem(QString("Not started"))); // Status
    runsModel->setItem(0, 2, new QStandardItem()); // LUT6
    runsModel->setItem(0, 3, new QStandardItem()); // ff
    runsModel->setItem(0, 4, new QStandardItem()); // BRAM
    runsModel->setItem(0, 5, new QStandardItem()); // dsp
    runsModel->setItem(0, 6, new QStandardItem()); // carry4
    runsModel->setItem(0, 7, new QStandardItem()); // 开始时间
    runsModel->setItem(0, 8, new QStandardItem()); // 持续时间
    runsModel->setItem(0, 9, new QStandardItem()); // 封装名称
    // ---------- impl ----------
    runsModel->setItem(1, 0, new QStandardItem(QString("impl")));
    runsModel->setItem(1, 1, new QStandardItem(QString("Not started"))); // Status
    runsModel->setItem(1, 2, new QStandardItem()); // LUT6
    runsModel->setItem(1, 3, new QStandardItem()); // ff
    runsModel->setItem(1, 4, new QStandardItem()); // BRAM
    runsModel->setItem(1, 5, new QStandardItem()); // dsp
    runsModel->setItem(1, 6, new QStandardItem()); // carry
    runsModel->setItem(1, 7, new QStandardItem()); // 开始时间
    runsModel->setItem(1, 8, new QStandardItem()); // 持续时间
    runsModel->setItem(1, 9, new QStandardItem()); // 封装名称

    QFile synthInFile(synthPath + "/gen_run.json");
    if (synthInFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonData = synthInFile.readAll();
        synthInFile.close();
        nlohmann::json j = nlohmann::json::parse(jsonData.toStdString());
        lut6NumSynth =j.value("LUT6", 0);
        ffNumSynth = j.value("ff", 0);
        bramNumSynth =j.value("BRAM", 0);
        dspNumSynth = j.value("dsp", 0);
        carry4NumSynth = j.value("carry4", 0);
        runsModel->item(0, 1)->setText(QString::fromStdString(j.value("status", ""))); // Status
        runsModel->item(0, 2)->setText(QString::number(j.value("LUT6", 0))); // LUT6
        runsModel->item(0, 3)->setText(QString::number(j.value("ff", 0))); // ff
        runsModel->item(0, 4)->setText(QString::number(j.value("BRAM", 0))); // BRAM
        runsModel->item(0, 5)->setText(QString::number(j.value("dsp", 0))); // dsp
        runsModel->item(0, 6)->setText(QString::number(j.value("carry4", 0))); // carry4
        runsModel->item(0, 7)->setText(QString::fromStdString(j.value("startTime", ""))); // 开始时间
        runsModel->item(0, 8)->setText(QString::fromStdString(j.value("Elapsed", ""))); // 持续时间
        runsModel->item(0, 9)->setText(QString::fromStdString(j.value("partName", ""))); // 封装名称
    }

    QFile implInFile(implPath + "/gen_run.json");
    if (implInFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonData = implInFile.readAll();
        implInFile.close();
        nlohmann::json j = nlohmann::json::parse(jsonData.toStdString());
        runsModel->item(1, 1)->setText(QString::fromStdString(j.value("status", ""))); // Status
        runsModel->item(1, 2)->setText(QString::number(j.value("LUT6", 0))); // LUT6
        runsModel->item(1, 3)->setText(QString::number(j.value("ff", 0))); // ff
        runsModel->item(1, 4)->setText(QString::number(j.value("BRAM", 0))); // BRAM
        runsModel->item(1, 5)->setText(QString::number(j.value("dsp", 0))); // dsp
        runsModel->item(1, 6)->setText(QString::number(j.value("carry4", 0))); // carry4
        runsModel->item(1, 7)->setText(QString::fromStdString(j.value("startTime", ""))); // 开始时间
        runsModel->item(1, 8)->setText(QString::fromStdString(j.value("Elapsed", ""))); // 持续时间
        runsModel->item(1, 9)->setText(QString::fromStdString(j.value("partName", ""))); // 封装名称
    }
}

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[InfoWidget] Constructing...";
    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    QTabBar *tabBar = tabWidget->tabBar();
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(tabWidget);

    // ---------------------- Tcl Console ---------------------
    tabWidget->addTab(TclConsole::instance(), "Tcl Console");
    // tabWidget->setTabEnabled(0, false);
    // ----------------------- Messages -----------------------
    tabWidget->addTab(MessageWidget::instance(), "Messages");
    // ------------------------- Log --------------------------
    tabWidget->addTab(LogWidget::instance(),"Log");
    // ----------------------- Reports ------------------------
    rpt = new QPlainTextEdit, rpt->setReadOnly(true);
    tabWidget->addTab(rpt, "Reports");
    tabWidget->setTabEnabled(3, false);
    // --------------------- Design Runs ----------------------
    runsView = new QTreeView;
    tabWidget->addTab(runsView, "Design Runs");
    tabWidget->setCurrentIndex(4);
    runsModel = new QStandardItemModel(runsView);
    QStringList headers = {"Name",
                           "Status",
                           "LUT6",
                           "FF",
                           "BRAM",
                           "DSP",
                           "CARRY4",
                           "Start",
                           "Elapsed",
                           "Part"};
    runsModel->setHorizontalHeaderLabels(headers);
    // runsView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    initDesignRunsView();
    runsView->setModel(runsModel);
    runsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    runsView->expandAll();
    runsView->setColumnWidth(1, 240);
    runsView->setColumnWidth(7, 200);
    runsView->setColumnWidth(9, 200);

    // -------------------------------------------------------
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &InfoWidget::onTabWidgetTabCloseRequested);
    tabBar->setTabButton(0, QTabBar::RightSide, nullptr);
    tabBar->setTabButton(1, QTabBar::RightSide, nullptr);
    tabBar->setTabButton(2, QTabBar::RightSide, nullptr);
    tabBar->setTabButton(3, QTabBar::RightSide, nullptr);
    tabBar->setTabButton(4, QTabBar::RightSide, nullptr);

}

InfoWidget::~InfoWidget()
{
    qDebug() << "[InfoWidget] Distructing...";
}

void InfoWidget::initSummary(const QString phase) {
    if (phase == "synth"){
        // 初始化综合阶段资源统计数据
        lut6NumSynth = 0 , lutNumSynth = 0, muxf6NumSynth = 0 , ffNumSynth = 0 , bramNumSynth = 0 , fifo18NumSynth = 0 , ranb18NumSynth = 0 , ranb36NumSynth = 0 , dspNumSynth = 0 ,carry4NumSynth = 0;
    } else if (phase == "impl"){
        // 初始化布局布线阶段资源统计数据
        lut6NumImpl = 0 , lutNumImpl = 0, muxf6NumImpl = 0 , ffNumImpl = 0 , bramNumImpl = 0 , fifo18NumImpl = 0 , ranb18NumImpl = 0 , ranb36NumImpl = 0 , dspNumImpl = 0 ,carry4NumImpl = 0;
    }
}

void InfoWidget::generateTimingSummary()
{
    // 提取参数
    QString projectPath = ProjectManager::instance().getParameter(Project::Path);
    QString topModuleName = ProjectManager::instance().getParameter(Project::TopModule);

    // 构建 timing.json 文件路径
    QString timingResultPath = QDir(projectPath).filePath("runs/impl/" + topModuleName + "_timing.json");
    qDebug() << "[InfoWidget generateTimingSummary] timingResultPath : " << timingResultPath;
    // 创建 TimingWidget 并加载数据
    TimingWidget* timingWidget = new TimingWidget;
    timingWidget->loadDataFromJson(timingResultPath);
    onTabWidgetTabCloseRequested(5);
    tabWidget->insertTab(5, timingWidget, "Timing");
    tabWidget->setCurrentIndex(5);

}

void InfoWidget::generateReportPower()
{
    // 提取参数
    QString projectPath = ProjectManager::instance().getParameter(Project::Path);
    QString topModuleName = ProjectManager::instance().getParameter(Project::TopModule);

    // 构建 power.json 文件路径
    QString powerResultPath = QDir(projectPath).filePath("runs/impl/" + topModuleName + "_power.json");
    qDebug() << "[InfoWidget generateReportPower] powerResultPath : " << powerResultPath;
    PowerWidget* powerWidget = new PowerWidget(powerResultPath);
    onTabWidgetTabCloseRequested(5);
    tabWidget->insertTab(5, powerWidget, "Power");
    tabWidget->setCurrentIndex(5);
}

void InfoWidget::generateIOports()
{
    IOportsTreeWidget* ioportsTreeWidget = new IOportsTreeWidget;
    onTabWidgetTabCloseRequested(5);
    tabWidget->insertTab(5, ioportsTreeWidget, "I/O Ports");
    tabWidget->setCurrentIndex(5);
}

void InfoWidget::onTabWidgetTabCloseRequested(int index)
{
    auto *widget = tabWidget->widget(index);
    if (!widget)
        return;

    tabWidget->removeTab(index);
    delete widget;
}


