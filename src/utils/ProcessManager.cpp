#include "ProcessManager.h"
#include <QTimer>
#include "utils/StringUtilities.h"
#include "widgets/LogWidget.h"
#include <QTextCodec>
#include <chrono>
#include "utils/TimeUtilities.h"
#include "base/Globals.h"
#include "mainwindow.h"
#include <QDebug>
#include "utils/MemoryUtilities.h"

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
    // ============= 启动标准输出流监听 ====================
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
    // ============= 启动标准输出流监听 ====================
}

// process执行结束后触发
void ProcessManager::handleFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    // if (curPhase == "Implementation") {
    //     // 延迟 5000 毫秒，但保持 UI 响应
    //     QEventLoop loop;
    //     QTimer::singleShot(5000, &loop, &QEventLoop::quit);
    //     loop.exec();
    // }
    // =================== 启动标准输出流监听 ====================
//    QTextCodec *tc = QTextCodec::codecForName("GBK");
//
//    // 读取可能剩余的标准输出
//    QByteArray remainingOutput = process->readAllStandardOutput();
//    // 转码
//    QString outputStr = tc->toUnicode(remainingOutput);
//    if (!remainingOutput.isEmpty()) {
//        LogWidget::instance()->appendLog(outputStr);
//    }
//
//    // 读取可能剩余的标准错误
//    QByteArray remainingError = process->readAllStandardError();
//    // 转码
//    outputStr = tc->toUnicode(remainingError);
//    if (!remainingError.isEmpty()) {
//        LogWidget::instance()->appendLog(outputStr);
//    }
    // =================== 启动标准输出流监听 ====================
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
    msg.isCancel = this->isCancel;

    MemoryUtilities::instance()->stopWatch();
    // 回传给taskmanager
    emit finishMessage(msg);

    // 仿真完成，发送信号，为了展示波形
    if(curPhase == "Simulation Run" && exitCode == QProcess::NormalExit ){
        emit  simulationFinish();
    }
}

ProcessManager::ProcessManager(): pipeServer(PipeServer::instance()),logManager(LogManager::instance())
{
    process = new QProcess();
    // ============== 启动管道监听 ==============
    pipeServer.start(); // 启动管道监听
    // ============== 启动管道监听 ==============

    MemoryUtilities::instance();
//    connect(process,&QProcess::channelReadyRead,this,&ProcessManager::handleChannelReadyReadOutput);
    // finished 信号，process执行完毕后触发
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(handleFinished(int,QProcess::ExitStatus)));

    // 仿真的错误输出
    connect(process, &QProcess::readyReadStandardError, [this](){
        if(curPhase == "Simulation Run"){
            QByteArray errorOutput = process->readAllStandardError();
            qDebug() << "Error output from the process:" << QString::fromUtf8(errorOutput);
        }
    });
}

ProcessManager::~ProcessManager()
{
    delete process;
}

void ProcessManager::executeCommand(const QString &phase, const QStringList &command) {
    isCancel = false;
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
    } else if(phase == "Simulation Run")  {
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
    this->lastTime = startTimeForCal;

    // 启动进程
    if (phase == "Synthesis") {
        qDebug() << command;
        process->start(projectProperty["synthesizer_path"], command);
    } else {
        process->start("cmd.exe", script);
    }

    //进程内存占用监听50ms获取一次
    MemoryUtilities::instance()->setWatchMemory(process->processId(), 50);
}

void ProcessManager::kill()
{
    isCancel = true;
#if WIN32
    qint64 pid = process->processId();
    QProcess proc;
    proc.start("taskkill", QStringList() << "/F" << "/T" << "/PID" << QString::number(pid));
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    if (proc.waitForFinished()) {
        QByteArray output = proc.readAllStandardOutput();
        QByteArray errorOutput = proc.readAllStandardError();

        QString outputStr = tc->toUnicode(output);
        QString errorOutputStr = tc->toUnicode(errorOutput);

        if (proc.exitStatus() == QProcess::NormalExit && proc.exitCode() == 0) {
            // taskkill 成功执行
            LogWidget::instance()->appendLog(curPhase, "Process terminated successfully.");
            qDebug() << outputStr;
        } else {
            // 出现错误
            LogWidget::instance()->appendLog(curPhase,QString("taskkill failed. Error output:").arg(errorOutputStr));
        }
    } else {
        // 等待超时或其他问题
        LogWidget::instance()->appendLog(curPhase, "Failed to execute taskkill.");
    }
#else
    process->kill();
#endif
}

void ProcessManager::initEnvironment() {
    env = QProcessEnvironment::systemEnvironment();
    QString system32_path =  QString("%1\\System32").arg(QString::fromLocal8Bit(qgetenv("WINDIR")));
    QString yosys_lib_path =  QString::fromStdString(StringUtilities::concatPath({GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString(), "yosys", "lib"}));;
    QString path = system32_path + ";" + yosys_lib_path;
    env.insert("PATH", path);
    env.insert("PYTHON3", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\common\python\python.exe)");
    env.insert("FASM2FRAMES", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\fasm2frames.exe)");
    env.insert("FRAMES2BIT", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\xc7frames2bit.exe)");
    env.insert("IMPL_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\implementation\bin\implementation.exe)");
    env.insert("BITSTREAMTOOL_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\bin\bitstreamTools.exe)");
    env.insert("PARSE_BITSTREAM_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\parse_bitstream.exe)");
    projectProperty["synthesizer_path"] = GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\synthesizer\bin\synthesizer.exe)";
    env.insert("SIMULATION_COMPILER_PATH", GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\simulator\bin\iverilog.exe)" );
    env.insert("SIMULATION_RUN_PATH",GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\simulator\bin\vvp.exe)");
    env.insert("PARSER_VCD_PATH", QCoreApplication::applicationDirPath() + R"(\parserVCD.exe)");
}

void ProcessManager::configDisplay(const QString &partname) {
    this->displayPartName = partname;
}

QString ProcessManager::getElapsedTime() {
    std::chrono::system_clock::time_point curTime = TimeUtilities::getCurTime();
    QString elapsed =  TimeUtilities::calculateTimeDifference(lastTime, curTime);
    this->lastTime = curTime;
    return elapsed;
}
