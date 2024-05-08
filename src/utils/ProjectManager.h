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
     * 接收工程文件(*.hpr)路径，打开工程
     * @param 工程文件绝对路径
     * @return
     */
    bool openProject(const QString &path);

    /**
     * 接收命令行参数，判断是否有工程文件(*.hpr)路径，执行打开工程函数
     * @param 命令行参数列表
     * @return
     */
    bool openProjectFromArgs(const QStringList &args);

    /**
     * 加载工程
     * @param 工程实例
     * @return
     */
    void loadFiles(Project *project);

    /**
     * 启动Wizard，添加Sources
     * @return
     */
    void addSourcesAction();

    /**
     * 移除工程中的文件
     * @param 目标文件路径
     * @return
     */
    bool removeFileAction(const QString &path);

public slots:
    void closeProject();

private:
    ProjectManager();

    Project *_project = nullptr;
};

#endif // PROJECTMANAGER_H
