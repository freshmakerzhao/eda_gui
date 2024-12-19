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
    QString m_type;      // 管道信息类型
    StatusCode m_status = StatusCode::SUCCESS;    // 结束状态
    QJsonValue m_data;   // 实际数据
    QString m_phase;     // 阶段信息
    QString m_subPhase;  // 子阶段信息

public:
    // 构造函数
    DataPipeContent() = default;
    DataPipeContent(QString type, int status, QJsonValue data, QString phase, const QString &subPhase);

    // Getter 和 Setter 方法
    QString getType() const;
    void setType(const QString &type);

    StatusCode getStatus() const;
    void setStatus(StatusCode status_code);

    QJsonValue getData() const;
    void setData(QJsonValue data);

    QString getPhase() const;
    void setPhase(const QString &phase);

    QString getSubPhase() const;
    void setSubPhase(const QString &subPhase);

    // 转换为 JSON 对象
    QJsonObject toJson() const;

    // 从 JSON 对象创建实体
    static DataPipeContent fromJson(const QJsonObject &obj);
};


#endif //HYBRDLINK_DATAPIPECONTENT_H
