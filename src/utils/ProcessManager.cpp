#include "ProcessManager.h"
#include "utils/StringUtilities.h"
#include "widgets/LogWidget.h"
#include "widgets/InfoWidget.h"
#include "mainwindow.h"
#include <QTextCodec>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "utils/TimeUtilities.h"
#include "dialog/CustomMessageBox.h"
#include "base/Globals.h"

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
void ProcessManager::handleReadyReadStandardOutput()
{
    // 获取标准输出
    QByteArray normalOutput = process->readAllStandardOutput();
    // 获取错误输出
    QByteArray errorOutput = process->readAllStandardError();
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    // 转码
    QString outputStr = tc->toUnicode(normalOutput);
    QString errorOutputStr = tc->toUnicode(errorOutput);

    // 没有错误发生，输出 output
//    qDebug() << "========output========== ";
//    qDebug() << outputStr;
//    qDebug() << "========output========== ";

    qDebug() << "======== errorOutput ========== ";
    qDebug() << errorOutput;
    qDebug() << "======== errorOutput ========== ";

    // 判断是否报错
    if (process->error() == QProcess::UnknownError) {
//        // 没有错误发生，输出 output
//        qDebug() << "================== ";
//        qDebug() << outputStr;
//        qDebug() << "================== ";

        LogWidget::instance()->appendLog(outputStr);
    } else {
        // 发生错误，输出 errorOutput
        LogWidget::instance()->appendLog(errorOutputStr);
    }
}

// process执行结束后触发
void ProcessManager::handleFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    // 读取可能剩余的标准输出
    QByteArray remainingOutput = process->readAllStandardOutput();
    // 转码
    QString outputStr = tc->toUnicode(remainingOutput);
    if (!remainingOutput.isEmpty()) {
        LogWidget::instance()->appendLog(outputStr);
    }
    qDebug() << "1. =============================";
    qDebug() << outputStr;
    qDebug() << "1. =============================";
    // 读取可能剩余的标准错误
    QByteArray remainingError = process->readAllStandardError();
    // 转码
    outputStr = tc->toUnicode(remainingError);
    if (!remainingError.isEmpty()) {
        LogWidget::instance()->appendLog(outputStr);
    }
//    qDebug() << "2. =============================";
//    qDebug() << outputStr;
//    qDebug() << "2. =============================";

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

ProcessManager::ProcessManager()
{
    process = new QProcess();
    // readyReadStandardOutput 信号，有输出则显示在message中
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(handleReadyReadStandardOutput()));
    // finished 信号，process执行完毕后触发
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(handleFinished(int,QProcess::ExitStatus)));
}

ProcessManager::~ProcessManager()
{
    delete process;
}

void ProcessManager::excuteCommand(QString &phase, const QStringList& command) {
    initEnvironment(); // 初始化环境变量
    this->curPhase = phase; // 当前执行阶段

    process->terminate(); // 执行前中断process

    QStringList script;
    if (phase == "synthesizer"){
        script << "/c" << projectProperty["synthesizer_path"] << command;
    } else if (phase == "implementation"){
        script << "/c" << projectProperty["implementation_path"] << command;

    }
    qDebug() << " =================================== ";
    qDebug() << script;
    qDebug() << " =================================== ";

    // 记录开始执行的时间
    this->startTime = TimeUtilities::getCurTimeAndFormat(); // 展示
    this->startTimeForCal = TimeUtilities::getCurTime(); // 计算

    process->start("cmd.exe", script);
}

void ProcessManager::initEnvironment() {
    env = QProcessEnvironment::systemEnvironment();
    QString YosysLibPath = QString::fromStdString(StringUtilities::concatPath({GLOBAL_RESOURCE_PATH.toStdString(), "yosys", "lib"}));
    env.insert("PATH", YosysLibPath);
    projectProperty["synthesizer_path"] = GLOBAL_RESOURCE_PATH + R"(\yosys\bin\yosys.exe)";
    projectProperty["implementation_path"] = GLOBAL_RESOURCE_PATH + R"(\nextpnr\bin\nextpnr-xilinx.exe)";
}

void ProcessManager::configDisplay(const QString &partname) {
    this->displayPartName = partname;
}
