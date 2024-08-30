#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QTreeWidget>
#include <QDebug>
#include <QFileSystemWatcher>
#include "utils/ProjectManager.h"
#include "settings/SettingsDialog.h"
#include "dialog/CustomMessageBox.h"

class TaskManager : public QObject
{
    Q_OBJECT
public:
    static TaskManager& instance();

    void handleTreeItemActivation(const int &mode);

    /**
     * 设置工程参数
     */
    void setParams(const QMap<Project::ParamKey, QString> &params);

    /**
     * 任务分配器
     * @param mode 待执行任务
     */
    void taskController(const int mode);

    // 两个选项的弹窗，true 左侧，false 右侧
    bool twoOptionMsg(
            const QString &title,
            const QString &text,
            QMessageBox::StandardButton buttonLeft,
            QMessageBox::StandardButton buttonRight
    );

    /**
     * 关闭工程清除参数
    */
    void cleanParams();

    //! 设置需要监控的文件列表
    void setWatchFiles();
    //! 添加需要监控的文件
    void addWatchFiles(const QStringList &filePath);
    //! 移除受监控的文件列表
    void removeWatchFiles();
    //! 移除受监控的文件
    void removeWatchFile(const QString &filePath);

//    QStringList sourceList;
    // 存储设计与约束文件
    QList<QString> sourcePathList;
    QList<QString> constraintPathList;

private:
    // 项目路径
    QString projectPath;
    // 综合路径
    QString projectSynthPath;
    // implement路径
    QString projectImplPath;
    // part name
    QString partName;
    // display Part Name （临时解决）
    QString displayPartName;
    // arch name xc7a50t
    QString archName;
    // arch 50t
    QString arch;
    // arch name
    QString GLOBAL_RESOURCE_PATH;
    // top name
    QString topName = "top";
    // netlist type
    QString netlistType = ".eblif";
    // pack result type
    QString packResultType = ".net";
    // place result type
    QString placeResultType = ".place";
    // route result type
    QString routeResultType = ".route";
    // family name xc7
    QString familyName = "xc7";

private:
    TaskManager();
    ~TaskManager();

    QTreeWidget *taskTree;

    QStringList buildSynthScript();
    std::string buildImpScript();
    void buildPack();
    void buildPlace(int mode);
    void buildRoute();
    void buildBit(int mode);

    // SettingsDialog *settingDialog = nullptr;

    QFileSystemWatcher *fileWatcher;

    bool fileChanged = false;
    //! 设计文件被修改的flag
    void onFileChanged();

public:
    void downloadBit(const QString &projectImplPath1, const QString &topName1);
    void downloadFlash(const QString &projectImplPath1, const QString &topName1);
    QWidget* gridView = nullptr;

    void readBackMemory(const QString &rbdFilePath);
    void readBackRegister(const QString &registerAddress);
};

#endif // TASKMANAGER_H
