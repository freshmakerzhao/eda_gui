/**
  ******************************************************************************
  * @file           : TaskManager.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/4
  ******************************************************************************
  */
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
#include "dialog/CustomMessageBox.h"
#include "settings/SettingsDialog.h"
#include "base/Globals.h"
#include "widgets/LogWidget.h"
#include "component/timing/TimingSummaryDialog.h"
#include "component/power/ReportPowerDialog.h"

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
        flowTaskController(0);
    } else if (mode == 1) {
        // synthReport();
    } else if (mode == 2) {
        // pack place route全流程
        flowTaskController(2);
    } else if (mode == 3) {
//        buildPack();
    } else if (mode == 6) {
        // impReport();
    } else if (mode == 8) {
        flowTaskController(8);
    } else if (mode == 9) {
        qDebug() << "[TaskManager] arch " << this->arch;
        std::string tileGridPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + QString("/chip_view/maps/tilegrid_%1.json").arg(this->arch).toStdString();
        std::string tileColorPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tile_info_map.json";
        qDebug() << "[TaskManager] tileGridPath " << QString::fromStdString(tileGridPath);
        qDebug() << "[TaskManager] tileColorPath " << QString::fromStdString(tileColorPath);
        if (glwidget3) {
            delete glwidget3;
            glwidget3 = nullptr;
        }
        if (gridView) {
            delete gridView;  // 删除现存的对象
            gridView = nullptr;  // 确保指针不再指向已删除的对象
        }
        glwidget3 = new FPGAOpenGLWidget3;
        gridView = new FrameView(tileGridPath, tileColorPath, "", glwidget3);
        glwidget3->resize(1600, 900);
        glwidget3->show();
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
        TclConsole::instance()->executeTclCommand(buildSimScript());
    } else if (mode == 19) {
        TimingSummaryDialog dialog;
        if (dialog.exec() == QDialog::Accepted)
            emit timingDialogAccept();
    } else if (mode == 20) {
        ReportPowerDialog dialog;
        if (dialog.exec() == QDialog::Accepted)
            emit powerDialogAccept();
    }
}

void TaskManager::flowTaskController(const int &mode) {
    const bool sourceModified = fileChanged;
    std::string netlistPath = StringUtilities::concatPath({projectSynthPath.toStdString(), (topName + netlistType).toStdString()});
    const bool netlistExist = FileHelper::fileExists(netlistPath);
    std::string implResultPath = StringUtilities::concatPath({projectImplPath.toStdString(), (topName + implResultType).toStdString()});
    const bool implementExist = FileHelper::fileExists(implResultPath);

    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
    if (!netlistExist) {
        settings.setValue(FlowPhase::Synthsis, FlowState::NoStarted);
    }
    if (!implementExist) {
        settings.setValue(FlowPhase::Implementation, FlowState::NoStarted);
        settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
    }
    settings.sync();

    //! Clear the task queue before each start.
    flowTaskQueue.clear();

    switch (mode) {
    case 0:
        handleSynthClick();
        break;
    case 2:
        handleImplClick();
        break;
    case 8:
        handleGenBitClick();
        break;
    default:
        break;
    }

    if (flowTaskQueue.empty()) {
        qDebug("\033[32m[Click Cancel on any MessageBox.]\033[0m");
        return;
    }

    InfoWidget::instance()->setCurrentPage(2);

    qDebug("\033[32m[Run the first task in the task queue.]\033[0m");
    handleFlowTaskQueue();
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

void TaskManager::initStateMachine()
{
    //! Create .state if it does not exist.
    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
    if (!settings.contains(FlowPhase::Synthsis)) {
        settings.setValue(FlowPhase::Synthsis, FlowState::NoStarted);
    }
    if (!settings.contains(FlowPhase::Implementation)) {
        settings.setValue(FlowPhase::Implementation, FlowState::NoStarted);
    }
    if (!settings.contains(FlowPhase::GenerateBitstream)) {
        settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoStarted);
    }
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
    connect(this, &TaskManager::timingDialogAccept, InfoWidget::instance(), &InfoWidget::generateTimingSummary);
    connect(this, &TaskManager::powerDialogAccept, InfoWidget::instance(), &InfoWidget::generateReportPower);
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

    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
    if (settings.value(FlowPhase::Synthsis) != FlowState::NoStarted) {
        settings.setValue(FlowPhase::Synthsis, FlowState::OutOfDate);
    }
    if (settings.value(FlowPhase::Implementation) != FlowState::NoStarted) {
        settings.setValue(FlowPhase::Implementation, FlowState::NoAvailable);
    }
    if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
        settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
    }
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

