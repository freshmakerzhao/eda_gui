#include <QMessageBox>
#include "TaskManager.h"
#include "utils/ProcessManager.h"
#include "utils/StringUtilities.h"
#include "utils/CommandBuilder.h"
#include "ProjectManager.h"
#include "widgets/InfoWidget.h"
#include "widgets/FrameView.h"
#include "mainwindow.h"
#include "FileHelper.h"
#include "service/HardWareManager.h"
#include "dialog/AdvancedFileDialog.h"
#include "base/Globals.h"
#include "widgets/LogWidget.h"

TaskManager& TaskManager::instance()
{
    static TaskManager instance;
    return instance;
}

void TaskManager::handleTreeItemActivation(const int &mode)
{
    ProcessManager::instance().configDisplay(displayPartName);
    if (this->arch == ""){
        // 用户未选择架构时
        CustomMessageBox::critical(MainWindow::instance(), "Failed", "Please select or create a project.");
        return;
    }
    if (!MainWindow::instance()->saveAllFile()) {
        return;
    }
    // 双击触发
    if (mode == 0) {
        // 综合
        taskController(0);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 1) {
        // synthReport();
    } else if (mode == 2) {
        // pack place route全流程
        taskController(2);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 3) {
//        buildPack();
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 6) {
        // impReport();
    } else if (mode == 4) {
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 5) {
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 8) {
        QString arguments = buildBitScript();
        publishScript(projectSynthPath,arguments);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 9) {
        qDebug() << "[TaskManager] arch " << this->arch;
        std::string tileGridPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tilegrid_" + this->arch.toStdString() + ".json";
        std::string tileColorPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tile_info_map.json";
        std::string pinsInfoPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/pins_info_" + this->arch.toStdString() + ".json";
        qDebug() << "[TaskManager] tileGridPath " << QString::fromStdString(tileGridPath);
        qDebug() << "[TaskManager] tileColorPath " << QString::fromStdString(tileColorPath);
        qDebug() << "[TaskManager] pinsInfoPath " << QString::fromStdString(pinsInfoPath);
        if (gridView) {
            delete gridView;  // 删除现存的对象
            gridView = nullptr;  // 确保指针不再指向已删除的对象
        }
        gridView = new FrameView(tileGridPath,tileColorPath,pinsInfoPath, projectImplPath);
        gridView->resize(1000, 800);
        gridView->show();
    } else if (mode == 7) {
        // if (!frameView) {
        //     frameView = new NetlistView();
        // }
        // frameView->resize(1000, 800);
        // frameView->show();
    } else if (mode == 10) {
        // downloadBit();
        HardWareManager::instance().openProgramDeviceAndDownload(0);
        // 激活 log 窗口
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 11) {
        ProjectManager::instance().addSourcesAction();
    } else if (mode == 12) {
        MainWindow::instance()->showIPCatalog();
    } else if (mode == 13) {
        SettingsDialog dialog;
        dialog.exec();
        // if (settingDialog) {
        //     delete settingDialog;
        //     settingDialog = nullptr;
        // }
        // settingDialog = new SettingsDialog;
        // settingDialog->show();
    } else if (mode == 14) {
        MainWindow::instance()->showPrjSummary();
    } else if (mode == 15) {
        HardWareManager::instance().openProgramDeviceAndDownload(1);
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 16) {
        // HardWareManager::instance().openProgramDevice(2);
        QDialog dialog;
        QFormLayout *formLayout = new QFormLayout(&dialog);
        QLineEdit lineEdit;
        formLayout->addRow("Register Address: ", &lineEdit);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        formLayout->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, [&lineEdit, &dialog, this](){
            QString regAddress = lineEdit.text();
            qDebug() << "Reg Address:" << regAddress;
            readBackRegister(regAddress);
            dialog.accept();
        });
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        dialog.exec();
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 17) {
        QDialog dialog;
        dialog.setFixedWidth(600);
        QLabel label("Rbd file path: ");
        QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
        QHBoxLayout *hboxLayout = new QHBoxLayout();
        hboxLayout->addWidget(&label);
        QLineEdit *lineEdit = new QLineEdit();
        hboxLayout->addWidget(lineEdit);
        QPushButton *browseButton = new QPushButton("Browse...");
        hboxLayout->addWidget(browseButton);
        mainLayout->addLayout(hboxLayout);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        mainLayout->addWidget(buttonBox);
        QObject::connect(browseButton, &QPushButton::clicked, [lineEdit]() {
            QString filePath = AdvancedFileDialog::getSaveFileName(nullptr, "Save Rbd File", "", "*.rbd;;All Files (*)");
            if (!filePath.isEmpty()) {
                if (QFileInfo(filePath).suffix().isEmpty()) {
                    filePath += ".rbd";
                }
                lineEdit->setText(filePath);
            }
        });
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, [&lineEdit, &dialog, this]() {
            QString rbdFilePath = lineEdit->text();
            qDebug() << "RbdFilePath:" << rbdFilePath;
            readBackMemory(rbdFilePath);
            dialog.accept();
        });
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        dialog.exec();
        InfoWidget::instance()->setCurrentPage(2);
    } else if (mode == 18) {
        // simulation
        publishScript(projectSimPath,buildSimScript());
    }
}
/**
 * 任务分配器
 * @param mode 待执行任务
 */
