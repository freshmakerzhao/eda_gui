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

enum class StatusCode {
    SUCCESS,
    BAD_REQUEST,
    UNAUTHORIZED,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR
};

enum class LevelCode {
    INFO_LOG,
    WARNING_LOG,
    CRITICAL_WARNING_LOG,
    ERROR_LOG,
    ALWAYS_LOG,
};

class LogPipeContent {
private:
    QString m_pipe_type;      // 数据类型
    LevelCode m_level_code = LevelCode::INFO_LOG;    // 日志级别
    QString m_message_content;   // 日志内容
    QString m_category;   // 日志标识类别
    QString m_phase;     // 阶段信息
    QString m_sub_phase;  // 子阶段信息

public:
    // 构造函数
    LogPipeContent() = default;
    LogPipeContent(const QString &pipe_type, int level_code, const QString &message_content, const QString &category, const QString &phase, const QString &sub_phase);

    // Getter 和 Setter 方法
    QString getPipeType() const;
    void setPipeType(const QString &pipe_type);

    LevelCode getLevelCode() const;
    void setLevelCode(LevelCode level_code);

    QString getMessageContent() const;
    void setMessageContent(const QString &message_content);

    QString getCategory() const;
    void setCategory(const QString &category);

    QString getPhase() const;
    void setPhase(const QString &phase);

    QString getSubPhase() const;
    void setSubPhase(const QString &sub_phase);

    // 转换为 JSON 对象
    QJsonObject toJson() const;

    // 从 JSON 对象创建实体
    static LogPipeContent fromJson(const QJsonObject &obj);
};
#endif //HYBRDLINK_LOGPIPECONTENT_H
