/**
  ******************************************************************************
  * @file           : PipeServer.h
  * @author         : zs
  * @description    : Header file for PipeServer
  * @attention      : None
  * @date           : 2024/12/12
  ******************************************************************************
  */

#ifndef PIPESERVER_H
#define PIPESERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QJsonObject>
#include <QByteArray>

class PipeServer : public QObject {
Q_OBJECT
public:
    explicit PipeServer(QObject *parent = nullptr);  // 构造函数
    ~PipeServer();                                   // 析构函数

    /**
     * @brief 启动命名管道服务
     * @param logPipeName 日志管道名
     * @param dataPipeName 数据管道名
     * @param controlPipeName 控制管道名
     * @return 启动是否成功
     */
    bool start(const QString &logPipeName, const QString &dataPipeName, const QString &controlPipeName);

    /**
     * @brief 初始化并启动管道服务
     */
    bool initialize();

    /**
     * @brief 停止管道监听服务
     */
    void stop();

    /**
     * @brief 清空管道数据
     */
    void clearPipes();

signals:
    /**
     * @brief 日志消息信号
     * @param level 日志级别
     * @param message 日志内容
     */
    void logMessageReceived(const QString &level, const QString &message);

    /**
     * @brief 数据消息信号
     * @param data 收到的数据内容
     */
    void dataMessageReceived(const QJsonValue &data);

    /**
     * @brief 控制命令信号
     * @param command 控制命令内容
     */
    void controlMessageReceived(const QString &command);

private:
    // QLocalServer 实例，用于处理不同类型的管道
    QLocalServer logServer;       // 日志管道
    QLocalServer dataServer;      // 数据管道
    QLocalServer controlServer;   // 控制管道

    /**
     * @brief 设置管道服务器
     * @param server QLocalServer 对象
     * @param pipeName 管道名称
     * @param signal 连接到的信号函数
     */
    template <typename SignalHandler>
    void setupServer(QLocalServer &server, const QString &pipeName, SignalHandler handler);


    /**
     * @brief 处理新连接
     * @param server QLocalServer 对象
     * @param signal 连接到的信号函数
     */
    void handleNewConnection(QLocalServer *server, void (PipeServer::*signal)(const QByteArray &));

    /**
     * @brief 处理 JSON 数据包
     * @param jsonObj 收到的 JSON 对象
     */
    void processJsonPacket(const QJsonObject &jsonObj);
    bool parseJsonData(const QByteArray &data, QJsonObject &jsonObj);
};

#endif // PIPESERVER_H
