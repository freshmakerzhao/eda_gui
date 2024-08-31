#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>
#include <QDebug>

struct ProcessMessage {
    QString phase;
    int exitCode;
    QString startTime;
    QString elapsedTime;
    QString displayPartName;
    QString workPath;
    QString statusInfo;
    QString showInfoContent;
};

class ProcessManager
        : public QObject {
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

    void excuteCommand(QString &phase, const QStringList& command);

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

private:
    ProcessManager();
    ~ProcessManager();
    QProcess* process;

private slots:
    void handleFinished(int exitCode,QProcess::ExitStatus exitStatus);
    void handleChannelReadyReadOutput();

signals:

    void finishMessage(ProcessMessage &msg);
};



#endif // PROCESSMANAGER_H
