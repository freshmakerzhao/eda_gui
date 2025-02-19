#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>
#include <QDebug>
#include "service/PipeServer.h"
#include "service/LogManager.h"
#include "utils/MemoryUtilities.h"

struct ProcessMessage {
    QString phase;
    int exitCode;
    QString startTime;
    QString elapsedTime;
    QString displayPartName;
    QString workPath;
    bool isCancel;
};

class ProcessManager : public QObject {
Q_OBJECT
public:
    static ProcessManager& instance();
    QProcess *getProcess();

    void configWorkPath(const QString &path);
    void configDisplay(const QString &partname);

    // 进程环境变量
    QProcessEnvironment env;
    // 工程参数
    QMap<QString, QString> projectProperty;
    // 当前执行的阶段
    QString curPhase;
    // 当前执行路径
    QString curProjectPath;
    // 开始执行的时间（用于显示）
    QString startTime;
    // 上个子阶段开始的时间 (用于计算)
    std::chrono::system_clock::time_point lastTime;
    // 开始执行的时间（用于计算）
    std::chrono::system_clock::time_point startTimeForCal;
    // 结束执行的时间（用于计算）
    std::chrono::system_clock::time_point  endTimeForCal;
    // 执行总用时（用于显示）
    QString elapsedTime;
    // partName
    QString partName;
    // displayPart
    QString displayPartName;

    // 下一阶段使用的命令
    QString nextPhase = nullptr;
    QString nextpath = nullptr;
    QStringList nextscript = QStringList();
    QString nextpName = nullptr;

    void executeCommand(const QString &phase, const QStringList& command);

    void kill();

    /**
     * 初始化环境变量
     */
    void initEnvironment();

    /**
     * 获取指定变量值
     * @param key
     * @return
     */
    std::string getProperty(const std::string& key);

    //获取时间间隔
    QString getElapsedTime();

private:
    ProcessManager();
    ~ProcessManager();
    QProcess* process;
    PipeServer &pipeServer; // 成员变量引用
    LogManager &logManager; // 成员变量引用
    void initializePipeServer();            // 初始化管道服务

    bool isCancel = false;

private slots:
    void handleFinished(int exitCode,QProcess::ExitStatus exitStatus);
    void handleChannelReadyReadOutput();

signals:

    void finishMessage(ProcessMessage &msg);
};



#endif // PROCESSMANAGER_H
