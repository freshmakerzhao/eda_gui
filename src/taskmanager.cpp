#include <QMessageBox>
#include "taskmanager.h"
#include "processmanager.h"
#include "utils/StringUtilities.h"
#include "utils/CommandBuilder.h"
#include "infowidget.h"
#include "widgets/FrameView.h"
#include "mainwindow.h"

TaskManager *TaskManager::instance(QWidget *parent)
{
    static TaskManager *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new TaskManager(parent);
    }
    return m_instance;
}

bool TaskManager::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == taskTree->viewport()) {
        //点击树的空白,取消选中
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                QModelIndex index = taskTree->indexAt(mouseEvent->pos());
                if (!index.isValid()) {
                    taskTree->setCurrentIndex(QModelIndex());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

TaskManager::TaskManager(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[TaskManager] Constructing...";
    taskTree = new QTreeWidget(this);
    taskTree->viewport()->installEventFilter(this); //事件过滤
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(taskTree);
    layout->setMargin(0);
    taskTree->setColumnCount(1);
    taskTree->setHeaderHidden(true);
    // ================== 综合 ==================
    QTreeWidgetItem *synthItem = new QTreeWidgetItem(taskTree, QStringList() << "Synthesis");
    synthItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    QTreeWidgetItem *synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run");
    synthRunItem->setIcon(0, QIcon(""));
    // run synth
    QTreeWidgetItem *synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(""));
    // ================== imp ==================
    QTreeWidgetItem *impItem = new QTreeWidgetItem(taskTree, QStringList() << "Implementation");
    impItem->setIcon(0, QIcon(""));
    // pack_place_route
    QTreeWidgetItem *impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(""));
    // 仅pack
    QTreeWidgetItem *impPackItem = new QTreeWidgetItem(impItem, QStringList() << "Only pack");
    impPackItem->setIcon(0, QIcon(""));
    // 仅place
    QTreeWidgetItem *impPlaceItem = new QTreeWidgetItem(impItem, QStringList() << "Only place");
    impPlaceItem->setIcon(0, QIcon(""));
    // 仅route
    QTreeWidgetItem *impRouteItem = new QTreeWidgetItem(impItem, QStringList() << "Only route");
    impRouteItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *impPackReportItem = new QTreeWidgetItem(impItem, QStringList() << "Report");
    // 查看 Pack 日志
    impPackReportItem->setIcon(0, QIcon(""));
    // ================== 码流及可视化 ==================
    QTreeWidgetItem *proItem = new QTreeWidgetItem(taskTree, QStringList() << "Program and Debug");
    proItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proNetlistViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate NetlistView");
    proNetlistViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitItem = new QTreeWidgetItem(proItem, QStringList() << "Generate Bitstream");
    proBitItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate GridView");
    proBitViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proDownloadBitItem = new QTreeWidgetItem(proItem, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(""));

    QObject::connect(taskTree, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column) {
        if (this->arch == ""){
            // 用户未选择架构时
            QMessageBox::critical(MainWindow::instance(), "Failed", "Please select or create a project.");
            return ;
        }
        // 双击触发
        if (item == synthRunItem) {
            runSynth();
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
//            InfoWidget::instance()->updateSynthItem(projectSynthPath);
            // 激活 Design Runs 窗口
//            InfoWidget::instance()->setCurrentPage(4);
        } else if (item == synthReportItem) {
            // synthReport();
        } else if (item == impAllItem) {
            // pack place route全流程
            buildImp();
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
            InfoWidget::instance()->updateImplItem(projectImplPath);
        } else if (item == impPackItem) {
            buildPack();
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
        } else if (item == impPackReportItem) {
            // impReport();
        } else if (item == impPlaceItem) {
            buildPlace(3);
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
        } else if (item == impRouteItem) {
            buildRoute();
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
        } else if (item == proBitItem) {
            buildBit(2);
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
        } else if (item == proBitViewItem) {
            qDebug() << "[TaskManager] arch " << this->arch;
            std::string tileGridPath = GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tilegrid_" + this->arch.toStdString() + ".json";
            std::string tileColorPath = GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tile_info_map.json";
            qDebug() << "[TaskManager] tileGridPath " << QString::fromStdString(tileGridPath);
            qDebug() << "[TaskManager] tileColorPath " << QString::fromStdString(tileColorPath);
            if (gridView) {
                delete gridView;  // 删除现存的对象
                gridView = nullptr;  // 确保指针不再指向已删除的对象
            }
            gridView = new FrameView(tileGridPath,tileColorPath);
            gridView->resize(1000, 800);
            gridView->show();
        } else if (item == proNetlistViewItem) {
            // if (!frameView) {
            //     frameView = new NetlistView();
            // }
            // frameView->resize(1000, 800);
            // frameView->show();
        } else if (item == proDownloadBitItem) {
            downloadBit();
            // 激活 log 窗口
            InfoWidget::instance()->setCurrentPage(2);
        }
    });

    taskTree->expandAll();

}

