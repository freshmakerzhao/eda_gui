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
#include "component/properties/Properties.h"


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

void LogManager::addLog(const LogPipeContent& oneLog) {

    if (oneLog.getMessageContent().isEmpty() || oneLog.getPhase().isEmpty() || oneLog.getSubPhase().isEmpty()) {
        qWarning() << "Invalid log data. Skipping entry.";
        return;
    }

    QString message = oneLog.getMessageContent();
    message = message.trimmed();

   //输出上一个子阶段所占用的cpu和内存资源
    static QString lastSubPhase = oneLog.getSubPhase();
    static QString lastPhase = oneLog.getPhase();
    QString curSubPhase = oneLog.getSubPhase();
    QString curPhase = oneLog.getPhase();
    if(curSubPhase != lastSubPhase && curPhase == lastPhase) {
        if(firstSubPhaseStatus) {
            firstSubPhaseStatus = false;
        } else {
            LogWidget::instance()->appendLog(
                    lastPhase,
                    "Finished " + lastSubPhase +
                    ": Time (s): elapsed = " + ProcessManager::instance().getElapsedTime() +
                    " Memory (MB): peak = " + QString::number(ProcessManager::instance().getPeak(), 'f', 2) +
                    " gain = " + QString::number(ProcessManager::instance().getGain(), 'f', 2)
            );
            LogWidget::instance()->appendLog(lastPhase, QString("-").repeated(100));
        }
    }
    lastSubPhase = oneLog.getSubPhase();
    lastPhase = oneLog.getPhase();

    // 同步到logwidget
        LogWidget::instance()->appendLog(oneLog);

    if (oneLog.getLevelCode() == LevelCode::ALWAYS_LOG) {
        // always_log 不加入message
        return;
    }

    // ==================== 存储日志 =======================
    // 特殊阶段（synth、program）处理
    if (oneLog.getPhase() == "SYNTHESIS" || oneLog.getPhase() == "PROGRAM_AND_DEBUG") {
        QMap<QString, QStringList> &phaseMessages = log_storage[oneLog.getPhase()];
        QStringList &messages = phaseMessages[""]; // 特殊阶段不区分 sub_phase
        messages.append(message);
        return;
    }

    // 其他阶段处理
    QMap<QString, QMap<QString, QStringList>> &logStorage = log_storage;
    QMap<QString, QStringList> &phaseMessages = logStorage[oneLog.getPhase()];
    QStringList &subPhaseMessages = phaseMessages[oneLog.getSubPhase()];
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
    if (one_data.getPhase() == "COMMON") {
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
    } else if (one_data.getPhase() == "SIMULATION") {
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
    } else if (one_data.getPhase() == "SYNTHESIS") {
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
    } else if (one_data.getPhase() == "IMPLEMENTATION") {
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
    } else if (one_data.getPhase() == "PROGRAM_AND_DEBUG") {
        // 处理返回的data
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
        Properties::instance()->updateHardwareProperties(one_data.getData().toObject());
    } else {
        MessageWidget::instance()->populateTreeFromLogStorage(log_storage);
    }
}
