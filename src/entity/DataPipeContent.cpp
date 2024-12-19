/**
  ******************************************************************************
  * @file           : DataPipeContent.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#include "DataPipeContent.h"

#include <utility>

// 构造函数
DataPipeContent::DataPipeContent(QString type, int status, QJsonValue data, QString phase, const QString &subPhase)
        : m_type(std::move(type)), m_data(std::move(data)), m_phase(std::move(phase)), m_subPhase(subPhase) {
    if (status == 200) {
        m_status = StatusCode::SUCCESS;
    } else if (status == 400) {
        m_status = StatusCode::BAD_REQUEST;
    } else if (status == 401) {
        m_status = StatusCode::UNAUTHORIZED;
    } else if (status == 404) {
        m_status = StatusCode::NOT_FOUND;
    } else if (status == 500) {
        m_status = StatusCode::INTERNAL_SERVER_ERROR;
    } else {
        m_status = StatusCode::SUCCESS;
    }
}

// Getter 和 Setter 实现
QString DataPipeContent::getType() const { return m_type; }
void DataPipeContent::setType(const QString &type) { m_type = type; }

StatusCode DataPipeContent::getStatus() const { return m_status; }
void DataPipeContent::setStatus(StatusCode level) { m_status = level; }

QJsonValue DataPipeContent::getData() const { return m_data; }
void DataPipeContent::setData(const QJsonValue data) { m_data = data; }

QString DataPipeContent::getPhase() const { return m_phase; }
void DataPipeContent::setPhase(const QString &phase) { m_phase = phase; }

QString DataPipeContent::getSubPhase() const { return m_subPhase; }
void DataPipeContent::setSubPhase(const QString &subPhase) { m_subPhase = subPhase; }

// 转换为 JSON 对象
QJsonObject DataPipeContent::toJson() const {
    QJsonObject obj;
    obj["type"] = m_type;
    obj["status"] = (m_status == StatusCode::SUCCESS) ? "success" :
                    (m_status == StatusCode::BAD_REQUEST) ? "bad" :
                    (m_status == StatusCode::UNAUTHORIZED) ? "unauthorized" :
                    (m_status == StatusCode::NOT_FOUND) ? "not_found" :
                    (m_status == StatusCode::INTERNAL_SERVER_ERROR) ? "internal_error" : "unknown";
    obj["data"] = m_data;
    obj["phase"] = m_phase;
    obj["sub_phase"] = m_subPhase;
    return obj;
}

