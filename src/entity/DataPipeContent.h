/**
  ******************************************************************************
  * @file           : DataPipeContent.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#ifndef HYBRDLINK_DATAPIPECONTENT_H
#define HYBRDLINK_DATAPIPECONTENT_H


#include <QString>
#include <QJsonValue>
#include "LogPipeContent.h"

class DataPipeContent {
private:
    QString m_pipe_type;      // 管道信息类型
    StatusCode m_status_code = StatusCode::SUCCESS;    // 结束状态
    QJsonValue m_data;   // 实际数据
    QString m_phase;     // 阶段信息
    QString m_sub_phase;  // 子阶段信息

public:
    // 构造函数
    DataPipeContent() = default;
    DataPipeContent(QString pipe_type, int status_code, QJsonValue data, QString phase, const QString &sub_phase);

    // Getter 和 Setter 方法
    QString getPipeType() const;
    void setPipeType(const QString &pipe_type);

    StatusCode getStatusCode() const;
    void setStatusCode(StatusCode status_code);

    QJsonValue getData() const;
    void setData(QJsonValue data);

    QString getPhase() const;
    void setPhase(const QString &phase);

    QString getSubPhase() const;
    void setSubPhase(const QString &sub_phase);

    // 转换为 JSON 对象
    QJsonObject toJson() const;

    // 从 JSON 对象创建实体
    static DataPipeContent fromJson(const QJsonObject &obj);
};


#endif //HYBRDLINK_DATAPIPECONTENT_H