void TaskManager::handleMessage(ProcessMessage &msg) {
    // 如果当前执行完毕
    // exitCode 为0表示正常执行并成功退出
    if (msg.exitCode == 0) {
        MainWindow::instance()->setRunState(msg.phase + " Complete!", false);
        if (msg.phase == "Synthesis"){
            QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
            settings.setValue(FlowPhase::Synthsis, FlowState::Complete);
            // 综合结束后，读取资源统计信息
            InfoWidget::instance()->updateSynthItem(
                    msg.workPath,
                    msg.phase + " Complete!",
                    msg.startTime,
                    msg.elapsedTime,
                    msg.displayPartName);
            if(this->_isShowSuccessMessage) {
                // 只有接下来不做操作时，才弹出窗口
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
                // 需要弹窗则弹窗
                CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
            }
        } else if (msg.phase == "Implementation"){
            QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
            settings.setValue(FlowPhase::Implementation, FlowState::Complete);
            InfoWidget::instance()->updateImplItem(
                    msg.workPath,
                    msg.phase + " Complete!",
                    msg.startTime,
                    msg.elapsedTime,
                    msg.displayPartName);
            if(this->_isShowSuccessMessage){
                // 只有接下来不做操作时，才弹出窗口
                // Implementation结束后，读取资源统计信息
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
                CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
            }
        } else if (msg.phase == "Generate Bitstream"){
            QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::Complete);
            InfoWidget::instance()->updateImplItem(
                msg.workPath,
                msg.phase + " Complete!",
                msg.startTime,
                msg.elapsedTime,
                msg.displayPartName);
            // 生成码流结束提示
            CustomMessageBox::information(MainWindow::instance(), msg.phase + " Completed", msg.phase + " successfully completed.");
        }

        //输出阶段耗时，内存占用
        LogWidget::instance()->appendLog(
            msg.phase,
            msg.phase + "_design:" +
            " Time (s): elapsed = " + msg.elapsedTime +
                " Memory (MB): peak = " + QString::number(MemoryUtilities::instance()->getPeakMemory(), 'f', 2) +
                " gain = " + QString::number(MemoryUtilities::instance()->getGainMemory(), 'f', 2)
            );
        LogWidget::instance()->appendLog(msg.phase, QString("-").repeated(100));
        LogManager::instance().firstSubPhaseStatus = true;
        
        qDebug("\033[32m[Run the next task.]\033[0m");
        handleFlowTaskQueue();

        return;
    }

    LogManager::instance().firstSubPhaseStatus = true;

    // Terminate or Fail
    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);
    if (msg.isCancel) {
        if (msg.phase == "Synthesis")          settings.setValue(FlowPhase::Synthsis, FlowState::NoAvailable);
        if (msg.phase == "Implementation")     settings.setValue(FlowPhase::Implementation, FlowState::NoAvailable);
        if (msg.phase == "Generate Bitstream") settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);

        MainWindow::instance()->resetRunState();
    } else {
        if (msg.phase == "Synthesis")          settings.setValue(FlowPhase::Synthsis, FlowState::Fail);
        if (msg.phase == "Implementation")     settings.setValue(FlowPhase::Implementation, FlowState::Fail);
        if (msg.phase == "Generate Bitstream") settings.setValue(FlowPhase::GenerateBitstream, FlowState::Fail);

        MainWindow::instance()->setRunState(msg.phase + " failed.", false);
        CustomMessageBox::critical(MainWindow::instance(), msg.phase + " Failed", msg.phase + " failed.");
    }
}

void TaskManager::handleFlowTaskQueue()
{
    if (!flowTaskQueue.empty()) {
        //! Show MessageBox after executing the last task.
        _isShowSuccessMessage = (flowTaskQueue.size() == 1) ? true : false;

        //! Execute task.
        FlowTask &task = flowTaskQueue.front();
        switch (task) {
        case FlowTask::Synthesis:
            TclConsole::instance()->executeTclCommand(buildSynthScript());
            LogWidget::instance()->switchSynLog();
            break;
        case FlowTask::Implementation:
            TclConsole::instance()->executeTclCommand(buildImpScript());
            LogWidget::instance()->switchImpLog();
            break;
        case FlowTask::WriteBitstream:
            TclConsole::instance()->executeTclCommand(buildBitScript());
            break;
        default:
            break;
        }

        flowTaskQueue.dequeue();
        qDebug() << "\033[32mRemaining tasks:" << flowTaskQueue.size() << "\033[0m";
        return;
    }

    qDebug("\033[32m[Task queue completed.]\033[0m");
}

