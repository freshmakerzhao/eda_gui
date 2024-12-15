/**
  ******************************************************************************
  * @file           : PipeServer.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/12
  ******************************************************************************
  */

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLoggingCategory>
#include <QCoreApplication>
#include "PipeServer.h"

// 日志分类

Q_LOGGING_CATEGORY(pipeServerLog, "pipeServer")
Q_LOGGING_CATEGORY(pipeServerError, "pipeServer.error")
PipeServer::PipeServer(QObject *parent) : QObject(parent) {}

PipeServer::~PipeServer() {
    stop();
}

bool PipeServer::start(const QString &logPipeName, const QString &dataPipeName, const QString &controlPipeName) {
    bool logPipeStarted = setupServer(logServer, logPipeName, &PipeServer::logMessageReceived);
    bool dataPipeStarted = setupServer(dataServer, dataPipeName, &PipeServer::dataMessageReceived);
    bool controlPipeStarted = setupServer(controlServer, controlPipeName, &PipeServer::controlMessageReceived);

    if (logPipeStarted && dataPipeStarted && controlPipeStarted) {
        qDebug() << "All pipes started successfully:";
        qDebug() << "Log pipe:" << logPipeName;
        qDebug() << "Data pipe:" << dataPipeName;
        qDebug() << "Control pipe:" << controlPipeName;
        return true;
    } else {
        qCritical() << "Failed to start one or more pipes.";
        stop();  // 停止所有已启动的管道以清理资源
        return false;
    }
}


void PipeServer::clearPipes() {
    // 停止当前的管道服务
    stop();

    // 使用当前的管道名称重新启动
    if (!start(logServer.serverName(), dataServer.serverName(), controlServer.serverName())) {
        qDebug() << "Failed to restart pipes during clearing.";
    } else {
        qDebug() << "Pipes cleared and restarted successfully.";
    }
}


void PipeServer::stop() {
    // 关闭并移除 logServer
    if (logServer.isListening()) {
        QLocalServer::removeServer(logServer.serverName());
        logServer.close();
    }

    // 关闭并移除 dataServer
    if (dataServer.isListening()) {
        QLocalServer::removeServer(dataServer.serverName());
        dataServer.close();
    }

    // 关闭并移除 controlServer
    if (controlServer.isListening()) {
        QLocalServer::removeServer(controlServer.serverName());
        controlServer.close();
    }

    qDebug() << "PipeServer stopped and resources released.";
}


template <typename SignalHandler>
void PipeServer::setupServer(QLocalServer &server, const QString &pipeName, SignalHandler handler) {
    if (server.listen(pipeName)) {
        connect(&server, &QLocalServer::newConnection, this, [this, &server, handler]() {
            QLocalSocket *clientConnection = server.nextPendingConnection();
            if (!clientConnection) {
                qCWarning(pipeServerError) << "No pending connection available on pipe:" << server.serverName();
                return;
            }

            // 处理数据读取和解析
            connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection, handler]() {
                QByteArray data = clientConnection->readAll();
                if (data.isEmpty()) {
                    qCWarning(pipeServerError) << "Received empty data on pipe:" << clientConnection->serverName();
                    return;
                }

                QJsonObject jsonObj;
                if (parseJsonData(data, jsonObj)) {
                    std::invoke(handler, this, jsonObj);  // 调用处理函数
                }
            });

            // 连接断开时释放资源
            connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
        });

        qCInfo(pipeServerLog) << "Listening on pipe:" << pipeName;
    } else {
        qCCritical(pipeServerError) << "Failed to start pipe:" << pipeName << "Error:" << server.errorString();
        QLocalServer::removeServer(pipeName);
    }
}


/**
 * @brief 解析 JSON 数据
 * @param data 输入的原始数据
 * @param jsonObj 输出的 JSON 对象
 * @return 解析是否成功
 */
bool PipeServer::parseJsonData(const QByteArray &data, QJsonObject &jsonObj) {
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()) {
            jsonObj = jsonDoc.object();
            return true;
        } else {
            qCWarning(pipeServerError) << "Parsed JSON is not an object. Data:" << data;
            return false;
        }
    } else {
        qCWarning(pipeServerError) << "JSON parsing error:" << parseError.errorString() << "Data:" << data;
        return false;
    }
}

void PipeServer::handleNewConnection(QLocalServer *server, void (PipeServer::*signal)(const QByteArray &)) {
    QLocalSocket *clientConnection = server->nextPendingConnection();
    if (!clientConnection) {
        qDebug() << "No pending connection available.";
        return;
    }

    qDebug() << "New connection established on pipe:" << server->serverName();

    // 处理客户端消息
    connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection, signal]() {
        QByteArray data;
        while (clientConnection->bytesAvailable() > 0) {
            data.append(clientConnection->readAll());
        }

        if (data.isEmpty()) {
            qDebug() << "Received empty data from client.";
            return;
        }

        // 解析 JSON 数据包
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            if (jsonDoc.isObject()) {
                processJsonPacket(jsonDoc.object());
            } else {
                qDebug() << "Received JSON is not an object.";
            }
        } else {
            qDebug() << "Failed to parse JSON data:" << parseError.errorString();
        }
    });

    // 处理客户端断开连接，释放资源
    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
}

void PipeServer::processJsonPacket(const QJsonObject &jsonObj) {
    QString type = jsonObj.value("type").toString();
    if (type == "log") {
        emit logMessageReceived(jsonObj.value("level").toString(), jsonObj.value("message").toString());
    } else if (type == "data") {
        emit dataMessageReceived(jsonObj.value("data"));
    } else if (type == "control") {
        emit controlMessageReceived(jsonObj.value("command").toString());
    } else {
        qWarning() << "Unknown type in JSON packet:" << type;
    }
}

bool PipeServer::initialize() {
    // 根据父进程 ID 生成唯一管道名称
    qint64 parentPid = QCoreApplication::applicationPid();
    QString logPipeName = QString("logPipe_%1").arg(parentPid);
    QString dataPipeName = QString("dataPipe_%1").arg(parentPid);
    QString controlPipeName = QString("controlPipe_%1").arg(parentPid);

    // 启动管道服务
    if () {
        qDebug() << "PipeServer started with pipes:" << logPipeName << dataPipeName << controlPipeName;
        return true;
    }

    qCritical() << "Failed to initialize PipeServer.";
    return false;
}