TaskManager::~TaskManager()
{
    qDebug() << "[TaskManager] Distructing...";
}

// 执行综合阶段
void TaskManager::runSynth() {

    // TODO 执行前判断source与constraint文件是否存在
    // TODO source不存在不能执行
    // TODO constraint文件不存在弹出提示框
    // TODO runs及synth路径是否存在
    QString family = "xc7";
    QString topName = "top";
    // 初始化环境
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);

    //start synthesis
    QString tclPath = QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH.toStdString(), "f4pga", "scripts", "xc7.f4pga_xilinx.tcl"}));
    QString yosysCmd = QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH.toStdString(), "yosys", "bin", "yosys.exe"}));
    QString script1 = yosysCmd + " -p " + R"(")" + "tcl " + tclPath + R"(")" + " -l top_synth.log ";
    QString script2 = yosysCmd + " -p \"tcl " + tclPath + "\"" + " -l top_synth.log ";
    QStringList arguments;
    arguments << "/c" << yosysCmd
              << "-p" << "tcl "+ tclPath
              << "-l" << "top_synth.log";

    for(const QString& sourcePath :sourcePathList){
        arguments << sourcePath;
    }
    ProcessManager::instance().checkCallSpecific("Synthesis", projectSynthPath, arguments,partName);
}

void TaskManager::buildPack() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    std::string script = CommandBuilder::instance().generateImpPackCommands(projectSynthPath,projectImplPath,archName);
    ProcessManager::instance().checkCall("Pack", projectImplPath, QString::fromStdString(script));
}


// place 阶段
// mode 1 ： 仅生成top.ioplace
// mode 2 ： 生成top.ioplace 与 constrains.place
// mode 3 ： 生成top.ioplace 与 constrains.place 并完成 vpr_place
void TaskManager::buildPlace(int mode) {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    std::string script;
    // 生成top.ioplace
    if (mode == 1) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        ProcessManager::instance().checkCall("IOPlace Generation", projectPath, QString::fromStdString(script));
    }

    // 生成top.ioplace 与 constrains.place
    if (mode == 2) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        ProcessManager::instance().checkCall("Constraints Generation", projectImplPath, QString::fromStdString(script));
    }

    // 完成 vpr_place
    if (mode == 3) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script += " && " + CommandBuilder::instance().generateImpPlaceCommands(projectSynthPath,projectImplPath,archName);
        ProcessManager::instance().checkCall("Place", projectImplPath, QString::fromStdString(script));
    }
}


// Route 阶段
void TaskManager::buildRoute() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);

    std::string script = CommandBuilder::instance().generateImpRouteCommands(projectSynthPath,archName);
    ProcessManager::instance().checkCall("Route", projectImplPath, QString::fromStdString(script));
}

// pack place route
void TaskManager::buildImp() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    // pack
    std::string script = CommandBuilder::instance().generateImpPackCommands(projectSynthPath,projectImplPath,archName);
    // place
    script += " && " + CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
    script += " && " + CommandBuilder::instance().generateImpConstrainsCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
    script += " && " + CommandBuilder::instance().generateImpPlaceCommands(projectSynthPath,projectImplPath,archName);
    // route
    script += " && " + CommandBuilder::instance().generateImpRouteCommands(projectSynthPath,archName);
    ProcessManager::instance().checkCall("Implementation", projectImplPath, QString::fromStdString(script));
}

// 生成bit流 阶段
// mode 1 ： 仅生成top.fasm
// mode 2 ： 生成top.fasm 与 top.bit
void TaskManager::buildBit(int mode) {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    if (mode == 1) {
        std::string script = CommandBuilder::instance().generateFasmCommands(projectSynthPath,archName);
        ProcessManager::instance().checkCall("Fasm Generation", projectImplPath, QString::fromStdString(script));
    }
    if (mode == 2) {
        std::string script = CommandBuilder::instance().generateFasmCommands(projectSynthPath,archName);
        script += " && " + CommandBuilder::instance().generateBitCommands(projectImplPath,"%PYTHON3%");
        ProcessManager::instance().checkCall("Bitstream Generation", projectImplPath, QString::fromStdString(script));
    }
}

void TaskManager::downloadBit() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    std::string script = CommandBuilder::instance().generateDownloadBitCommands(projectImplPath,"digilent_hs3","top.bit");
    ProcessManager::instance().checkCall("Bitstream Download", projectImplPath, QString::fromStdString(script));
}
