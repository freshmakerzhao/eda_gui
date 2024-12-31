/**
  ******************************************************************************
  * @file           : PipeServer.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/12
  ******************************************************************************
  */
#include "PipeServer.h"
#include "entity/LogPipeContent.h"

PipeServer& PipeServer::instance()
{
    static PipeServer instance;
    return instance;
}

PipeServer::PipeServer() {
    // 构建管道名称
    this->logPipeName = QString("LogPipe_%1").arg(InitialConfig::instance().pid_str);
    this->dataPipeName = QString("DataPipe_%1").arg(InitialConfig::instance().pid_str);
    this->controlPipeName = QString("ControlPipe_%1").arg(InitialConfig::instance().pid_str);
}

PipeServer::~PipeServer() {
    // 程序结束时调用stop确保清理
    stop();
}

bool PipeServer::startServer(QLocalServer &server, const QString &name) {
    if (server.isListening()) {
        // 防止重复对已经处于监听状态的 QLocalServer 进行二次监听操作
        return true;
    }

    // 如果存在同名的server文件残留，先移除
    QLocalServer::removeServer(name);

    if (!server.listen(name)) {
        qWarning() << "Failed to start server on pipe:" << name << server.errorString();
        QLocalServer::removeServer(name);
        return false;
    }

    qDebug() << "Server started listening on pipe:" << name;
    return true;
}

void PipeServer::start() {
    // 启动每个server，如果其中一个失败，可根据逻辑选择是否立即return或者做恢复
    bool logOk  = startServer(logServer, this->logPipeName);
    bool dataOk = startServer(dataServer, this->dataPipeName);
    bool controlOk   = startServer(controlServer, this->controlPipeName);

    if (!logOk || !dataOk || !controlOk) {
        qWarning() << "One or more servers failed to start.";
        // Todo 可以根据需求决定后续逻辑。这里简单return
        return;
    }

    // 分别连接不同槽函数，处理初次建立连接的事件
    connect(&logServer, &QLocalServer::newConnection, this, &PipeServer::handleLogConnection);
    connect(&dataServer, &QLocalServer::newConnection, this, &PipeServer::handleDataConnection);
    connect(&controlServer, &QLocalServer::newConnection, this, &PipeServer::handleControlConnection);
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

// 日志管道建立连接槽函数
void PipeServer::handleLogConnection() {
    QLocalSocket *clientConnection = logServer.nextPendingConnection();
    if (!clientConnection) return;

    connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection]() {
        QByteArray data = clientConnection->readAll();
        processLogPipeMessage(logServer.serverName(), data);
    });

    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
}

// 日志管道建立连接槽函数
void PipeServer::handleDataConnection() {
    QLocalSocket *clientConnection = dataServer.nextPendingConnection();
    if (!clientConnection) return;

    connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection]() {
        QByteArray data = clientConnection->readAll();
        processDataPipeMessage(dataServer.serverName(), data);
    });

    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
}

void PipeServer::handleControlConnection() {
    QLocalSocket *clientConnection = controlServer.nextPendingConnection();
    if (!clientConnection) return;

    connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection]() {
        QByteArray data = clientConnection->readAll();
        processControlPipeMessage(controlServer.serverName(), data);
    });

    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
}

QJsonObject PipeServer::parseJsonObject(const QByteArray &data) {
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
    // 无法处理时，返回空json对象
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON data:" << parseError.errorString();
        return QJsonObject();
    }

    if (!jsonDoc.isObject()) {
        qWarning() << "Received JSON is not an object.";
        return QJsonObject();
    }

    return jsonDoc.object();
}

