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
DataPipeContent::DataPipeContent(QString pipe_type, int status_code, QJsonValue data, QString phase, const QString &sub_phase)
        : m_pipe_type(std::move(pipe_type)), m_data(std::move(data)), m_phase(std::move(phase)), m_sub_phase(sub_phase) {
    if (status_code == 200) {
        m_status_code = StatusCode::SUCCESS;
    } else if (status_code == 400) {
        m_status_code = StatusCode::BAD_REQUEST;
    } else if (status_code == 401) {
        m_status_code = StatusCode::UNAUTHORIZED;
    } else if (status_code == 404) {
        m_status_code = StatusCode::NOT_FOUND;
    } else if (status_code == 500) {
        m_status_code = StatusCode::INTERNAL_SERVER_ERROR;
    } else {
        m_status_code = StatusCode::SUCCESS;
    }
}

// Getter 和 Setter 实现
QString DataPipeContent::getPipeType() const { return m_pipe_type; }
void DataPipeContent::setPipeType(const QString &pipe_type) { m_pipe_type = pipe_type; }

StatusCode DataPipeContent::getStatusCode() const { return m_status_code; }
void DataPipeContent::setStatusCode(StatusCode status_code) { m_status_code = status_code; }

QJsonValue DataPipeContent::getData() const { return m_data; }
void DataPipeContent::setData(const QJsonValue data) { m_data = data; }

QString DataPipeContent::getPhase() const { return m_phase; }
void DataPipeContent::setPhase(const QString &phase) { m_phase = phase; }

QString DataPipeContent::getSubPhase() const { return m_sub_phase; }
void DataPipeContent::setSubPhase(const QString &sub_phase) { m_sub_phase = sub_phase; }

// 转换为 JSON 对象
QJsonObject DataPipeContent::toJson() const {
    QJsonObject obj;
    obj["pipe_type"] = m_pipe_type;
    obj["status_code"] = (m_status_code == StatusCode::SUCCESS) ? "success" :
                    (m_status_code == StatusCode::BAD_REQUEST) ? "bad" :
                    (m_status_code == StatusCode::UNAUTHORIZED) ? "unauthorized" :
                    (m_status_code == StatusCode::NOT_FOUND) ? "not_found" :
                    (m_status_code == StatusCode::INTERNAL_SERVER_ERROR) ? "internal_error" : "unknown";
    obj["data"] = m_data;
    obj["phase"] = m_phase;
    obj["sub_phase"] = m_sub_phase;
    return obj;
}

