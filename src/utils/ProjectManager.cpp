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
 * @param 工程文件绝对路径
 * @return
 */
bool ProjectManager::openProject(const QString &path)
{
    std::vector<XmlRecent> recentLists = {
            {0, path.toStdString()}
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
    if (!new_open_project->parseProject(path)) {
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
 * @param 命令行参数列表
 * @return
 */
bool ProjectManager::openProjectFromArgs(const QStringList &args)
{
    if(args.size() <= 1) {
        return false;
    }
    // 获取第一个文件路径
    QString hprfile = args.at(1);
    qDebug() << "Loading Project:" << hprfile;
    if (!openProject(hprfile)) {
        return false;
    }
    return true;
}

/**
 * 加载工程
 * @param 工程实例
 * @return
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
 * @return
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
    _project->makeProject();
    loadFiles(_project);
}

/**
 * 移除工程中的文件
 * @param 目标文件路径
 * @return
 */
bool ProjectManager::removeFileAction(const QString &path)
{
    QFileInfo fileInfo(path);
    QString folderName = fileInfo.dir().dirName();
    qDebug() << folderName;
    if (folderName == "sources") {
        _project->sourceList.removeOne(path);
    } else if (folderName == "constraints") {
        _project->constraintList.removeOne(path);
    }
    _project->makeProject();
    loadFiles(_project);
    return true;
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
