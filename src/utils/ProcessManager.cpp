#include "ProcessManager.h"
#include <QTimer>
#include "utils/StringUtilities.h"
#include "widgets/LogWidget.h"
#include <QTextCodec>
#include <chrono>
#include "utils/TimeUtilities.h"
#include "base/Globals.h"
#include "mainwindow.h"

ProcessManager& ProcessManager::instance()
{
    static ProcessManager instance;
    return instance;
}

QProcess *ProcessManager::getProcess()
{
    return process;
}

/**
 * 设置工作路径
 * @param path 工作路径
 */
void ProcessManager::configWorkPath(const QString &path) {
    this->curProjectPath = path;
    process->setWorkingDirectory(path); // 设置工作目录
}

// 实时回显数据
void ProcessManager::handleChannelReadyReadOutput()
{
//    // 获取标准输出
//    QByteArray normalOutput = process->readAllStandardOutput();
//    // 获取错误输出
//    QByteArray errorOutput = process->readAllStandardError();
//    QTextCodec *tc = QTextCodec::codecForName("GBK");
//    // 转码
//    QString outputStr = tc->toUnicode(normalOutput);
//    QString errorOutputStr = tc->toUnicode(errorOutput);
//
//    if (!outputStr.isEmpty()){
//        LogWidget::instance()->appendLog(outputStr);
//    }
//
//    if (!errorOutputStr.isEmpty()){
//        LogWidget::instance()->appendLog(errorOutputStr);
//    }
}

// process执行结束后触发
void ProcessManager::handleFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    if (curPhase == "Implementation") {
        // 延迟 5000 毫秒，但保持 UI 响应
        QEventLoop loop;
        QTimer::singleShot(5000, &loop, &QEventLoop::quit);
        loop.exec();
    }
//    QTextCodec *tc = QTextCodec::codecForName("GBK");
//
//    // 读取可能剩余的标准输出
//    QByteArray remainingOutput = process->readAllStandardOutput();
//    // 转码
//    QString outputStr = tc->toUnicode(remainingOutput);
//    if (!remainingOutput.isEmpty()) {
////        LogWidget::instance()->appendLog(outputStr);
//    }
//
//    // 读取可能剩余的标准错误
//    QByteArray remainingError = process->readAllStandardError();
//    // 转码
//    outputStr = tc->toUnicode(remainingError);
//    if (!remainingError.isEmpty()) {
////        LogWidget::instance()->appendLog(outputStr);
//    }

    // 结束时间
    this->endTimeForCal = TimeUtilities::getCurTime();
    // 持续时间
    this->elapsedTime = TimeUtilities::calculateTimeDifference(startTimeForCal,endTimeForCal);

    ProcessMessage msg;
    msg.phase = curPhase;
    msg.exitCode = exitCode;
    msg.startTime = startTime;
    msg.elapsedTime = elapsedTime;
    msg.displayPartName = displayPartName;
    msg.workPath = curProjectPath;
    msg.statusInfo = this->curPhase + " Complete!";
    msg.showInfoContent = this->curPhase + " successfully completed.";

    // 回传给taskmanager
    emit finishMessage(msg);
}

ProcessManager::ProcessManager(): pipeServer(PipeServer::instance()),logManager(LogManager::instance())
{
    process = new QProcess();
    pipeServer.start(); // 启动管道监听
    // readyReadStandardOutput 信号，有输出则显示在message中
    // connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(handleReadyReadStandardOutput()));
    connect(process,&QProcess::channelReadyRead,this,&ProcessManager::handleChannelReadyReadOutput);
    // finished 信号，process执行完毕后触发
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(handleFinished(int,QProcess::ExitStatus)));
}

ProcessManager::~ProcessManager()
{
    delete process;
}

void ProcessManager::executeCommand(const QString &phase, const QStringList &command) {
    this->curPhase = phase; // 当前执行阶段
    MainWindow::instance()->setRunState(QString("Run %1...").arg(curPhase), true);
    process->setProcessEnvironment(env);

    process->terminate(); // 执行前中断process

    QStringList script;
    if (phase == "Synthesis") {
    } else if (phase == "Implementation") {
        script << "/c" << command;
    } else if (phase == "Generate Bitstream") {
        script << "/c" << command;
    } else if (phase == "Download Bitstream") {
        script << "/c" << command;
    } else if (phase == "Auto Connect") {
        script << "/c" << command;
    }
    
    qDebug() << "------------------------------------------------- ";
    qDebug() << script;
    qDebug() << "------------------------------------------------- ";

    // 记录开始执行的时间
    this->startTime = TimeUtilities::getCurTimeAndFormat(); // 展示
    this->startTimeForCal = TimeUtilities::getCurTime(); // 计算

    // 启动进程
    if (phase == "Synthesis") {
        qDebug() << command;
        process->start(projectProperty["synthesizer_path"], command);
    } else {
        process->start("cmd.exe", script);
    }
}

void ProcessManager::initEnvironment() {
    env = QProcessEnvironment::systemEnvironment();

//    QString origin_path = env.value("PATH");
//    origin_path += ";" + QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH.toStdString(), "yosys", "lib"}));
//    qgetenv("WINDIR")

    QString system32_path =  QString("%1\\System32").arg(QString::fromLocal8Bit(qgetenv("WINDIR")));
    QString yosys_lib_path =  QString::fromStdString(StringUtilities::concatPath({GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString(), "yosys", "lib"}));;
    QString path = system32_path + ";" + yosys_lib_path;
    env.insert("PATH", path);
    env.insert("PYTHON3", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\common\python\python.exe)");
    env.insert("FASM2FRAMES", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\fasm2frames.exe)");
    env.insert("FRAMES2BIT", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\xc7frames2bit.exe)");
    env.insert("IMPL_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\implementation\bin\implementation.exe)");
    env.insert("BITSTREAMTOOL_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\bin\bitstreamTools.exe)");
    projectProperty["synthesizer_path"] = GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\synthesizer\bin\synthesizer.exe)";
    // projectProperty["implementation_path"] = GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\implementation\bin\implementation.exe)";
}

void ProcessManager::configDisplay(const QString &partname) {
    this->displayPartName = partname;
}