void TaskManager::taskController(const int mode) {
    const bool sourceModified = fileChanged;
    std::string netlistPath = StringUtilities::concatPath({projectSynthPath.toStdString(), (topName + netlistType).toStdString()});
    const bool netlistExist = FileHelper::fileExists(netlistPath);
    this->initMessageStatus();
    if (mode == 0){
        // 综合操作
        if (netlistExist){
            if (sourceModified){
                // 有网表且文件有改动
                if(twoOptionMsg(
                        "Run Synthesis",
                        "Re-running synthesis will result in resetting implementation and removing all results files. OK to proceed?",
                        QMessageBox::Ok,
                        QMessageBox::Cancel
                )){
                    // 用户点击OK
                    this->setSynthSuccessMsgStatus(true);
                    this->setNextImplementProcessStatus(false);
                    // 执行综合
                    QString arguments = buildSynthScript();
                    publishScript(projectSynthPath,arguments);
                    fileChanged = false;
                    return;
                } else {
                    // 取消操作
                    return;
                }
            } else {
                // 有网表且文件没有改动
                if(twoOptionMsg(
                        "Run Synthesis",
                        "Synthesis has already completed and is up to date. Re-run anyway?",
                        QMessageBox::Ok,
                        QMessageBox::Cancel
                )){
                    // 用户点击OK
                    this->setSynthSuccessMsgStatus(true);
                    this->setNextImplementProcessStatus(false);
                    // 执行综合
                    QString arguments = buildSynthScript();
                    publishScript(projectSynthPath,arguments);
                    fileChanged = false;
                    return;
                } else {
                    // 取消操作
                    return;
                }
            }
        } else {
            // 如果网表不存在
            this->setSynthSuccessMsgStatus(true);
            this->setNextImplementProcessStatus(false);
            // 直接执行
            QString arguments = buildSynthScript();
            publishScript(projectSynthPath,arguments);
            fileChanged = false;
            return;
        }
    } else if (mode == 2){
        // implement
        std::string implResultPath = StringUtilities::concatPath({projectImplPath.toStdString(), (topName + implResultType).toStdString()});
        bool implementExist = FileHelper::fileExists(implResultPath);
        if (netlistExist){
            if (implementExist){
                // 存在
                if (sourceModified){
                    // 设计文件有改动
                    if(twoOptionMsg(
                            "Synthesis is Out-of-date",
                            "Synthesis is out-of-date. OK to launch synthesis first? Implementation will automatically start when synthesis completes.",
                            QMessageBox::Ok,
                            QMessageBox::Cancel
                    )){
                        // 重新综合
                        // 综合命令
                        QString synthScript = buildSynthScript();
                        // implement命令
                        QString implScript = buildImpScript();

                        this->setSynthSuccessMsgStatus(false);
                        this->setNextImplementProcessStatus(true);

                        setNextPhaseParam("implementation", projectImplPath, implScript);
                        publishScript(projectSynthPath,synthScript);
                        fileChanged = false;
                        return;
                    } else {
                        // 不操作
                        return;
                    }
                } else {
                    // 设计文件没有改动
                    if(twoOptionMsg(
                            "Run Implement",
                            "A completed implementation run exists. Re-run anyway?",
                            QMessageBox::Ok,
                            QMessageBox::Cancel
                    )){
                        // 重新implement
                        this->setSynthSuccessMsgStatus(false);
                        this->setNextImplementProcessStatus(true);
                        QString implScript = buildImpScript();
                        publishScript(projectImplPath,implScript);
                        fileChanged = false;
                        return;
                    } else {
                        // 不操作
                        return;
                    }
                }
            } else {
                // 没有net、place、route文件
                if (sourceModified){
                    // 设计文件有改动
                    if(twoOptionMsg(
                            "Synthesis is Out-of-date",
                            "Synthesis is out-of-date. OK to launch synthesis first? Implementation will automatically start when synthesis completes.",
                            QMessageBox::Ok,
                            QMessageBox::Cancel
                    )){
                        // 重新综合
                        // 综合命令
                        QString synthScript = buildSynthScript();
                        // implement命令
                        QString implScript = buildImpScript();
                        // 不显示综合成功弹窗
                        this->setSynthSuccessMsgStatus(false);
                        this->setNextImplementProcessStatus(true);

                        // 定义综合后执行的命令
                        this->setNextPhaseParam("implementation", projectImplPath, implScript);
                        // 执行综合
                        publishScript(projectSynthPath,synthScript);
                        fileChanged = false;
                        return;
                    } else {
                        // 不操作
                        return;
                    }
                } else {
                    // 设计文件没有改动
                    // 直接执行implement
                    this->setSynthSuccessMsgStatus(false);
                    this->setNextImplementProcessStatus(true);
                    QString implScript = buildImpScript();
                    publishScript(projectImplPath,implScript);
                    fileChanged = false;
                    return;
                }
            }
        } else {
            // 网表不存在
            if(twoOptionMsg(
                    "Missing Syntheis Results",
                    "There is no netlist available. OK to launch synthesis first? Implementation will automatically start when synthesis completes.",
                    QMessageBox::Ok,
                    QMessageBox::Cancel
            )){
                // 综合
                // 综合命令
                QString synthScript = buildSynthScript();
                // implement命令
                QString implScript = buildImpScript();
                // 不显示综合成功弹窗
                this->setSynthSuccessMsgStatus(false);
                this->setNextImplementProcessStatus(true);
                // 定义综合后执行的命令
                setNextPhaseParam("implementation", projectImplPath, implScript);
                publishScript(projectSynthPath,synthScript);
                fileChanged = false;
                return;
            } else {
                // 取消操作
                return;
            }
        }
    }
}

