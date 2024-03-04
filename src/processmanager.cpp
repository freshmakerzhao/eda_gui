#include "processmanager.h"

ProcessManager& ProcessManager::instance()
{
    static ProcessManager instance;
    return instance;
}

QProcess *ProcessManager::getProcess()
{
    return process;
}

void ProcessManager::checkCall(const QString &phase, const QString &path, const QString &script) {
    QStringList arguments;
    arguments << "/c" << script;
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
    process->start("cmd.exe", arguments);
}

void ProcessManager::checkCallSpecific(const QString &phase, const QString &path, QStringList arguments) {
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
    process->start("cmd.exe", arguments);
}

/**
 * 设置工作路径
 * @param path 工作路径
 */
void ProcessManager::configWorkPath(const QString &path) {
    process->setWorkingDirectory(path); // 设置工作目录
}

ProcessManager::ProcessManager()
{
    process = new QProcess();
}

ProcessManager::~ProcessManager()
{
    delete process;
}
