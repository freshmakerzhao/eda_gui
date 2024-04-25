/**
  ******************************************************************************
  * @file           : ProjectManager.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/7
  ******************************************************************************
  */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include "Project.h"
#include "TaskManager.h"

class ProjectManager :  public QObject
{
    Q_OBJECT
public:
    static ProjectManager& instance();

    bool startProcess(Project *project);

    /**
     * 传递文件作为命令行参数，加载工程
     * @param 命令行参数列表
     * @return
     */
    bool loadProject(QStringList args);

    /**
     * 加载工程
     * @param 工程实例
     * @return
     */
    void loadFiles(Project *project);

    void addSourcesAction();

    bool removeFileAction(const QString &path);

public slots:
    void closeProject();

private:
    ProjectManager();

    Project *_project = nullptr;
};

#endif // PROJECTMANAGER_H