/**
  * 设置工程参数
  */
void TaskManager::setParams(const QMap<Project::ParamKey, QString> &params)
{
    // 存储路径
    QString path = params[Project::Path];
    projectSynthPath = path + "/runs/synth";
    projectImplPath = path + "/runs/impl";
    projectSimPath = path + "/run/sim";
    projectPath = path;
    topName = params[Project::TopModule];
    // 存储partname
    partName = params[Project::Part];
    displayPartName = params[Project::DisplayPart];
    archName = params[Project::ArchName];
    arch = params[Project::Arch];
}

/**
  * 关闭工程清除参数
  */
void TaskManager::cleanParams()
{
    sourcePathList.clear();
    constraintPathList.clear();
    simPathList.clear();
    projectSynthPath = "";
    projectImplPath = "";
    projectPath = "";
    partName = "";
    archName = "";
    arch = "";
    removeWatchFiles();
    fileChanged = false;
}

void TaskManager::setWatchFiles() {
    fileWatcher->addPaths(sourcePathList);
    fileWatcher->addPaths(constraintPathList);
    fileWatcher->addPaths(simPathList);
}

void TaskManager::addWatchFiles(const QStringList &filePath)
{
    fileWatcher->addPaths(filePath);
}

void TaskManager::removeWatchFiles() {
    fileWatcher->removePaths(fileWatcher->files());
}

