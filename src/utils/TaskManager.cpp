#include <QMessageBox>
#include "TaskManager.h"
#include "utils/ProcessManager.h"
#include "utils/StringUtilities.h"
#include "utils/CommandBuilder.h"
#include "widgets/Infowidget.h"
#include "widgets/FrameView.h"
#include "mainwindow.h"
#include "dialog/CustomMessageBox.h"
#include "widgets/Navigator.h"
#include "FileHelper.h"

TaskManager& TaskManager::instance()
{
    static TaskManager instance;
    return instance;
}

void TaskManager::handleTreeItemActivation(const int mode)
{
    if (this->arch == ""){
        // 用户未选择架构时
        // QMessageBox::critical(MainWindow::instance(), "Failed", "Please select or create a project.");
        CustomMessageBox::showQuestion(MainWindow::instance(), "Failed", "Please select or create a project.");
        return;
    }
    if (!MainWindow::instance()->saveAllFile()) {
        return;
    }
    // 双击触发
    if (mode == 0) {
        runSynth();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 1) {
        // synthReport();
    } else if (mode == 2) {
        // pack place route全流程
        buildImp();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 3) {
        buildPack();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 6) {
        // impReport();
    } else if (mode == 4) {
        buildPlace(3);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 5) {
        buildRoute();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 8) {
        buildBit(2);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 9) {
        qDebug() << "[TaskManager] arch " << this->arch;
        std::string tileGridPath = GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tilegrid_" + this->arch.toStdString() + ".json";
        std::string tileColorPath = GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tile_info_map.json";
        qDebug() << "[TaskManager] tileGridPath " << QString::fromStdString(tileGridPath);
        qDebug() << "[TaskManager] tileColorPath " << QString::fromStdString(tileColorPath);
        if (gridView) {
            delete gridView;  // 删除现存的对象
            gridView = nullptr;  // 确保指针不再指向已删除的对象
        }
        gridView = new FrameView(tileGridPath,tileColorPath,projectImplPath);
        gridView->resize(1000, 800);
        gridView->show();
    } else if (mode == 7) {
        // if (!frameView) {
        //     frameView = new NetlistView();
        // }
        // frameView->resize(1000, 800);
        // frameView->show();
    } else if (mode == 10) {
        downloadBit();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 11) {
        Navigator::instance()->addSourcesAction();
    }
}

/**
  * 设置工程参数
  */
void TaskManager::setParams(QMap<QString, QString> params)
{
    // 存储路径
    QString path = params["path"];
    projectSynthPath = path + "/runs/synth";
    projectImplPath = path + "/runs/impl";
    projectPath = path;
    // 存储partname
    partName = params["part"];
    archName = params["archName"];
    arch = params["arch"];

    // 测试用
    QString TEST_PATH1 = "E:/workspace/qt_demo/resource_win";
    QString TEST_PATH2 = "C:/HybrdLink/resource_win";
    QString TEST_PATH3 = "C:/Users/INTEL/Desktop/Work/VMwareFileWorkspace/HybrdLink/resource_win";
    // 打包用
    QString PACK_PATH = QString::fromStdString(StringUtilities::concatPath({QCoreApplication::applicationDirPath().toStdString(), "resource_win"}));

    QFileInfo fileInfo1(TEST_PATH1);
    QFileInfo fileInfo2(TEST_PATH2);
    QFileInfo fileInfo3(TEST_PATH3);
    QFileInfo fileInfo4(PACK_PATH);

    if(fileInfo1.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH1;
    } else if (fileInfo2.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH2;
    } else if (fileInfo3.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH3;
    } else if (fileInfo4.exists()) {
        GLOBAL_RESOURCE_PATH = PACK_PATH;
    }
    qDebug() << GLOBAL_RESOURCE_PATH;
}

/**
  * 关闭工程清除参数
  */
void TaskManager::cleanParams()
{
    sourcePathList.clear();
    constraintPathList.clear();
    projectSynthPath = "";
    projectImplPath = "";
    projectPath = "";
    partName = "";
    archName = "";
    arch = "";
}

TaskManager::TaskManager()
{
    qDebug() << "[TaskManager] Constructing...";
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
    ProcessManager::instance().checkCall("Pack", projectImplPath, QString::fromStdString(script),partName);
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
        ProcessManager::instance().checkCall("IOPlace Generation", projectPath, QString::fromStdString(script),partName);
    }

    // 生成top.ioplace 与 constrains.place
    if (mode == 2) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        ProcessManager::instance().checkCall("Constraints Generation", projectImplPath, QString::fromStdString(script),partName);
    }

    // 完成 vpr_place
    if (mode == 3) {
        script = CommandBuilder::instance().generateImpIOPlaceCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script = script + " && " + CommandBuilder::instance().generateImpConstrainsCommands(projectSynthPath,projectImplPath,"%PYTHON3%");
        script += " && " + CommandBuilder::instance().generateImpPlaceCommands(projectSynthPath,projectImplPath,archName);
        ProcessManager::instance().checkCall("Place", projectImplPath, QString::fromStdString(script),partName);
    }
}


// Route 阶段
void TaskManager::buildRoute() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);

    std::string script = CommandBuilder::instance().generateImpRouteCommands(projectSynthPath,archName);
    ProcessManager::instance().checkCall("Route", projectImplPath, QString::fromStdString(script),partName);
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
    ProcessManager::instance().checkCall("Implementation", projectImplPath, QString::fromStdString(script),partName);
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
        ProcessManager::instance().checkCall("Fasm Generation", projectImplPath, QString::fromStdString(script),partName);
    }
    if (mode == 2) {
        std::string script = CommandBuilder::instance().generateFasmCommands(projectSynthPath,archName);
        script += " && " + CommandBuilder::instance().generateBitCommands(projectImplPath,"%PYTHON3%");
        ProcessManager::instance().checkCall("Bitstream Generation", projectImplPath, QString::fromStdString(script),partName);
    }
}

void TaskManager::downloadBit() {
    QString family = "xc7";
    QString topName = "top";
    ProcessManager::instance().initEnvironment(family,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
    std::string script = CommandBuilder::instance().generateDownloadBitCommands(projectImplPath,"digilent_hs3","top.bit");
    ProcessManager::instance().checkCall("Bitstream Download", projectImplPath, QString::fromStdString(script),partName);
}
