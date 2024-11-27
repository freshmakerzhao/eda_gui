/**
  ******************************************************************************
  * @file           : ProjectManager.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/7
  ******************************************************************************
  */
#include "base/InitialConfig.h"
#include "mainwindow.h"
#include "ProjectManager.h"
#include "TaskManager.h"
#include "wizard/Wizard.h"
#include "dialog/CustomMessageBox.h"
#include "dialog/ProgressHelper.h"
#include "widgets/FileManager.h"
#include "widgets/ProjectSummary.h"
#include "widgets/InfoWidget.h"
#include "widgets/Form.h"
#include "service/RecentService.h"
#include "FileHelper.h"

ProjectManager &ProjectManager::instance()
{
    static ProjectManager instance;
    return instance;
}

bool ProjectManager::hasProjectOpen()
{
    return _project == nullptr;
}

/**
 * 创建一个新的HybrdLink工程
 * @param name
 * @param path
 * @param part
 * @param arch
 * @param archName
 * @param displayPart
 * @param familyName
 * @return
 */
bool ProjectManager::createProject(const QString &name,
                                   const QString &path,
                                   const QString &part,
                                   const QString &arch,
                                   const QString &archName,
                                   const QStringList &designSrcs,
                                   const QStringList &constraints,
                                   const QString &displayPart,
                                   const QString &familyName)
{
    // 此处的project是一个临时对象，生成工程文件后销毁
    Project *project = new Project;
    project->initProject(name,
                         path,
                         part,
                         arch,
                         archName,
                         displayPart,
                         familyName);
    project->sourceList = designSrcs;
    project->constraintList = constraints;
    project->writeProject();
    delete project;
    QString hprPath = QString("%1/%2.hpr").arg(path, name);
    if (!openProject(hprPath)) {
        return false;
    }
    return true;
}

/**
 * Reopen project on startup
 * Open project in New window
 * @param hprPath 工程文件路径
 * @return
 */
bool ProjectManager::startProcess(const QString &hprPath)
{
    CustomMessageBox::StandardButton btn = CustomMessageBox::showQuestion(MainWindow::instance(),
                                                                          "Question", "Open this Window?",
                                                                          QMessageBox::Yes | QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        closeProject();
        openProject(hprPath);
        return false;
    }

    // 获取程序路径
    QString programPath = QCoreApplication::applicationFilePath();
    // 创建一个新进程
    QProcess process;
    // 传入新打开工程路径
    // 启动程序本身
    process.startDetached(programPath, QStringList() << hprPath);

    return true;
}

/**
 * 接收工程文件(*.hpr)路径，打开工程
 * @param hprPath 工程文件路径
 * @return
 */
bool ProjectManager::openProject(const QString &hprPath)
{
    bool isCanceled = ProgressHelper::showProgressDialog(MainWindow::instance());
    if (isCanceled) {
        return false;
    }

    // 必须转换为完整路径
    const QString standardHprPath = FileHelper::convertToStandardPath(hprPath);

    Project *newOpenProject = new Project;
    if (!newOpenProject->parseProject(standardHprPath)) {
        CustomMessageBox::showError(MainWindow::instance(), "Error",
                                    "Failed to open project, File parsing error.");
        delete newOpenProject;
        return false;
    }
    ProjectManager::instance().loadFiles(newOpenProject);
    InfoWidget::instance()->initDesignRunsView(newOpenProject->getParameter(Project::Path));
    TaskManager::instance().setWatchFiles();

    // 更新 Recent Projects 列表
    RecentService::writeRecentProject(standardHprPath);
    MainWindow::instance()->setRecentMenu();

    return true;
}

/**
 * 接收命令行参数，判断是否有工程文件(*.hpr)路径，执行打开工程函数
 * @param args 命令行参数列表
 * @return
 */
bool ProjectManager::openProjectFromArgs(const QStringList &args)
{
    if(args.size() <= 1) {
        return false;
    }
    // 获取第一个文件路径
    QString hprPath = args.at(1);
    qDebug() << "Loading Project:" << hprPath;
    if (!openProject(hprPath)) {
        return false;
    }
    return true;
}

/**
 * 加载工程到TaskManager
 * @param project 工程实例
 */
