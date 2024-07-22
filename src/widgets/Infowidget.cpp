#include "InfoWidget.h"
#include "LogWidget.h"

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

void InfoWidget::updateSynthItem(const QString synthPath, const QString status, const QString startTime, const QString Elapsed , const QString partName){
    initSummary("synth"); // 初始化数据
    // 解析资源使用报告
    QFile file(synthPath + "/synth_stat.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject object = doc.object();
    // 配置表格数据
    // runsModel->setItem(0, 0, new QStandardItem(QString("synth")));
    for (auto it = object.begin(); it != object.end(); ++it) {
        if (it.key() == "$lut") {
            lutNumSynth += it.value().toInt();
        } else if (it.key() == "MUXF6") {
            muxf6NumSynth += it.value().toInt();
        } else if (it.key() == "FDRE_ZINI" || it.key() == "FDSE_ZINI" || it.key() == "FDCE_ZINI" || it.key() == "FDPE_ZINI") {
            ffNumSynth += it.value().toInt();
        } else if (it.key() == "FDRE_ZINI_1" || it.key() == "FDSE_ZINI_1" || it.key() == "FDCE_ZINI_1" || it.key() == "FDPE_ZINI_1") {
            ffNumSynth += it.value().toInt();
        } else if (it.key() == "FIFO18E1_VPR") {
            fifo18NumSynth += it.value().toInt();
        } else if (it.key() == "RAMB18E1_VPR") {
            ranb18NumSynth += it.value().toInt();
        } else if (it.key() == "RAMB36E1_PRIM") {
            ranb36NumSynth += it.value().toInt();
        } else if (it.key() == "DSP48E1_VPR") {
            dspNumSynth += it.value().toInt();
        } else if (it.key() == "CARRY4_VPR") {
            carry4NumSynth += it.value().toInt();
        }
    }
    lut6NumSynth = lutNumSynth - muxf6NumSynth; // LUT6
    bramNumSynth = fifo18NumSynth + (ranb18NumSynth+1)/2 + ranb36NumSynth; // BRAM

    runsModel->setItem(0, 1, new QStandardItem(status)); // Status
    runsModel->setItem(0, 2, new QStandardItem(QString::number(lut6NumSynth))); // LUT6
    runsModel->setItem(0, 3, new QStandardItem(QString::number(ffNumSynth))); // ff
    runsModel->setItem(0, 4, new QStandardItem(QString::number(bramNumSynth))); // BRAM
    runsModel->setItem(0, 5, new QStandardItem(QString::number(dspNumSynth))); // dsp
    runsModel->setItem(0, 6, new QStandardItem(QString::number(carry4NumSynth))); // carry4
    runsModel->setItem(0, 7, new QStandardItem(startTime)); // 开始时间
    runsModel->setItem(0, 8, new QStandardItem(Elapsed)); // 持续时间
    runsModel->setItem(0, 9, new QStandardItem(partName)); // 封装名称


    // Display_Synth_Usage
}

void InfoWidget::updateImplItem(const QString& implPath, const QString& status, const QString& startTime, const QString& Elapsed , const QString& partName){
    initSummary("impl"); // 初始化数据
    QFile file(implPath + "/pb_type_count.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject object = doc.object();
    // 配置表格数据
    // runsModel->setItem(1, 0, new QStandardItem(QString("impl")));
    for (auto it = object.begin(); it != object.end(); ++it) {
        if (it.key() == "BLK-TL-DSP48E1") {
            dspNumImpl += it.value().toInt();
        } else if (it.key() == "BLK-TL-BRAM_L") {
            bramNumImpl += it.value().toInt();
        }
    }

    runsModel->setItem(1, 1, new QStandardItem(status)); // Status
    runsModel->setItem(1, 2, new QStandardItem(QString::number(lut6NumSynth))); // LUT6
    runsModel->setItem(1, 3, new QStandardItem(QString::number(ffNumSynth))); // ff
    runsModel->setItem(1, 4, new QStandardItem(QString::number(bramNumImpl))); // BRAM
    runsModel->setItem(1, 5, new QStandardItem(QString::number(dspNumImpl))); // dsp
    runsModel->setItem(1, 6, new QStandardItem(QString::number(carry4NumSynth))); // carry
    runsModel->setItem(1, 7, new QStandardItem(startTime)); // 开始时间
    runsModel->setItem(1, 8, new QStandardItem(Elapsed)); // 持续时间
    runsModel->setItem(1, 9, new QStandardItem(partName)); // 封装名称


}

void InfoWidget::initDesignRunsView()
{
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

}

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[InfoWidget] Constructing...";
    tabWidget = new QTabWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(tabWidget);

    // =========================== Csg =============================
    csl = new QPlainTextEdit(this);
    tabWidget->addTab(csl, "Tcl Console");
    tabWidget->setTabEnabled(0, false);
    // =========================== Msg =============================
    msg = new QPlainTextEdit(this), msg->setReadOnly(true);
    tabWidget->addTab(msg, "Messages");
    tabWidget->setTabEnabled(1, false);
    // =========================== Log =============================
    tabWidget->addTab(LogWidget::instance(),"Log");
    tabWidget->setCurrentIndex(2);
    // ============================ Rpt ============================
    rpt = new QPlainTextEdit(this), rpt->setReadOnly(true);
    tabWidget->addTab(rpt, "Reports");
    tabWidget->setTabEnabled(3, false);
    // ======================== Design Runs ========================
    runsView = new QTreeView(this);
    runsView->setStyleSheet("QTreeView { border: 1px solid #999; }"
                            "QTreeView::item { border-bottom: 1px solid #999; border-right: 1px solid #999;}"
                            "QTreeView::item:selected { background-color: #4f7cce; }");
    tabWidget->addTab(runsView, "Design Runs");
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
    initDesignRunsView();
    runsView->setModel(runsModel);
    runsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    runsView->expandAll();
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


