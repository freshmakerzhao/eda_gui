#include "processmanager.h"
#include "utils/StringUtilities.h"
#include "infowidget.h"
#include <QTextCodec>
#include <QMessageBox>

ProcessManager& ProcessManager::instance()
{
    static ProcessManager instance;
    return instance;
}

QProcess *ProcessManager::getProcess()
{
    return process;
}

void ProcessManager::checkCall(const QString &phase, const QString &path, const QString &script) {
    this->curPhase = phase;
    QStringList arguments;
    arguments << "/c" << script;
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
    process->start("cmd.exe", arguments);
}

void ProcessManager::checkCallSpecific(const QString &phase, const QString &path, const QStringList& arguments) {
    this->curPhase = phase;
    process->terminate(); // 开始前先终止
    configWorkPath(path);
    qDebug() << arguments;
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

    env.insert("OUT_EBLIF", "top.eblif");
    env.insert("OUT_FASM_EXTRA", "top_fasm_extra.fasm");
    env.insert("OUT_JSON", "top.json");
    env.insert("OUT_SDC", "top.sdc");
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
    QByteArray output = process->readAllStandardOutput();
    // 获取错误输出
    QByteArray errorOutput = process->readAllStandardError();
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    // 转码
    QString outputStr = tc->toUnicode(output);
    QString errorOutputStr = tc->toUnicode(errorOutput);
    // 判断是否报错
    if (process->error() == QProcess::UnknownError) {
        // 没有错误发生，输出 output
        InfoWidget::instance()->appendLog(outputStr);
    } else {
        // 发生错误，输出 errorOutput
        InfoWidget::instance()->appendLog(errorOutputStr);
    }
}

// process执行结束后触发
void ProcessManager::handleFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    // 显示信息弹窗
    // exitCode 为0表示正常执行并成功退出
    if (exitCode == 0) {
        QMessageBox::information(nullptr, this->curPhase + " Completed", this->curPhase + " successfully completed.");
    } else {
        QMessageBox::critical(nullptr, this->curPhase + " Failed", this->curPhase + " failed.");
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