// 解析data信息
void PipeServer::processDataPipeMessage(const QString &serverName, const QByteArray &data) {
    // qDebug() << "Received data message from client on" << serverName << ":" << data;
    QJsonObject jsonObj = parseJsonObject(data);
    if (jsonObj.isEmpty()) {
        // 解析失败，通知上层处理
        qWarning() << "Data message parsing failed. Emitting signal with error status.";
        emit dataArrived(DataPipeContent("data",400,QJsonValue(),"Common","Common"));
        return;
    }

    // 确保 "type" 字段存在且为 "data"
    QString type = jsonObj.value("pipe_type").toString();
    if (type != "data") {
        qWarning() << "Invalid message type for data pipe message:" << type;
        emit dataArrived(DataPipeContent("data",400,QJsonValue(),"Common","Common"));
        return;
    }

    // 获取 status 字段
    QJsonValue statusValue = jsonObj.value("status_code");
    if (!statusValue.isDouble()) {
        qWarning() << "Missing or invalid 'status' field in data message.";
        emit dataArrived(DataPipeContent("data",400,QJsonValue(),"Common","Common"));
        return;
    }

    // 提取 phase 字段
    QString phase = jsonObj.value("phase").toString();
    if (phase.isEmpty()) {
        qWarning() << "Missing or invalid 'phase' field in phase_info.";
        emit dataArrived(DataPipeContent("data",400,QJsonValue(),"Common","Common"));
        return;
    }

    // 提取 sub_phase 字段
    QString subPhase = jsonObj.value("sub_phase").toString();
    if (subPhase.isEmpty()) {
        qWarning() << "Missing or invalid 'sub_phase' field in phase_info.";
        emit dataArrived(DataPipeContent("data",400,QJsonValue(),"Common","Common"));
        return;
    }

    int status = statusValue.toInt();
    QJsonValue dataValue = jsonObj.value("data");

    // 根据 status 进行处理
    if (status == 200) {
        // qDebug() << "Data Received successfully. Content:" << dataValue;
    } else {
        qWarning() << "Data message returned status:" << status;
    }
    // 发射信号将data传给外部使用者,即使是非200状态，也发射信号，让上层决定处理逻辑
    emit dataArrived(DataPipeContent("data",status,dataValue,phase,subPhase));
}

// 解析logo信息
void PipeServer::processLogPipeMessage(const QString &serverName, const QByteArray &data) {
    qDebug() << "Received log message from client on" << serverName << ":" << data;
    QJsonObject jsonObj = parseJsonObject(data);
    if (jsonObj.isEmpty()) return; // 解析失败直接退出

    QString pipe_type = jsonObj.value("pipe_type").toString();
    if (pipe_type != "log") {
        qWarning() << "Invalid message type for log pipe message:" << pipe_type;
    }

    // 获取 level 字段
    QJsonValue level_code = jsonObj.value("level_code");
    if (!level_code.isDouble()) {
        qWarning() << "Missing or invalid 'level' field in log message.";
    }

    // 提取 phase 字段
    QString phase = jsonObj.value("phase").toString();
    if (phase.isEmpty()) {
        qWarning() << "Missing or invalid 'phase' field in log message.";
    }

    // 提取 sub_phase 字段
    QString sub_phase = jsonObj.value("sub_phase").toString();
    if (sub_phase.isEmpty()) {
        qWarning() << "Missing or invalid 'sub_phase' field in log message.";
    }

    // 提取  字段
    QString category = jsonObj.value("category").toString();
    if (category.isEmpty()) {
        qWarning() << "Missing or invalid 'category' field in log message.";
    }

    QString message_content = jsonObj.value("message_content").toString();
    if (message_content.isEmpty()) {
        qWarning() << "Missing or invalid 'message' field in log message.";
    }

    QString task_info = jsonObj.value("task_info").toString();
    if (task_info.isEmpty()) {
        qWarning() << "Missing or invalid 'task_info' field in log message.";
    }
    // 创建 LogMessage 实体
    LogPipeContent one_log(pipe_type, level_code.toInt(0), message_content, category, phase, sub_phase, task_info);

    // 通过信号将日志信息传给外部
    emit logArrived(one_log);
}

// 解析control信息
void PipeServer::processControlPipeMessage(const QString &serverName, const QByteArray &data) {
    qDebug() << "Received control message from client on" << serverName << ":" << data;
    QJsonObject jsonObj = parseJsonObject(data);
    if (jsonObj.isEmpty()) return; // 解析失败

    QString type = jsonObj.value("type").toString();
    if (type != "control") {
        qWarning() << "Invalid message type for control pipe message:" << type;
        return;
    }

    // control消息的内容应根据实际需求解析
    emit controlCommandArrived("",jsonObj);
}

PipeServer *PipeServer::getPipeServer() {
    return nullptr;
}
