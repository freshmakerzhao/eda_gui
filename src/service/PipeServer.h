/**
  ******************************************************************************
  * @file           : PipeServer.h
  * @author         : zs
  * @description    : Header file for PipeServer
  * @attention      : None
  * @date           : 2024/12/12
  ******************************************************************************
  */
#ifndef QTSERVICE_PIPESERVER_H
#define QTSERVICE_PIPESERVER_H

#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include <QJsonParseError>
#include <QJsonObject>
#include "base/InitialConfig.h"
#include "entity/LogPipeContent.h"
#include "entity/DataPipeContent.h"

class PipeServer : public QObject {
Q_OBJECT
public:
    static PipeServer& instance();
    PipeServer *getPipeServer();
    QString logPipeName;
    QString dataPipeName;
    QString controlPipeName;

    // 启动管道监听
    void start();

    /**
     * @brief 停止管道监听服务
     */
    void stop();

signals:
    // 在解析成功后发射这些信号，将数据传给外部模块
    void logArrived(LogPipeContent one_log);
    void dataArrived(DataPipeContent one_data);
    void controlCommandArrived(const QString &phase, const QJsonObject &command);
    // 一个阶段的结束信号
    // void processFinished(const QString &phase,const int status);

private slots:
    void handleLogConnection();
    void handleDataConnection();
    void handleControlConnection();

private:
    PipeServer();
    ~PipeServer();

    // 启动指定的QLocalServer监听
    bool startServer(QLocalServer &server, const QString &name);

    // 静态工具函数：解析JSON并返回QJsonObject，不成功则返回空对象
    static QJsonObject parseJsonObject(const QByteArray &data);

    // 处理接收到的原始数据
    void processLogPipeMessage(const QString &serverName, const QByteArray &data);
    void processDataPipeMessage(const QString &serverName, const QByteArray &data);
    void processControlPipeMessage(const QString &serverName, const QByteArray &data);

    QLocalServer logServer;
    QLocalServer dataServer;
    QLocalServer controlServer;
};

#endif //QTSERVICE_PIPESERVER_H