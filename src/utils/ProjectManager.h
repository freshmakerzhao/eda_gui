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
#include "entity/Project.h"

class ProjectManager :  public QObject
{
    Q_OBJECT
public:
    static ProjectManager& instance();

    bool hasProjectOpen();

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
    bool createProject(const QString &name,
                       const QString &path,
                       const QString &part,
                       const QString &arch,
                       const QString &archName,
                       const QStringList &designSrcs,
                       const QStringList &constraints,
                       const QString &displayPart = QString("MC1P110-FC484L-1"),
                       const QString &familyName = QString("MgiCubo"));

    /**
     * Reopen project on startup
     * Open project in New window
     * @param hprPath 工程文件路径
     * @return
     */
    bool startProcess(const QString &hprPath);

    /**
     * 接收工程文件(*.hpr)路径，打开工程
     * @param hprPath 工程文件路径
     * @return
     */
    bool openProject(const QString &hprPath);

    /**
     * 接收命令行参数，判断是否有工程文件(*.hpr)路径，执行打开工程函数
     * @param args 命令行参数列表
     * @return
     */
    bool openProjectFromArgs(const QStringList &args);

    /**
     * 加载工程到TaskManager
     * @param project 工程实例
     */
    void loadFiles(Project *project);

    /**
     * 启动Wizard，添加Sources
     */
    void addSourcesAction();

    /**
     * 将Wizard传入的Sources添加到工程
     */
    void addSourcesInProject(const QStringList &src, const int &mode);

    /**
     * 移除工程中的文件
     * @param path 目标文件路径
     * @param erase false:仅移除 true:彻底删除文件
     * @return
     */
    bool removeFileAction(const QString &path, const bool &erase = false);

    void setTopModule(const QString &topModule);

    void setDevicePart(const QString &deviceInfo);

    QString getParameter(const Project::ParamKey key) const;

    QStringList getDesignSourcesList() const;

    QStringList getConstraintsList() const;

public slots:
    void closeProject();

private:
    ProjectManager();

    ~ProjectManager();

    //! 指向当前打开的工程
    Project *_project = nullptr;
};

#endif // PROJECTMANAGER_H
