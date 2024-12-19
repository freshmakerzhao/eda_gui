/**
  ******************************************************************************
  * @file           : LogPipeContent.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#ifndef HYBRDLINK_LOGPIPECONTENT_H
#define HYBRDLINK_LOGPIPECONTENT_H
#include <QString>
#include <QJsonObject>

// 日志级别枚举
enum class StatusCode {
    INFO_LOG,
    WARNING_LOG,
    CRITICAL_WARNING_LOG,
    ERROR_LOG,
    SUCCESS,
    BAD_REQUEST,
    UNAUTHORIZED,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR
};

class LogPipeContent {
private:
    QString m_type;      // 数据类型
    StatusCode m_level = StatusCode::INFO_LOG;    // 日志级别
    QString m_message;   // 日志内容
    QString m_phase;     // 阶段信息
    QString m_subPhase;  // 子阶段信息

public:
    // 构造函数
    LogPipeContent() = default;
    LogPipeContent(const QString &type, int level, const QString &message, const QString &phase, const QString &subPhase);

    // Getter 和 Setter 方法
    QString getType() const;
    void setType(const QString &type);

    StatusCode getLevel() const;
    void setLevel(StatusCode level);

    QString getMessage() const;
    void setMessage(const QString &message);

    QString getPhase() const;
    void setPhase(const QString &phase);

    QString getSubPhase() const;
    void setSubPhase(const QString &subPhase);

    // 转换为 JSON 对象
    QJsonObject toJson() const;

    // 从 JSON 对象创建实体
    static LogPipeContent fromJson(const QJsonObject &obj);
};
#endif //HYBRDLINK_LOGPIPECONTENT_H
