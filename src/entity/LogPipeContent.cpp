/**
  ******************************************************************************
  * @file           : LogPipeContent.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#include "LogPipeContent.h"

// 构造函数
LogPipeContent::LogPipeContent(const QString &pipe_type, int level_code, const QString &message_content, const QString &category, const QString &phase, const QString &sub_phase, const QString &task_info)
        : m_pipe_type(pipe_type), m_message_content(message_content), m_category(category), m_phase(phase), m_sub_phase(sub_phase), m_task_info(task_info) {

    if (level_code == 0){
        m_level_code = LevelCode::ALWAYS_LOG;
    } else if (level_code == 1) {
        m_level_code = LevelCode::INFO_LOG;
    } else if (level_code == 2) {
        m_level_code = LevelCode::WARNING_LOG;
    } else if (level_code == 3) {
        m_level_code = LevelCode::CRITICAL_WARNING_LOG;
    } else if (level_code == 4) {
        m_level_code = LevelCode::ERROR_LOG;
    } else {
        m_level_code = LevelCode::ALWAYS_LOG;
    }
}

// Getter 和 Setter 实现
QString LogPipeContent::getPipeType() const { return m_pipe_type; }
void LogPipeContent::setPipeType(const QString &pipe_type) { m_pipe_type = pipe_type; }

LevelCode LogPipeContent::getLevelCode() const { return m_level_code; }
void LogPipeContent::setLevelCode(LevelCode level_code) { m_level_code = level_code; }

QString LogPipeContent::getMessageContent() const { return m_message_content; }
void LogPipeContent::setMessageContent(const QString &message_content) { m_message_content = message_content; }

QString LogPipeContent::getCategory() const { return m_category; }
void LogPipeContent::setCategory(const QString &category) { m_category = category; }

QString LogPipeContent::getPhase() const { return m_phase; }
void LogPipeContent::setPhase(const QString &phase) { m_phase = phase; }

QString LogPipeContent::getSubPhase() const { return m_sub_phase; }
void LogPipeContent::setSubPhase(const QString &sub_phase) { m_sub_phase = sub_phase; }

// 转换为 JSON 对象
QJsonObject LogPipeContent::toJson() const {
    QJsonObject obj;
    obj["pipe_type"] = m_pipe_type;
    obj["level_code"] = (m_level_code == LevelCode::INFO_LOG) ? "info" :
                   (m_level_code == LevelCode::WARNING_LOG) ? "warning" :
                   (m_level_code == LevelCode::CRITICAL_WARNING_LOG) ? "critical_warning" :
                   (m_level_code == LevelCode::ERROR_LOG) ? "error" : "always";
    obj["message_content"] = m_message_content;
    obj["category"] = m_category;
    // 创建 phase_info 对象
    obj["phase"] = m_phase;
    obj["sub_phase"] = m_sub_phase;
    return obj;
}