void TaskManager::removeWatchFile(const QString &filePath)
{
    fileWatcher->removePath(filePath);
}

TaskManager::TaskManager()
{
    qDebug() << "[TaskManager] Constructing...";
    fileWatcher = new QFileSystemWatcher(this);
    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &TaskManager::onFileChanged);
    connect(&ProcessManager::instance(), &ProcessManager::finishMessage, this, &TaskManager::handleMessage);
}

TaskManager::~TaskManager()
{
    qDebug() << "[TaskManager] Distructing...";
}

/**
 * 返回综合命令
 * @return
 */
QString TaskManager::buildSynthScript() {
    QStringList options;

    if (ProjectManager::instance().getParameter(Project::CompatibilityMode) == "enable")
        options << "-compatibility_mode";
    return QString("synth_design %1").arg(options.join(" "));
}

QString TaskManager::buildImpScript() {
    return QString("impl_design");
}

QString TaskManager::buildBitScript() {
    QStringList options;

    if (ProjectManager::instance().getParameter(Project::BinFile) == "enable")
        options << "-bin";
    if (ProjectManager::instance().getParameter(Project::RbtFile) == "enable")
        options << "-rbt";
    if (ProjectManager::instance().getParameter(Project::CRCOption) == "enable")
        options << "-crc";
    if (ProjectManager::instance().getParameter(Project::CompressOption) == "enable")
        options << "-compress";
    return QString("write_bitstream %1").arg(options.join(" "));
}

QString TaskManager::buildSimScript() {
    return QString("sim_design");
}

void TaskManager::onFileChanged() {
    fileChanged = true;
    qDebug("\033[43m[FileWatcher]\033[0m File Changed");
}

// 烧写位流
void TaskManager::downloadBit(const QString &bitstream, const QString &cable_name) {
    std::string script = CommandBuilder::instance().generateDownloadBitCommands(bitstream, cable_name);
    ProcessManager::instance().executeCommand("Download Bitstream", QStringList() << QString::fromStdString(script));
}

void TaskManager::readBackRegister(const QString &registerAddress) {
//    ProcessManager::instance().initEnvironment(familyName,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
//
//    std::string script = CommandBuilder::instance().generateReadBackRegisterCommands(partName, registerAddress);
}

void TaskManager::readBackMemory(const QString &rbdFilePath) {
//    ProcessManager::instance().initEnvironment(familyName,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
//
//    std::string script = CommandBuilder::instance().generateReadMemoryCommands(partName, rbdFilePath);
}

void TaskManager::downloadFlash(const QString &projectImplPath1, const QString &topName1) {
//    ProcessManager::instance().initEnvironment(familyName,GLOBAL_RESOURCE_PATH,archName,partName,constraintPathList,topName);
//    if (projectImplPath1.isEmpty() && topName1.isEmpty()) {
//        std::string script = CommandBuilder::instance().generateDownloadFlashCommands(projectImplPath, partName, topName);
//    } else {
//        std::string script = CommandBuilder::instance().generateDownloadFlashCommands(projectImplPath1, partName, topName1);
//    }
}

