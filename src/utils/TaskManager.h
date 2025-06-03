/**
  ******************************************************************************
  * @file           : TaskManager.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/4
  ******************************************************************************
  */
#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QTreeWidget>
#include <QQueue>
#include <QDebug>
#include <QFileSystemWatcher>
#include "utils/ProjectManager.h"
#include "ProcessManager.h"

namespace FlowState {
    static const QString NoStarted   = QStringLiteral("NoStarted");
    static const QString NoAvailable = QStringLiteral("NoAvailable");
    static const QString OutOfDate   = QStringLiteral("OutOfDate");
    static const QString Complete    = QStringLiteral("Complete");
    static const QString Fail        = QStringLiteral("Fail");
}

namespace FlowPhase {
    static const QString Synthsis          = QStringLiteral("State/Synthsis");
    static const QString Implementation    = QStringLiteral("State/Implementation");
    static const QString GenerateBitstream = QStringLiteral("State/GenerateBitstream");
}

enum class FlowTask {
    Synthesis,
    Implementation,
    WriteBitstream
};

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

    //! Create state machine record file.
    void initStateMachine();

    void flowTaskController(const int &mode);

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

    // 存储设计，约束文件与仿真激励文件
    QList<QString> sourcePathList;
    QList<QString> constraintPathList;
    QList<QString> simPathList;

public slots:
    void handleMessage(ProcessMessage &msg);

private:
    // 项目路径
    QString projectPath;
    // 综合路径
    QString projectSynthPath;
    // implement路径
    QString projectImplPath;
    // simulation路径
    QString projectSimPath;
    // part name
    QString partName;
    // display Part Name （临时解决）
    QString displayPartName;
    // arch name xc7a50t
    QString archName;
    // arch 50t
    QString arch;
    // top name
    QString topName = "top";
    // netlist type
    QString netlistType = ".json";
    // implementaion result type
    QString implResultType = ".fasm";
    // family name xc7
    QString familyName = "xc7";

private:
    TaskManager();
    ~TaskManager();

    QString buildSynthScript();
    QString buildImpScript();
    QString buildBitScript();
    QString buildSimScript();

    QFileSystemWatcher *fileWatcher;

    bool fileChanged = false;

    //! Modifications to the sources will trigger the state machine update.
    void onFileChanged();

    QQueue<FlowTask> flowTaskQueue;

    void handleFlowTaskQueue();

    bool _isShowSuccessMessage = true;

    void handleSynthClick();
    void handleImplClick();
    void handleGenBitClick();
public:
    void downloadBit(const QString &bitstream, const QString &cable_name = "digilent_hs3");
    void downloadFlash(const QString &projectImplPath1, const QString &topName1);
    QWidget* gridView = nullptr;

    void readBackMemory(const QString &rbdFilePath);
    void readBackRegister(const QString &registerAddress);

signals:
    void timingDialogAccept();
    void powerDialogAccept();
    void ioportsDialogAccept();
};

#endif // TASKMANAGER_H