void TaskManager::handleSynthClick()
{
    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);

    if (settings.value(FlowPhase::Synthsis) == FlowState::Complete) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "Run Synthesis",
            "Synthesis has already completed and is up to date. Re-run anyway?",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        if (settings.value(FlowPhase::Implementation) != FlowState::NoStarted) {
            if (QMessageBox::Cancel == CustomMessageBox::question(
                MainWindow::instance(),
                "Run Synthesis",
                "Re-running synthesis will result in resetting implementation and removing all results files. OK to proceed?",
                QMessageBox::Ok | QMessageBox::Cancel)) {
                return;
            }
            settings.setValue(FlowPhase::Implementation, FlowState::NoAvailable);
        }
        if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        fileChanged = false;
        return;
    }

    if (settings.value(FlowPhase::Synthsis) == FlowState::OutOfDate) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "Run Synthesis",
            "Re-running synthesis will result in resetting implementation and removing all results files. OK to proceed?",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        if (settings.value(FlowPhase::Implementation) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::Implementation, FlowState::NoAvailable);
        }
        if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        fileChanged = false;
        return;
    }

    // No MessageBox needed.
    flowTaskQueue.enqueue(FlowTask::Synthesis);
    fileChanged = false;
}

void TaskManager::handleImplClick()
{
    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);

    if (settings.value(FlowPhase::Synthsis) == FlowState::OutOfDate) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
                MainWindow::instance(),
                "Synthesis is Out-of-date",
                "Synthesis is out-of-date. OK to launch synthesis first? Implementation will automatically start when synthesis completes.",
                QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        flowTaskQueue.enqueue(FlowTask::Implementation);
        fileChanged = false;
        return;
    }

    //! Includes NoStarted, NoAvailable, Fail, except for OutOfDate status.
    if (settings.value(FlowPhase::Synthsis) != FlowState::Complete) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "Missing Synthesis Results",
            "There is no netlist available. OK to launch synthesis first? Implementation will automatically start when synthesis completes.",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        flowTaskQueue.enqueue(FlowTask::Implementation);
        fileChanged = false;
        return;
    }

    if (settings.value(FlowPhase::Implementation) == FlowState::Complete) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "Run Implement",
            "A completed implementation run exists. Re-run anyway?",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
            settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
        }
        flowTaskQueue.enqueue(FlowTask::Implementation);
        fileChanged = false;
        return;
    }

    // No MessageBox needed.
    if (settings.value(FlowPhase::GenerateBitstream) != FlowState::NoStarted) {
        settings.setValue(FlowPhase::GenerateBitstream, FlowState::NoAvailable);
    }
    flowTaskQueue.enqueue(FlowTask::Implementation);
    fileChanged = false;
    return;
}

void TaskManager::handleGenBitClick()
{
    QSettings settings(projectPath + "/runs/.works/.state", QSettings::IniFormat);

    if (settings.value(FlowPhase::Synthsis) == FlowState::OutOfDate) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
                MainWindow::instance(),
                "Synthesis is Out-of-date",
                "Synthesis is out-of-date. OK to launch synthesis and implementation first? 'Generate Bitstream' will automatically start when synthesis and implementation completes.",
                QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        flowTaskQueue.enqueue(FlowTask::Implementation);
        flowTaskQueue.enqueue(FlowTask::WriteBitstream);
        fileChanged = false;
        return;
    }

    //! Includes NoStarted, NoAvailable, Fail, except for OutOfDate status.
    if (settings.value(FlowPhase::Synthsis) != FlowState::Complete) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "No implementation Results Available",
            "There are no implementation results available. OK to launch synthesis and implementation? 'Generate Bitstream' will automatically start when synthesis and implementation completes.",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        flowTaskQueue.enqueue(FlowTask::Synthesis);
        flowTaskQueue.enqueue(FlowTask::Implementation);
        flowTaskQueue.enqueue(FlowTask::WriteBitstream);
        fileChanged = false;
        return;
    }

    if ((settings.value(FlowPhase::Synthsis) == FlowState::Complete) && ((settings.value(FlowPhase::Implementation) != FlowState::Complete))) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "No implementation Results Available",
            "There are no implementation results available. OK to launch implementation? 'Generate Bitstream' will automatically start when implementation completes.",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        flowTaskQueue.enqueue(FlowTask::Implementation);
        flowTaskQueue.enqueue(FlowTask::WriteBitstream);
        fileChanged = false;
        return;
    }

    if (settings.value(FlowPhase::GenerateBitstream) == FlowState::Complete) {
        if (QMessageBox::Cancel == CustomMessageBox::question(
            MainWindow::instance(),
            "Generate Bitstream",
            "Bitstream generation has already completed and is up-to-date. Re-run anyway?",
            QMessageBox::Ok | QMessageBox::Cancel)) {
            return;
        }
        flowTaskQueue.enqueue(FlowTask::WriteBitstream);
        fileChanged = false;
        return;
    }

    // No MessageBox needed.
    flowTaskQueue.enqueue(FlowTask::WriteBitstream);
    fileChanged = false;
}
