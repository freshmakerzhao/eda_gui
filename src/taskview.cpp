#include "taskview.h"
#include "processmanager.h"
#include "utils/StringUtilities.h"
#include "utils/CommandBuilder.h"

TaskView *TaskView::instance(QWidget *parent)
{
    static TaskView *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new TaskView(parent);
    }
    return m_instance;
}

bool TaskView::eventFilter(QObject *obj, QEvent *event)
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

TaskView::TaskView(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[TaskView] Constructing...";
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
        // 双击触发
        if (item == synthRunItem) {
             runSynth();
        } else if (item == synthReportItem) {
            // synthReport();
        } else if (item == impAllItem) {
            // pack place route全流程
            // buildImp();
        } else if (item == impPackItem) {
             buildPack();
        } else if (item == impPackReportItem) {
            // impReport();
        } else if (item == impPlaceItem) {
             buildPlace(3);
        } else if (item == impRouteItem) {
             buildRoute();
        } else if (item == proBitItem) {
             buildBit(2);
        } else if (item == proBitViewItem) {
            // if (frameView) {
            //     delete frameView;  // 删除现存的对象
            //     frameView = nullptr;  // 确保指针不再指向已删除的对象
            // }
            // frameView = new FrameView();
            // frameView->resize(1000, 800);
            // frameView->show();
        } else if (item == proNetlistViewItem) {
            // if (!frameView) {
            //     frameView = new NetlistView();
            // }
            // frameView->resize(1000, 800);
            // frameView->show();
        } else if (item == proDownloadBitItem) {
             downloadBit();
        }
    });

    taskTree->expandAll();

}

TaskView::~TaskView()
{
    qDebug() << "[TaskView] Distructing...";
}

// 执行综合阶段
void TaskView::runSynth() {

    // TODO 执行前判断source与constraint文件是否存在
    // TODO source不存在不能执行
    // TODO constraint文件不存在弹出提示框

    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    // 初始化环境
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);

    //start synthesis
    QString tclPath = QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH, "f4pga", "scripts", "xc7.f4pga_xilinx.tcl"}));
    QString yosysCmd = QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH, "yosys", "bin", "yosys.exe"}));
    QString script1 = yosysCmd + " -p " + R"(")" + "tcl " + tclPath + R"(")" + " -l top_synth.log ";
    QString script2 = yosysCmd + " -p \"tcl " + tclPath + "\"" + " -l top_synth.log ";
    QStringList arguments;
    arguments << "/c" << yosysCmd
              << "-p" << "tcl "+ tclPath
              << "-l" << "top_synth.log";

    for(const QString& sourcePath :sourcePathList){
        arguments << sourcePath;
    }
    ProcessManager::instance().checkCallSpecific("synth", QString::fromStdString(projectPath), arguments);
    // TODO 需要监听process的输出，打印到控制台
//    showLog("synth success", "synth failed", false);
}

void TaskView::buildPack() {
    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);
    std::string script = CommandBuilder::instance().generateImpPackCommands(GLOBAL_PART_NAME,projectPath);
    qDebug() << QString::fromStdString(script);
    const QString msgSuccess = "pack 启动成功";
    const QString msgFail = "pack 启动失败";
    const QString path = QString::fromStdString(projectPath);
    const QString phase = "pack";
//    configOutputSignals("pack");
    ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
    // TODO 需要监听process的输出，打印到控制台
//    showLog( msgSuccess, msgFail, false);
}


// place 阶段
// mode 1 ： 仅生成top.ioplace
// mode 2 ： 生成top.ioplace 与 constrains.place
// mode 3 ： 生成top.ioplace 与 constrains.place 并完成 vpr_place
void TaskView::buildPlace(int mode) {
    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);
    std::string script;
    // 生成top.ioplace
    if (mode == 1) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands("%PYTHON3%",projectPath);
        const QString phase = "generateIOPlace";
        const QString path = QString::fromStdString(projectPath);
        const QString msgFail = "generateIOPlace 启动失败";
        const QString msgSuccess = "generateIOPlace 启动成功";
        ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
        // TODO 需要监听process的输出，打印到控制台
    }

    // 生成top.ioplace 与 constrains.place
    if (mode == 2) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands("%PYTHON3%",projectPath);

        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands("%PYTHON3%",projectPath);

        const QString phase = "generateConstraints";
        const QString path = QString::fromStdString(projectPath);
        const QString msgFail = "generateConstraints 启动失败";
        const QString msgSuccess = "generateConstraints 启动成功";
        ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
        // TODO 需要监听process的输出，打印到控制台
    }

    // 完成 vpr_place
    if (mode == 3) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands("%PYTHON3%",projectPath);
        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands("%PYTHON3%",projectPath);
        script += " && " + CommandBuilder::instance().generateImpPlaceCommands(GLOBAL_PART_NAME,projectPath);
        const QString phase = "place";
        const QString path = QString::fromStdString(projectPath);
        const QString msgFail = "place 启动失败";
        const QString msgSuccess = "place 启动成功";
        ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
        // TODO 需要监听process的输出，打印到控制台
    }
}


// Route 阶段
void TaskView::buildRoute() {
    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);

    std::string script = CommandBuilder::instance().generateImpRouteCommands(GLOBAL_PART_NAME,projectPath);

    const QString phase = "Route";
    const QString path = QString::fromStdString(projectPath);
    const QString msgFail = "Route 启动失败";
    const QString msgSuccess = "Route 启动成功";
    ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
    // TODO 需要监听process的输出，打印到控制台
}

// 生成bit流 阶段
// mode 1 ： 仅生成top.fasm
// mode 2 ： 生成top.fasm 与 top.bit
void TaskView::buildBit(int mode) {
    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);
    if (mode == 1) {
        std::string script = CommandBuilder::instance().generateFasmCommands(GLOBAL_PART_NAME,projectPath);
        const QString phase = "Generate_fasm";
        const QString path = QString::fromStdString(projectPath);
        const QString msgFail = "Generate_fasm 启动失败";
        const QString msgSuccess = "Generate_fasm 启动成功";
        ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
        // TODO 需要监听process的输出，打印到控制台
    }
    if (mode == 2) {
        std::string script = CommandBuilder::instance().generateFasmCommands(GLOBAL_PART_NAME,projectPath);
        script += " && " + CommandBuilder::instance().generateBitCommands("%PYTHON3%",projectPath);
        const QString phase = "Generate_bit";
        const QString path = QString::fromStdString(projectPath);
        const QString msgFail = "Generate_bit 启动失败";
        const QString msgSuccess = "Generate_bit 启动成功";
        ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
        // TODO 需要监听process的输出，打印到控制台
    }
}


void TaskView::downloadBit() {
    QString family = "xc7";
    std::string GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    std::string GLOBAL_ARCH_NAME = "xc7a50t_test";
    std::string GLOBAL_PART_NAME = "xc7a35tfgg484-2";
    std::string projectPath = "E:/workspace/demo_files/00_multi_demo";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,GLOBAL_ARCH_NAME,GLOBAL_PART_NAME,constraintPathList,topName);

    std::string script = CommandBuilder::instance().generateDownloadBitCommands("digilent_hs3",projectPath,"top.bit");
    const QString phase = "Download_bit";
    const QString path = QString::fromStdString(projectPath);
    const QString msgFail = "Download_bit 失败";
    const QString msgSuccess = "Download_bit 成功";
    qDebug() << QString::fromStdString(script);
    ProcessManager::instance().checkCall(phase, path, QString::fromStdString(script));
    // TODO 需要监听process的输出，打印到控制台
}