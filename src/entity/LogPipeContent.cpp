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
LogPipeContent::LogPipeContent(const QString &type, int level, const QString &message, const QString &phase, const QString &subPhase)
        : m_type(type), m_message(message), m_phase(phase), m_subPhase(subPhase) {
    if (level == 1) {
        m_level = StatusCode::INFO_LOG;
    } else if (level == 2) {
        m_level = StatusCode::WARNING_LOG;
    } else if (level == 3) {
        m_level = StatusCode::CRITICAL_WARNING_LOG;
    } else if (level == 4) {
        m_level = StatusCode::ERROR_LOG;
    } else {
        m_level = StatusCode::INFO_LOG;
    }
}

// Getter 和 Setter 实现
QString LogPipeContent::getType() const { return m_type; }
void LogPipeContent::setType(const QString &type) { m_type = type; }

StatusCode LogPipeContent::getLevel() const { return m_level; }
void LogPipeContent::setLevel(StatusCode level) { m_level = level; }

QString LogPipeContent::getMessage() const { return m_message; }
void LogPipeContent::setMessage(const QString &message) { m_message = message; }

QString LogPipeContent::getPhase() const { return m_phase; }
void LogPipeContent::setPhase(const QString &phase) { m_phase = phase; }

QString LogPipeContent::getSubPhase() const { return m_subPhase; }
void LogPipeContent::setSubPhase(const QString &subPhase) { m_subPhase = subPhase; }

// 转换为 JSON 对象
QJsonObject LogPipeContent::toJson() const {
    QJsonObject obj;
    obj["type"] = m_type;
    obj["level"] = (m_level == StatusCode::INFO_LOG) ? "info" :
                   (m_level == StatusCode::WARNING_LOG) ? "warning" :
                   (m_level == StatusCode::CRITICAL_WARNING_LOG) ? "critical_warning" :
                   (m_level == StatusCode::ERROR_LOG) ? "error" : "unknown";
    obj["message"] = m_message;
    // 创建 phase_info 对象
    obj["phase"] = m_phase;
    obj["sub_phase"] = m_subPhase;
    return obj;
}

