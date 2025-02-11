/**
  ******************************************************************************
  * @file           : LogManager.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#include "LogManager.h"

#include <utility>
#include "PipeServer.h"
#include "widgets/LogWidget.h"
#include "widgets/MessageWidget.h"
#include "utils/ProcessManager.h"


LogManager& LogManager::instance()
{
    static LogManager _instance;
    return _instance;
}

LogManager::LogManager()
{
    // 假设 PipeServer 是单例
    PipeServer &pipeServer = PipeServer::instance();
    connect(&pipeServer, &PipeServer::logArrived, this, &LogManager::handleLogArrived);
    connect(&pipeServer, &PipeServer::dataArrived, this, &LogManager::handleDataArrived);
}

LogManager::~LogManager() = default;

void LogManager::addLog(const LogPipeContent& one_log) {

    if (one_log.getMessageContent().isEmpty() || one_log.getPhase().isEmpty() || one_log.getSubPhase().isEmpty()) {
        qWarning() << "Invalid log data. Skipping entry.";
        return;
    }

    QString message = one_log.getMessageContent();
    message = message.trimmed();

   //输出上一个子阶段所占用的cpu和内存资源
    static QString lastSubPhase = one_log.getSubPhase();
    static QString lastPhase = one_log.getPhase();
    QString curSubPhase = one_log.getSubPhase();
    QString curPhase = one_log.getPhase();
    if(
        curSubPhase != lastSubPhase && curPhase == lastPhase &&
        !( curSubPhase == "pack"  && lastSubPhase == "route")
    ) {
        LogWidget::instance()->appendLog(
                lastPhase,
                "Finished " + lastSubPhase +
                ": Time (s): elapsed = " + ProcessManager::instance().getElapsedTime() +
                " Memory (MB): peak = " + QString::number(ProcessManager::instance().getPeak(), 'f', 2) +
                " gain = " + QString::number(ProcessManager::instance().getGain(), 'f', 2)
        );
        LogWidget::instance()->appendLog(lastPhase, QString("-").repeated(100));
    }
    lastSubPhase = one_log.getSubPhase();
    lastPhase = one_log.getPhase();

    // 同步到logwidget
        LogWidget::instance()->appendLog(one_log);

    if (one_log.getLevelCode() == LevelCode::ALWAYS_LOG) {
        // always_log 不加入message
        return;
    }

    // ==================== 存储日志 =======================
    // 特殊阶段（synth、program）处理
    if (one_log.getPhase() == "SYNTHESIS" || one_log.getPhase() == "PROGRAM_AND_DEBUG") {
        QMap<QString, QStringList> &phaseMessages = log_storage[one_log.getPhase()];
        QStringList &messages = phaseMessages[""]; // 特殊阶段不区分 sub_phase
        messages.append(message);
        return;
    }

    // 其他阶段处理
    QMap<QString, QMap<QString, QStringList>> &logStorage = log_storage;
    QMap<QString, QStringList> &phaseMessages = logStorage[one_log.getPhase()];
    QStringList &subPhaseMessages = phaseMessages[one_log.getSubPhase()];
    subPhaseMessages.append(message);
    // ==================== 存储日志 =======================
}

//const std::vector<QString>& LogManager::getLogs(LogLevel level) const {
//    static const std::vector<QString> emptyLogs;
//    auto it = log_storage.find(level);
//    return (it != log_storage.end()) ? it->second : emptyLogs;
//}

void LogManager::clearLogs() {
    log_storage.clear();
    MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
}

void LogManager::handleLogArrived(const LogPipeContent& one_log) {
    addLog(one_log);
}

void LogManager::handleDataArrived(const DataPipeContent& one_data) {
    MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
}