void ProjectManager::loadFiles(Project *project)
{
    // 加载的不是同一个工程
    if (_project != nullptr && _project != project) {
        // 运行新进程，在新进程加载工程
        QString hprPath = QString("%1/%2.hpr").arg(project->getParameter(Project::Path), project->getParameter(Project::Name));
        ProjectManager::instance().startProcess(hprPath);
        // delete project; // 待解决
        return;
    }

    _project = project;

    qDebug() << "[ProjectManager] loadFiles...";

    // QString setDeviceCmd = QString("set_device %1").arg(_project->getParameter(Project::DisplayPart));
    // TclConsole::instance()->executeTclCommand(setDeviceCmd);

    QString setSetWorkDirCmd = QString("set_work_dir %1").arg(_project->getParameter(Project::Path));
    TclConsole::instance()->executeTclCommand(setSetWorkDirCmd);

    QString setTopModuleCmd = QString("set_top_module %1").arg(_project->getParameter(Project::TopModule));
    TclConsole::instance()->executeTclCommand(setTopModuleCmd);

    QString updateFileSetCmd = "update_fileset sources";
    TclConsole::instance()->executeTclCommand(updateFileSetCmd);
    updateFileSetCmd = "update_fileset constrs";
    TclConsole::instance()->executeTclCommand(updateFileSetCmd);

    // 存储设计文件与约束文件
    TaskManager::instance().sourcePathList = _project->sourceList;
    TaskManager::instance().constraintPathList = _project->constraintList;
    // 设置工程参数
    TaskManager::instance().setParams(_project->getAllParameters());
    // 设置工程参数
    ProjectSummary::instance()->setParams(_project->getAllParameters());
    // 加载文件树
    FileManager::instance()->updateDesignSources(_project->sourceList);
    FileManager::instance()->updateConstraints(_project->constraintList);
    // UI反馈
    MainWindow::instance()->showProjectTitle(0, _project->getParameter(Project::Path) + "/" + _project->getParameter(Project::Name) + ".hpr");
    MainWindow::instance()->setForm(0);
}

/**
 * 启动Wizard，添加Sources
 */
void ProjectManager::addSourcesAction()
{
    if (_project == nullptr) {
        CustomMessageBox::showQuestion(MainWindow::instance(), "Warning",
                                      "Please select or create a project.");
        return;
    }
    Wizard wizard(MainWindow::instance(), 1);
    wizard.exec();
    _project->writeProject();
    loadFiles(_project);
}

/**
 * 将Wizard传入的Sources添加到工程
 */
void ProjectManager::addSourcesInProject(const QStringList &src, const int &mode)
{
    if (_project == nullptr) {
        return;
    }
    QString path = _project->getParameter(Project::Path);
    qDebug() << path;

    QString targetPath;
    //! 0: Design Sources
    //! 1: Constraints
    switch (mode) {
    case 0:
        targetPath = path + "/sources/";
        foreach (const QString &file, src) {
            QFile::copy(file, targetPath + QFileInfo(file).fileName());
            _project->sourceList.append(targetPath + QFileInfo(file).fileName());
        }
        break;
    case 1:
        targetPath = path + "/constraints/";
        foreach (const QString &file, src) {
            QFile::copy(file, targetPath + QFileInfo(file).fileName());
            _project->constraintList.append(targetPath + QFileInfo(file).fileName());
        }
        break;
    default:
        break;
    }

    //! 添加文件监控
    TaskManager::instance().addWatchFiles(src);
}

/**
 * 移除工程中的文件
 * @param path 目标文件路径
 * @param erase false:仅移除 true:彻底删除文件
 * @return
 */
bool ProjectManager::removeFileAction(const QString &path, const bool &erase)
{
    QFileInfo fileInfo(path);
    QString folderName = fileInfo.dir().dirName();
    // qDebug() << folderName;
    if (folderName == "sources") {
        _project->sourceList.removeOne(path);
    } else if (folderName == "constraints") {
        _project->constraintList.removeOne(path);
    }

    if (erase) {
        QFile::remove(path);
        TaskManager::instance().removeWatchFile(path);
    }

    _project->writeProject();
    loadFiles(_project);
    return true;
}

void ProjectManager::setTopModule(const QString &topModule)
{
    if (_project) {
        _project->setTopModule(topModule);
        loadFiles(_project);
    }
}

void ProjectManager::setDevicePart(const QString &deviceInfo)
{
    if (_project) {
        _project->setDevicePart(deviceInfo);
        loadFiles(_project);
    }
}

QString ProjectManager::getParameter(const Project::ParamKey key) const
{
    if (!_project) {
        return QString();
    }
    return _project->getParameter(key);
}

QStringList ProjectManager::getDesignSourcesList() const
{
    if (!_project) {
        return QStringList();
    }
    return _project->sourceList;
}

QStringList ProjectManager::getConstraintsList() const
{
    if (!_project) {
        return QStringList();
    }
    return _project->constraintList;
}

void ProjectManager::closeProject()
{
    // 关闭项目提示
    // CustomMessageBox::StandardButton btn = CustomMessageBox::showTwoOptionQuestion(MainWindow::instance(), "Question", "Do you want to close the project?",
    //                                                                                QMessageBox::Yes, QMessageBox::No);
    // if (btn == QMessageBox::No) {
    //     return;
    // }

    // 检查是否能关闭所有Editor
    if (!MainWindow::instance()->cleanEditorTab()) {
        return;
    }

    // 删除工程对象
    delete _project;
    _project = nullptr;

    // 清除文件树
    FileManager::instance()->cleanFileItems();
    // 清除任务管理器参数
    TaskManager::instance().cleanParams();
    // 还原主窗口Title
    MainWindow::instance()->showProjectTitle(1);
    // 显示起始页
    MainWindow::instance()->setForm(1);
    // 重新初始化DesignRuns表
    InfoWidget::instance()->initDesignRunsView();
    // 更新最近使用工程列表
    Form::instance()->updateRecent();
}

ProjectManager::ProjectManager()
{

}

ProjectManager::~ProjectManager()
{
    if (_project) {
        delete _project;
    }
}
