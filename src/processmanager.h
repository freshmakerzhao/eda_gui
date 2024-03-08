#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>
#include <QDebug>

class ProcessManager
{
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
        const QString &script);

    /**
     * 执行特殊命令（包含双引号、括号等字符时）
     * @param phase 当前阶段
     * @param path 工作路径
     * @param script 执行命令
     */
    void checkCallSpecific(
        const QString &phase,
        const QString &path,
        const QStringList& arguments);

    void configWorkPath(const QString &path);


    // 进程环境变量
    QProcessEnvironment env;
    // 工程参数
    std::map<std::string, std::string> projectProperty;
    /**
     * 初始化环境变量
     */
    void initEnvironment(const QString& family,
                         const std::string& resourcePath,
                         const std::string& archName,
                         const std::string& partname,
                         QList<QString> constraintPathList,
                         const QString& topName = "top");
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
};

#endif // PROCESSMANAGER_H
