/**
  ******************************************************************************
  * @file           : ProjectManager.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/7
  ******************************************************************************
  */

#include "ProjectManager.h"
#include "widgets/FileManager.h"
#include "wizard/Wizard.h"
#include "mainwindow.h"
#include "dialog/CustomMessageBox.h"
#include "XmlUtilities.h"
#include "base/InitialConfig.h"

ProjectManager &ProjectManager::instance()
{
    static ProjectManager instance;
    return instance;
}

/**
 * 创建一个新的HybrdLink工程
 * @param name
 * @param path
 * @param part
 * @param arch
 * @param archName
 * @return
 */
bool ProjectManager::createProject(QString &name,
                                   QString &path,
                                   QString &part,
                                   QString &arch,
                                   QString &archName,
                                   QStringList &designSrcs,
                                   QStringList &constraints)
{
    // 此处的project是一个临时对象，生成工程文件后销毁
    Project *project = new Project;
    project->initProject(name,
                         path,
                         part,
                         arch,
                         archName);
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
 * ! Reopen project on startup
 * ! Open project in New window
 * @param project 工程实例
 * @return
 */
bool ProjectManager::startProcess(Project *project)
{
    // 获取程序路径
    QString programPath = QCoreApplication::applicationFilePath();
    // 创建一个新进程
    QProcess *process = new QProcess();
    // 传入新打开工程路径
    QString hprfile = project->getParam("path") +  "/" + project->getParam("name") + ".hpr";
    qDebug() << hprfile;
    // 启动程序本身
    process->start(programPath, QStringList() << hprfile);

    return true;
}

/**
 * 接收工程文件(*.hpr)路径，打开工程
 * @param hprPath 工程文件路径
 * @return
 */
bool ProjectManager::openProject(const QString &hprPath)
{
    std::vector<XmlRecent> recentLists = {
        {0, hprPath.toStdString()}
    };
    try {
        XmlUtilities::instance().insertHybrdLinkXmlRecent(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS",
            recentLists
            );
    } catch (const std::exception& e) {
        // 异常
        // 不让IO操作影响主进程
        qDebug() << "[ProjectManager] An error occurred from openProject: " << e.what();
    }

    Project *new_open_project = new Project;
    if (!new_open_project->parseProject(hprPath)) {
        CustomMessageBox::showError(MainWindow::instance(), "Error",
                                    "Failed to open project, File parsing error.");
        delete new_open_project;
        return false;
    }
    ProjectManager::instance().loadFiles(new_open_project);
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
        ProjectManager::instance().startProcess(project);
        return;
    }

    _project = project;

    qDebug() << "[ProjectManager] loadFiles...";

    // 存储设计文件与约束文件
    TaskManager::instance().sourcePathList = _project->sourceList;
    TaskManager::instance().constraintPathList = _project->constraintList;
    // 设置工程参数
    TaskManager::instance().setParams(_project->getAllParams());
    // 加载文件树
    FileManager::instance()->updateDesignSources(_project->sourceList);
    FileManager::instance()->updateConstraints(_project->constraintList);
    // UI反馈
    MainWindow::instance()->showProjectTitle(0, _project->getParam("path") + "/" + _project->getParam("name") + ".hpr");
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
    Wizard wizard(MainWindow::instance(), 1, _project);
    wizard.exec();
    _project->writeProject();
    loadFiles(_project);
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

QString ProjectManager::getTopModule()
{
    if (_project) {
        return _project->getParam("top");
    }
    return QString();
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
}

ProjectManager::ProjectManager() {}
