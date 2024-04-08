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
        Navigator::instance()->loadFile(new_project);
        // ProjectManager::instance().loadFiles(new_project);
        return true;
    }
    return false;
}

/**
  * 加载工程
  * @param 工程实例
  * @return
  */
// void ProjectManager::loadFiles(Project *project)
// {
//     // 加载的不是同一个工程
//     if (pro != nullptr && pro != project) {
//         // 运行新进程，在新进程加载工程
//         ProjectManager::instance().startProcess(project);
//         return;
//     }

//     pro = project;

//     qDebug() << "[PROJECTMANAGER] loadFiles...";

//     // 存储设计文件与约束文件
//     TaskManager::instance().sourcePathList = pro->sourceList;
//     TaskManager::instance().constraintPathList = pro->constraintList;
//     // 设置工程参数
//     TaskManager::instance().setParams(pro->getAllParams());

//     SourcesView::instance()->loadFileTree(pro);
// }

ProjectManager::ProjectManager() {}