// 两个选项的弹窗，true 左侧，false 右侧
bool TaskManager::twoOptionMsg(const QString &title, const QString &text, QMessageBox::StandardButton buttonLeft, QMessageBox::StandardButton buttonRight) {
    // 等待用户响应
    int msg = CustomMessageBox::question(
            MainWindow::instance(),
            title,
            text,
            buttonLeft | buttonRight
        );
    // 根据用户选择做出响应
    if (msg == buttonLeft) {
        return true;
    } else if (msg == buttonRight) {
        return false;
    }
    return false;
}

void TaskManager::handleMessage(ProcessMessage &msg) {
    // 如果当前执行完毕
    // exitCode 为0表示正常执行并成功退出
    if (msg.exitCode == 0) {
        MainWindow::instance()->setRunState(msg.phase + " Complete!", false);
        if (msg.phase == "Synthesis"){
            // 综合结束后，读取资源统计信息
            InfoWidget::instance()->updateSynthItem(
                    msg.workPath,
                    msg.phase + " Complete!",
                    msg.startTime,
                    msg.elapsedTime,
                    msg.displayPartName);
            if(this->_showSynthSuccessMsg){
                // 只有接下来不做操作时，才弹出窗口
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
                // 需要弹窗则弹窗
                CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
            }
            if(this->_hasNextImplementProcess){
                // 如果需要做
                this->publishScript(this->_nextWorkPath, this->_nextTclCommand);
            }
        } else if (msg.phase == "Implementation"){
            InfoWidget::instance()->updateImplItem(
                    msg.workPath,
                    msg.phase + " Complete!",
                    msg.startTime,
                    msg.elapsedTime,
                    msg.displayPartName);
            if(this->_showImplementSuccessMsg){
                // 只有接下来不做操作时，才弹出窗口
                // Implementation结束后，读取资源统计信息
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
                CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
            }
        } else {
            // 生成码流结束提示，后续在此扩展
            CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
        }

        //输出阶段耗时，内存占用
        LogWidget::instance()->appendLog(
            msg.phase + "_design:" +
            " Time (s): elapsed = " + msg.elapsedTime +
            " Memory (MB): peak = " + QString::number(ProcessManager::instance().getPeak(), 'f', 2) +
            " gain = " + QString::number(ProcessManager::instance().getGain(), 'f', 2)
            );
        LogWidget::instance()->appendLog(QString("-").repeated(100));

        return;
    }

    // Terminate or Fail
    if (msg.isCancel) {
        MainWindow::instance()->resetRunState();
    } else {
        MainWindow::instance()->setRunState(msg.phase + " failed.", false);
        CustomMessageBox::critical(MainWindow::instance(), msg.phase + " Failed", msg.phase + " failed.");
    }


}

// 将命令提交给tcl console
void TaskManager::publishScript(const QString &workPath, const QString &tclCommand) {
    Q_UNUSED(workPath); // 在TclConsole设置路径
    TclConsole::instance()->executeTclCommand(tclCommand);
}

void TaskManager::setNextPhaseParam(const QString &nextPhase, const QString &nextWorkPath, const QString &nextTclCommand) {
    this->_nextPhase = nextPhase;
    this->_nextWorkPath = nextWorkPath;
    this->_nextTclCommand = nextTclCommand;
}

void TaskManager::setSynthSuccessMsgStatus(bool status) {
    this->_showSynthSuccessMsg = status;
}

void TaskManager::setImplementSuccessMsgStatus(bool status) {
    this->_showImplementSuccessMsg = status;
}

void TaskManager::setNextImplementProcessStatus(bool status) {
    this->_hasNextImplementProcess = status;
}

void TaskManager::initMessageStatus() {
    this->_showSynthSuccessMsg = true;
    this->_showImplementSuccessMsg = true;
    this->_hasNextImplementProcess = false;
    this->_nextPhase = nullptr;
    this->_nextWorkPath = nullptr;
    this->_nextTclCommand = nullptr;
}
