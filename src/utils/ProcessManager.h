#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>
#include <QDebug>

class ProcessManager
        : public QObject {
Q_OBJECT
public:
    static ProcessManager& instance();
    QProcess *getProcess();

    /**
     * 执行命令
     * @param phase 当前阶段
     * @param path 工作路径
     * @param script 执行命令
     */
    void checkCall(
        const QString &phase,
        const QString &path,
        const QString &script,
        const QString &displayPartName);

    /**
     * 执行特殊命令（包含双引号、括号等字符时）
     * @param phase 当前阶段
     * @param path 工作路径
     * @param script 执行命令
     */
    void checkCallSpecific(
        const QString &phase,
        const QString &path,
        const QStringList& arguments,
        const QString &displayPartName);

    void configWorkPath(const QString &path);

    // 进程环境变量
    QProcessEnvironment env;
    // 工程参数
    std::map<std::string, std::string> projectProperty;
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
    QString nextscript = nullptr;
    QString nextpName = nullptr;

    /**
     * 初始化环境变量
     */
    void initEnvironment(const QString& family,
                         const QString& resourcePath,
                         const QString& archName,
                         const QString& partname,
                         QList<QString> constraintPathList,
                         const QString& topName = "top");
    /**
     * 设置弹窗显示状态，true则显示，false不显示
     * @param status
     */
    void setSynthSuccessMsgStatus(bool status);
    void setImplementSuccessMsgStatus(bool status);
    void setNextImplementProcessStatus(bool status);
    void setNextImplementProcessScript(const QString &phase, const QString &path, const QString &script, const QString &pName);
    void initStatus();
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
    // 是否显示综合成功弹窗
    bool showSynthSuccessMsg = true;
    // 是否显示布局布线成功弹窗
    bool showImplementSuccessMsg = true;
    // 是否显示生成码流成功弹窗
    bool showGenBitSuccessMsg = true;
    // 综合之后是否要执行implement
    bool hasNextImplementProcess = false;

private slots:
    void handleFinished(int exitCode,QProcess::ExitStatus exitStatus);
    void handleReadyReadStandardOutput();

};

#endif // PROCESSMANAGER_H
