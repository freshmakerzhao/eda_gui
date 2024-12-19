/**
  ******************************************************************************
  * @file           : LogManager.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/18
  ******************************************************************************
  */
#ifndef HYBRDLINK_LOGMANAGER_H
#define HYBRDLINK_LOGMANAGER_H


#include <QString>
#include <vector>
#include <map>
#include <QDebug>
#include <QObject>
#include "entity/LogPipeContent.h"
#include "entity/DataPipeContent.h"

class LogManager : public QObject {
    Q_OBJECT

public:
    static LogManager& instance();

    // 清空所有日志
    void clearLogs();
    // 获取分类日志
//    const std::vector<QString>& getLogs(LogLevel level) const;
    // 添加日志并分发到各个控件
    void addLog(const LogPipeContent& one_log);

signals:
    // 信号：通知 MessageWidget 构建日志树
    void logFinished();

private:
    LogManager();
    ~LogManager();

    Q_DISABLE_COPY(LogManager)

    // 存储分类日志，只有在用户重新执行综合时，才会清空，否则只更新或追加
    QMap<QString, QMap<QString, QStringList>> log_storage;

public slots:
    // 处理日志到达信号
    void handleLogArrived(const LogPipeContent& one_log);
    // 处理数据到达信号
     void handleDataArrived(const DataPipeContent& one_data);
};

#endif //HYBRDLINK_LOGMANAGER_H
