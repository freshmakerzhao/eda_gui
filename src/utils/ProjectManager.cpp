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
 * 传递文件作为命令行参数，加载工程
 * @param 命令行参数列表
 * @return
 */
bool ProjectManager::loadProject(QStringList args)
{
    // TODO 工程加载失败处理
    if(args.size() > 1) {
        // 获取第一个文件路径
        QString hprfile = args.at(1);
        qDebug() << "Loading Project:" << hprfile;
        Project *new_project = new Project;
        new_project->parseProject(hprfile);
        ProjectManager::instance().loadFiles(new_project);
        return true;
    }
    return false;
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

    qDebug() << "[PROJECTMANAGER] loadFiles...";

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
    if (!MainWindow::instance()->cleanEditorTab()) {
        return;
    }

    delete _project;
    _project = nullptr;

    // 清除文件树
    FileManager::instance()->closeProject();
    // 清除任务管理器参数
    TaskManager::instance().cleanParams();
    // 还原主窗口Title
    MainWindow::instance()->showProjectTitle(1);
    // 显示起始页
    MainWindow::instance()->setForm(1);
}

ProjectManager::ProjectManager() {}
