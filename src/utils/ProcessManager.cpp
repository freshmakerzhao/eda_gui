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

ProcessManager& ProcessManager::instance()
{
    static ProcessManager instance;
    return instance;
}

QProcess *ProcessManager::getProcess()
{
    return process;
}

void ProcessManager::checkCall(const QString &phase, const QString &path, const QString &script, const QString &pName) {
    this->curPhase = phase;
    this->curProjectPath = path;
    this->partName = pName;
    QStringList arguments;
    arguments << "/c" << script;
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
    // 记录开始执行的时间
    this->startTime = TimeUtilities::getCurTimeAndFormat(); // 展示
    this->startTimeForCal = TimeUtilities::getCurTime(); // 计算
    process->start("cmd.exe", arguments);
}

void ProcessManager::checkCallSpecific(const QString &phase, const QString &path, const QStringList& arguments , const QString &pName) {
    this->curPhase = phase;
    this->curProjectPath = path;
    this->partName = pName;
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
    // 记录开始执行的时间
    this->startTime = TimeUtilities::getCurTimeAndFormat(); // 展示
    this->startTimeForCal = TimeUtilities::getCurTime(); // 计算

    process->start("cmd.exe", arguments);
}

/**
 * 设置工作路径
 * @param path 工作路径
 */
void ProcessManager::configWorkPath(const QString &path) {
    process->setWorkingDirectory(path); // 设置工作目录
}

/**
 * 初始化环境变量
 * @param family
 * @param resourcePath
 * @param archName
 * @param partName
 * @param constraintPathList
 * @param topName
 */
void ProcessManager::initEnvironment(const QString& family,
                                             const QString& resourcePath,
                                             const QString& archName,
                                             const QString& partName,
                                             QList<QString> constraintPathList,
                                             const QString& topName) {
    initStatus();
    //synth env setting
    // 设置环境变量
    env = QProcessEnvironment::systemEnvironment();
//    env.insert("F4PGA_SHARE_DIR","");
    env.insert("FPGA_FAM", family);
    env.insert("MAKELEVEL", "1");
    QString libPathVpr = QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "vpr", "lib"}));
    QString libPathYosys = QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "yosys", "lib"}));
    QString libPathOpenFPGALoader = QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "openFPGALoader", "lib"}));
    QString envPath =  libPathVpr + ";" + libPathYosys + ";" + libPathOpenFPGALoader;
    env.insert("PATH", envPath);

    env.insert("OUT_EBLIF", topName + ".eblif");
    env.insert("OUT_FASM_EXTRA", "top_fasm_extra.fasm");
    env.insert("OUT_JSON", topName + ".json");
    env.insert("OUT_SDC", topName + ".sdc");
    env.insert("OUT_SYNTH_V", "top_synth.v");
    env.insert("PART_JSON", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "f4pga", "prjxray-db/artix7", partName.toStdString(), "part.json"})));
    // python路径
    env.insert("PYTHON3", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "common", "python", "python.exe"})));
    env.insert("SYNTH_JSON", "top_io.json");
//    env.insert("TARGET","arty_35");
    env.insert("TECHMAP_PATH", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "f4pga", "techmaps", "xc7_vpr", "techmap"})));
    // 顶层模块
    env.insert("TOP",topName);
    env.insert("USE_ROI", "FALSE");
    env.insert("UTILS_PATH", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "f4pga", "scripts"})));
    // TODO: add all xdc files
    if (!constraintPathList.isEmpty()) {
        env.insert("INPUT_XDC_FILES", constraintPathList.first());
    }

    env.insert("FIX_XC7_CARRY_PY", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/scripts/fix_xc7_carry.py"})));
    env.insert("YOSYS_SPLIT_INOUTS_PY", QString::fromStdString(StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/scripts/yosys_split_inouts.py"})));

    // vpr可执行文件
    projectProperty["vpr_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "vpr/bin/vpr.exe"});
    // openFPGALoader可执行文件
    projectProperty["openFPGALoader_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "openFPGALoader/bin/openFPGALoader.exe"});
    // 架构目录
    projectProperty["arch_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/arch",archName.toStdString()+"_test"});
    // create_ioplace路径
    projectProperty["generate_ioplace_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/scripts/create_ioplace.py"});
    // create_place_constraints路径
    projectProperty["generate_constraints_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/scripts/create_place_constraints.py"});
    // genfasm路径
    projectProperty["generate_fasm_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "vpr/bin/genfasm.exe"});
    // xcfasm路径
    projectProperty["generate_bit_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/scripts/xcfasm"});
    // prjxray-db路径
    projectProperty["prjxray_db_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "f4pga/prjxray-db"});
    //fasm2bit
    projectProperty["fasm2bit_path"] = StringUtilities::concatPath({resourcePath.toStdString(), "vpr/bin/xc7frames2bit.exe"});
    // part名称
    projectProperty["part_name"] = partName.toStdString();
    ProcessManager::instance().getProcess()->setProcessEnvironment(env);
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
    // 判断是否报错
    if (process->error() == QProcess::UnknownError) {
        // 没有错误发生，输出 output
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

    // 读取可能剩余的标准错误
    QByteArray remainingError = process->readAllStandardError();
    // 转码
    outputStr = tc->toUnicode(remainingError);
    if (!remainingError.isEmpty()) {
        LogWidget::instance()->appendLog(outputStr);
    }
    // 结束时间
    this->endTimeForCal = TimeUtilities::getCurTime();
    // 持续时间
    this->elapsedTime = TimeUtilities::calculateTimeDifference(startTimeForCal,endTimeForCal);

    // 显示信息弹窗
    // exitCode 为0表示正常执行并成功退出
    if (exitCode == 0) {
        if (this->curPhase == "Synthesis"){
            // 综合结束后，读取资源统计信息
            InfoWidget::instance()->updateSynthItem(
                    this->curProjectPath,
                    this->curPhase + " Complete!",
                    this->startTime,
                    this->elapsedTime,
                    this->partName);
            if(showSynthSuccessMsg){
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
                // 需要弹窗则弹窗
                CustomMessageBox::showSuccess(MainWindow::instance(), this->curPhase + " Completed", this->curPhase + " successfully completed.");
            }
            if(hasNextImplementProcess){
                // 接下来需要执行 Implement
                checkCall(this->nextPhase,this->nextpath,this->nextscript,this->nextpName);
            }
        } else if (this->curPhase == "Place"){
            // Place结束后，读取资源统计信息
            InfoWidget::instance()->updateImplItem(
                    this->curProjectPath,
                    this->curPhase + " Complete!",
                    this->startTime,
                    this->elapsedTime,
                    this->partName);
            // 跳转到资源展示窗口
            InfoWidget::instance()->setCurrentPage(4);
        } else if (this->curPhase == "Implementation"){
            InfoWidget::instance()->updateImplItem(
                    this->curProjectPath,
                    this->curPhase + " Complete!",
                    this->startTime,
                    this->elapsedTime,
                    this->partName);
            if (showImplementSuccessMsg){
                CustomMessageBox::showSuccess(MainWindow::instance(), this->curPhase + " Completed", this->curPhase + " successfully completed.");
                // Implementation结束后，读取资源统计信息
                // 跳转到资源展示窗口
                InfoWidget::instance()->setCurrentPage(4);
            }
        } else {
            // 生成码流结束提示，后续在此扩展
            if (showGenBitSuccessMsg){
                CustomMessageBox::showSuccess(MainWindow::instance(), this->curPhase + " Completed", this->curPhase + " successfully completed.");
            }
        }
    } else {
        CustomMessageBox::showError(MainWindow::instance(), this->curPhase + " Failed", this->curPhase + " failed.");
    }
}

/**
 * 获取指定参数
 * @param key
 * @return
 */
std::string ProcessManager::getProperty(const std::string& key){
    return projectProperty[key];
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

void ProcessManager::setSynthSuccessMsgStatus(bool status) {
    showSynthSuccessMsg = status;
}

void ProcessManager::setImplementSuccessMsgStatus(bool status) {
    showImplementSuccessMsg = status;
}
void ProcessManager::initStatus() {
    showSynthSuccessMsg = true;
    showImplementSuccessMsg = true;
    hasNextImplementProcess = false;
    nextPhase = nullptr;
    nextpath = nullptr;
    nextscript = nullptr;
    nextpName = nullptr;
}

void ProcessManager::setNextImplementProcessStatus(bool status) {
    hasNextImplementProcess = status;
}

void ProcessManager::setNextImplementProcessScript(
        const QString &phase,
        const QString &path,
        const QString &script,
        const QString &pName) {
    this->nextPhase = phase;
    this->nextpath = path;
    this->nextscript = script;
    this->nextpName = pName;
}
