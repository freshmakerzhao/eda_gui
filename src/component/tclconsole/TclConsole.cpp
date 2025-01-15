#include "TclConsole.h"
#include "utils/DeviceInfoUtils.h"
#include "utils/ProcessManager.h"
#include "utils/ProjectManager.h"
#include "base/Globals.h"
#include "dialog/CustomMessageBox.h"

const QColor TclConsole::NORMAL_COLOR = QColor::fromRgbF(0, 0, 0);
const QColor TclConsole::ERROR_COLOR = QColor::fromRgbF(1.0, 0, 0);
const QColor TclConsole::OUTPUT_COLOR = QColor::fromRgbF(0, 0, 1.0);

TclConsole *TclConsole::instance()
{
    static TclConsole *_instance = nullptr;
    if (!_instance) {
        _instance = new TclConsole(nullptr);
    }
    return _instance;
}

TclConsole::TclConsole(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    output = new OutputEditor(this);
    layout->addWidget(output);

    input = new LineEditor(this);
    layout->addWidget(input);

    connect(input, &LineEditor::textLineInserted, this, &TclConsole::onCommandEnter);

    interp = Tcl_CreateInterp();
    if (Tcl_Init(interp) == TCL_ERROR) {
        output->append("Error initializing Tcl interpreter: ");
        output->append(Tcl_GetStringResult(interp));
        return;
    }

    // 定义自定义通道类型
    channelType = new Tcl_ChannelType;
    // 使用 memset 将结构体的所有字节设置为0
    memset(channelType, 0, sizeof(Tcl_ChannelType));
    // 设置结构体的字段
    channelType->typeName = "qtConsole";
    channelType->outputProc = &QtTclOutput;
    channelType->closeProc = &QtTclClose;
    channelType->watchProc = &QtTclWatch;
    channelType->getHandleProc = &QtTclGetHandle;
    channelType->seekProc = &QtTclSeekProc;       // 添加 seekProc
    channelType->wideSeekProc = &QtTclWideSeekProc; // 添加 wideSeekProc

    // 创建并注册通道
    Tcl_Channel channel = Tcl_CreateChannel(channelType, "qtConsoleChannel", reinterpret_cast<ClientData>(output), TCL_WRITABLE);
    Tcl_SetStdChannel(channel, TCL_STDOUT);
    Tcl_SetStdChannel(channel, TCL_STDERR);
    Tcl_RegisterChannel(interp, channel);

    output->append("Tcl console initialized. Type your commands in the line edit below.");

    Tcl_CreateCommand(interp, "set_device", TclSetDeviceCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_work_dir", TclSetWorkDirCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_top_module", TclSetTopModuleCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "synth_design", TclSynthCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "auto_connect", TclHardwareCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "impl_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "pack_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "place_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "route_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "update_fileset", TclUpdateFileSetCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "write_bitstream", TclWriteBitstreamCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "sim_design", TclSimCmd, nullptr, nullptr);
}

TclConsole::~TclConsole() {
    Tcl_DeleteInterp(interp);
    delete channelType;
    //    process->deleteLater();
}

void TclConsole::onCommandEnter(QString text) {
    QString command = text;
    // if (!command.isEmpty()) {
    executeTclCommand(command);
    // }
}

int TclConsole::QtTclOutput(ClientData clientData, const char *buf, int toWrite, int *errorCodePtr) {
    OutputEditor *output = reinterpret_cast<OutputEditor*>(clientData);
    if (output) {
        output->moveCursor(QTextCursor::End);
        output->insertPlainText(QString::fromUtf8(buf, toWrite));
        output->moveCursor(QTextCursor::End);
    }
    return toWrite;
}

int TclConsole::QtTclClose(ClientData clientData, Tcl_Interp *interp) {
    // 处理通道关闭时的清理工作
    OutputEditor *output = reinterpret_cast<OutputEditor*>(clientData);
    if (output) {
        // 清理操作，如必要时释放资源
    }
    return 0;
}

void TclConsole::QtTclWatch(ClientData clientData, int mask) {
    // 由于我们不处理异步事件，可以留空
}

int TclConsole::QtTclGetHandle(ClientData clientData, int direction, ClientData *handlePtr) {
    return TCL_ERROR;  // 由于我们不涉及系统级文件描述符，返回错误
}

int TclConsole::QtTclSeekProc(ClientData instanceData, long offset, int seekMode, int *errorCode) {
    *errorCode = 0;
    return -1;  // 或者根据需要返回0表示成功
}

Tcl_WideInt TclConsole::QtTclWideSeekProc(ClientData instanceData, Tcl_WideInt offset, int seekMode, int *errorCode) {
    *errorCode = 0;
    return -1;  // 或者根据需要返回0表示成功
}

void TclConsole::executeTclCommand(const QString &command) {
    output->setTextColor(NORMAL_COLOR);
    output->append(QString("> %1").arg(command));
    if (Tcl_Eval(interp, command.toUtf8().constData()) == TCL_ERROR) {
        output->setTextColor(ERROR_COLOR);
        output->append("Error: " + QString(Tcl_GetStringResult(interp)));
        output->setTextColor(NORMAL_COLOR);
        return;
    }
    output->setTextColor(OUTPUT_COLOR);
    output->append(QString(Tcl_GetStringResult(interp)));
    output->setTextColor(NORMAL_COLOR);
    Tcl_Channel channel = Tcl_GetStdChannel(TCL_STDOUT);
    if (!channel) {
        return;
    }
    output->setTextColor(OUTPUT_COLOR);
    Tcl_Flush(channel);
    output->moveCursor(QTextCursor::End);
}

int TclConsole::TclHardwareCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
    QStringList script;
    QString info;
    const QString task = QString(argv[0]);
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-cable") {
            if (i + 1 < argc) { // 校验
                args[arg] = argv[++i];
            } else {
                std::string errorMsg = "Error: Option " + arg + " requires a value.";
                Tcl_SetResult(interp, (char*)errorMsg.c_str(), TCL_VOLATILE);
                return TCL_ERROR;
            }
        } else {
            Tcl_SetResult(interp, (char*)"Warning: Unknown option ", TCL_STATIC);
        }
    }
    if (task == "auto_connect") {
        script << "%BITSTREAMTOOL_PATH%";
        script << "-c";
        if (args.find("-cable") != args.end()) {
            script << QString::fromStdString(args["-cable"]);
        } else {
            script << "digilent_hs3";
        }
        script << "--read-register-from-address";
        script << "01100,10110,00111,01001,10001,01101,10000";
        script << "--father-process-id";
        script << InitialConfig::instance().pid_str;
        info = "Starting Auto Connect";
    } else {
        info = "Unknown Hardware Command";
    }

    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    const QString phase = "Auto Connect";
    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);
    QDir dir(workDir);
    const QString implPath = dir.filePath("runs/impl");
    ProcessManager::instance().configWorkPath(implPath);
    ProcessManager::instance().executeCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclImplCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }

    const QString topName = QString(topVar);

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QStringList resultList;
    const char *tclVarValue = Tcl_GetVar(interp, "constrs", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);
        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    if (resultList.isEmpty()) {
        const std::string errorMessage = "There are no Design Constraints in the project. Please use \"Add Source\" to add files.";
        Tcl_SetResult(interp, const_cast<char*>(errorMessage.c_str()), TCL_STATIC);
        QWidget *parent = QApplication::activeWindow();
        CustomMessageBox::showError(nullptr, "Run lmplementation", QString::fromStdString(errorMessage));
        return TCL_ERROR;
    }

    QDir dir(workDir);
    const QString implPath = dir.filePath("runs/impl");
    const QString synthJsonPath = dir.filePath("runs/synth/" + topName + ".json");
    const QString packJsonPath = dir.filePath("runs/impl/pack.json");
    const QString placeJsonPath = dir.filePath("runs/impl/place.json");
    const QString routeJsonPath = dir.filePath("runs/impl/route.json");
    const QString fasmPath = dir.filePath("runs/impl/" + topName + ".fasm");
    QStringList script;

    QString info;
    // 公共部分提取为函数，避免重复代码
    auto addCommonArgs = [&script, &resultList](const QString &jsonPath,
                                                const QString &writePath,
                                                const QStringList &extraArgs = {}) {
        const QString archName = ProjectManager::instance().getParameter(Project::ArchName);
        script << "%IMPL_PATH%";
        script << "--chipdb";
        script << GlobalConfig::GLOBAL_RESOURCE_PATH + "/common/archs/" + archName + ".bin";
        script << "--xdc" << resultList;
        script << "--json" << jsonPath;
        script << "--write" << writePath;
        script << extraArgs;
    };

    const QString task = QString(argv[0]);

    if (task == "impl_design") {
        addCommonArgs(synthJsonPath, routeJsonPath, {"--fasm", fasmPath, "--U", "--debug", "--process_number", InitialConfig::instance().pid_str, "-l", "log_implementation.log"});
        info = "Starting Implementation Task";
    } else if (task == "pack_design") {
        addCommonArgs(synthJsonPath, packJsonPath, {"--pack-only", "-l", "log_pack.log"});
        info = "Starting Pack Task";
    } else if (task == "place_design") {
        addCommonArgs(packJsonPath, placeJsonPath, {"--no-pack", "--no-route", "-l", "log_place.log"});
        info = "Starting Place Task";
    } else if (task == "route_design") {
        addCommonArgs(placeJsonPath, routeJsonPath, {"--fasm", fasmPath, "--no-pack", "--no-place", "-l", "log_route.log"});
        info = "Starting Route Task";
    } else {
        info = "Unknown implement command";
    }

    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    const QString phase = "Implementation";
    ProcessManager::instance().configWorkPath(implPath);
    ProcessManager::instance().executeCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclSetDeviceCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    if (argc != 2) {
        Tcl_SetResult(interp, const_cast<char*>("Usage: set_device <part>"), TCL_STATIC);
        return TCL_ERROR;
    }
    // const char* varName = "device_model";
    const char* part = argv[1];
    // const char* result = Tcl_SetVar(interp, varName, part, TCL_GLOBAL_ONLY);
    ProjectManager::instance().setDevicePart(part);
    Tcl_SetResult(interp, const_cast<char*>(part), TCL_VOLATILE);
    return TCL_OK;
}

int TclConsole::TclSetWorkDirCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    if (argc != 2) {
        Tcl_SetResult(interp, const_cast<char*>("Usage: set_work_dir <dir>"), TCL_STATIC);
        return TCL_ERROR;
    }
    const char* varName = "work_dir";
    const char* workDir = argv[1];
    const char* result = Tcl_SetVar(interp, varName, workDir, TCL_GLOBAL_ONLY);
    Tcl_SetResult(interp, const_cast<char*>(workDir), TCL_VOLATILE);
    return TCL_OK;
}

int TclConsole::TclSetTopModuleCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    if (argc != 2) {
        Tcl_SetResult(interp, const_cast<char*>("Usage: set_top_module <module>"), TCL_STATIC);
        return TCL_ERROR;
    }
    const char* varName = "top_module";
    const char* topName = argv[1];
    const char* result = Tcl_SetVar(interp, varName, topName, TCL_GLOBAL_ONLY);
    QString info = QString("Top module: %1").arg(topName);
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);
    return TCL_OK;
}

int TclConsole::TclSynthCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    const QString phase = "Synthesis";
    // 是否为兼容模式
    bool isCompatibilityMode = false;
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-top") {
            if (i + 1 < argc) { // 确保参数后有值
                args[arg] = argv[++i];
            } else {
                // std::cerr << "Error: Option " << arg << " requires a value." << std::endl;
                std::string errorMsg = "Error: Option " + arg + " requires a value.";
                Tcl_SetResult(interp, (char*)errorMsg.c_str(), TCL_VOLATILE);
                return TCL_ERROR;
            }
        } else if (arg == "-compatibility_mode") {
            // 当传入 compatibility_mode 时，表示开启兼容模式，isCompatibilityMode 置为 true
            isCompatibilityMode = true;
        } else {
            Tcl_SetResult(interp, (char*)"Warning: Unknown option ", TCL_STATIC);
        }
    }

    QString deviceModel = ProjectManager::instance().getParameter(Project::DisplayPart);
    // const char* deviceModel = Tcl_GetVar(interp, "device_model", TCL_GLOBAL_ONLY);
    // if (deviceModel == nullptr) {
    //     Tcl_SetResult(interp, const_cast<char*>("Device model not set"), TCL_STATIC);
    //     return TCL_ERROR;
    // }

    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (args.find("-top") != args.end()) {
        topVar = args.at("-top").c_str();
    }


    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }

    const QString topName = QString(topVar);
    // qDebug() << topName;

    QStringList resultList;
    getOriginalFile(interp, resultList, "source");

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QDir dir(workDir);
    const QString jsonPath = dir.filePath("runs/synth/" + topName + ".json");
//    const QString edifPath = dir.filePath("runs/synth/" + topName + ".edn");
    const QString viewVerilogPath = dir.filePath("runs/synth/" + topName + ".v");
    const QString synthPath = dir.filePath("runs/synth");

    QStringList script;
    script << "-p";
    script << QString("synth_xilinx -flatten -nowidelut -abc9 -arch xc7 -top %1; select -module %1; write_json -selected %2; write_verilog -selected %3; ")
            .arg(topName, jsonPath, viewVerilogPath);
    // Design Sources
    for (const QString &item : resultList) {
        script << item;
    }

    // IP Core
    for (const QString &item : ProjectManager::instance().ipList) {
        script << item;
    }

    // 进程id
    script << "-K";
    script << InitialConfig::instance().pid_str;

    // 兼容模式下 增加 -R ，表示开启兼容模式，能够加载hybrdchip之外的原语
    if (isCompatibilityMode){
        script << "-R";
    }
    // 加密网表
    script << "-U";
    // log verbose
    script << "-v 9";

    QString info = QString("Starting synth_design\n"
                           "Using part: %1\n"
                           "Top: %2").arg(deviceModel, topName);
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    ProcessManager::instance().configWorkPath(synthPath);
    ProcessManager::instance().executeCommand(phase, script);
    qDebug() << script;
    return TCL_OK;
}

int TclConsole::TclSimCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    const QString phaseSimulation ="Simulation Run";
    //  argv ： sim_design     编译源文件
    //  iverilog  -y  verilog\src\retarget\ -y verilog\src\unisims\ -o tb_run  sourve.v

    //   command
    QStringList scriptSimRun;
    scriptSimRun << "%SIMULATION_COMPILER_PATH%";
    //  -y lib
   const QString designElementPath = GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\simulationer\share\element_design_lib\)";
    scriptSimRun << QString("-y")<<(designElementPath + R"(retarget\)");
    scriptSimRun << QString("-y")<<(designElementPath + R"(unisims\)");
    scriptSimRun << QString("-y")<<(designElementPath + R"(glbl\)");
    //  -o  compile_file
    const  QString compileFile = "tb_run";
    //scriptFirst << QString("-o  %1").arg("tb_run");
    // scriptFirst << QString("-o  tb_run");
    scriptSimRun << "-o" << compileFile;
     // Design Sources
    QStringList sourceFileList;
    getOriginalFile(interp, sourceFileList, "source");
    scriptSimRun.append(sourceFileList);
    // 仿真路径
    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);
    QDir dir(workDir);
    const QString simPath = dir.filePath("runs/sim");

    // 添加激励文件
    QStringList tbSourceFileList;
    getOriginalFile(interp, tbSourceFileList, "simulation");
    scriptSimRun.append(tbSourceFileList);
    // 生成波形配置文件
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }
    const QString topName = QString(topVar);
    QString configWaveFullFilePath;
    if(generateSimWaveConfigFilePath(topName, simPath, configWaveFullFilePath )) {
        Tcl_SetResult(interp, const_cast<char*>("Unable to open the waveform configuration file."), TCL_STATIC);
        return TCL_ERROR;
    }
    //  添加波形配置文件
    scriptSimRun << configWaveFullFilePath;

    // generating command finish.
    QString info = QString("Starting sim_design\n");
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);
    //  设置路径
    ProcessManager::instance().configWorkPath(simPath);

    //  执行仿真:  vvp  compileFile
    scriptSimRun << "&&";
    scriptSimRun <<"%SIMULATION_RUN_PATH%" << compileFile;
    ProcessManager::instance().executeCommand(phaseSimulation, scriptSimRun);

    return TCL_OK;
}

int TclConsole::generateSimWaveConfigFilePath(const QString topName, const QString simPath, QString& configWaveFullFilePath )
{
    int error = 0;
    // 默认的波形生成，只生成最上层的信号。
    //  配置信息
    const QString waveFileName = "hybrdLinkWaveFile.vcd";
    const QString WaveSignalConfig = "1, " + topName;  // 1:只输出此层的信号，具体使用查询$dumpvars的使用
    const QString moduleName = "hybrdLinkWaveConfigModule";
    // 生成文件
    const QDir simFilePath(simPath);
    const QString configFileName(moduleName + ".v");
    configWaveFullFilePath =  simFilePath.filePath(configFileName);

    QFile onfigFile(configWaveFullFilePath);
    if(onfigFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&onfigFile);
        out << QString( "module %1 ();").arg(moduleName) <<Qt:: endl;
        out << "initial begin" << Qt::endl;
        out << QString( "$dumpfile( \"%1\") ;" ).arg(waveFileName ) <<Qt:: endl;
        out << QString("$dumpvars( %1 ) ;").arg(WaveSignalConfig) <<Qt:: endl;
        out << "end" << Qt::endl;
        out << "endmodule" << Qt::endl;
        onfigFile.close();
        qDebug()  << "The waveform configuration file has been generated." <<Qt::endl;
    } else {
        qDebug() << "Unable to open the waveform configuration file."  << Qt::endl;
        error = 1;
    }
    return error;
}

void TclConsole::getOriginalFile(Tcl_Interp *interp, QStringList& sourceFileList, const char* tclCommand)
{
    const char *tclVarValue = Tcl_GetVar(interp, tclCommand, 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);
        // 将 QString 转换为 QStringList
        sourceFileList = qStringList.split(' ', Qt::SkipEmptyParts);
    }
    return;
}

int TclConsole::TclUpdateFileSetCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    QStringList stringList;
    std::string fileset = argv[1];
    if (fileset == "sources") {
        stringList = ProjectManager::instance().getDesignSourcesList();
        QString listStr;
        listStr = stringList.join(" ");
        listStr = listStr.trimmed();
        Tcl_SetVar(interp, "source", listStr.toUtf8().constData(), 0);
        Tcl_SetResult(interp, const_cast<char*>("INFO: Update design sources"), TCL_VOLATILE);
    } else if (fileset == "constrs") {
        stringList = ProjectManager::instance().getConstraintsList();
        QString listStr;
        listStr = stringList.join(" ");
        listStr = listStr.trimmed();
        Tcl_SetVar(interp, "constrs", listStr.toUtf8().constData(), 0);
        Tcl_SetResult(interp, const_cast<char*>("INFO: Update constraints"), TCL_VOLATILE);
    } else if (fileset == "simulations") {
        stringList = ProjectManager::instance().getSimSourcesList();
        QString listStr;
        listStr = stringList.join(" ");
        listStr = listStr.trimmed();
        Tcl_SetVar(interp, "simulation", listStr.toUtf8().constData(), 0);
        Tcl_SetResult(interp, const_cast<char*>("INFO: Update simulation sources"), TCL_VOLATILE);
    } else {
        std::string errorMsg = "Error: Option " + fileset +  ".";
        Tcl_SetResult(interp, (char*)errorMsg.c_str(), TCL_VOLATILE);
        return TCL_ERROR;
    }

    return TCL_OK;
}

int TclConsole::TclWriteBitstreamCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
    QStringList script;
    QString part_name = ProjectManager::instance().getParameter(Project::Part);

    bool enableCRC = false;
    bool enableCompress = false;
    bool generatorBin = false;
    bool generatorRbt = false;
    bool parse_bitstream_mode = false;

    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-crc") {
            enableCRC = true;
            parse_bitstream_mode = true;
        } else if (arg == "-compress"){
            enableCompress = true;
            parse_bitstream_mode = true;
        } else if (arg == "-bin"){
            generatorBin = true;
            parse_bitstream_mode = true;
        } else if (arg == "-rbt"){
            generatorRbt = true;
            parse_bitstream_mode = true;
        }
    }

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }
    const QString topName = QString(topVar);

    QDir dir(workDir);
    const QString implPath = dir.filePath("runs/impl");
    const QString fasmPath = dir.filePath("runs/impl/" + topName + ".fasm");
    const QString framesPath = dir.filePath("runs/impl/" + topName + ".frames");
    const QString bitstreamPath = dir.filePath("runs/impl/" + topName + ".bit");
    const QString binPath = dir.filePath("runs/impl/" + topName + ".bin");
    const QString rbtPath = dir.filePath("runs/impl/" + topName + ".rbt");

    script << "%FASM2FRAMES%";
    script << "--part";
    script << part_name;
    script << "--db-root";
    script << GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\hybrdlink_db\MC7F)";
    script << "--fn_out";
    script << framesPath;
    script << "--fn_in";
    script << fasmPath;
    script << "--start_decompression"; // 解密
    script << "&&";
    script << "%FRAMES2BIT%";
    script << "--part_file";
    script << GlobalConfig::GLOBAL_RESOURCE_PATH + "/bitstreamTools/hybrdlink_db/MC7F/" + part_name + "/part.yaml";
    script << "--part_name";
    script << part_name;
    script << "--frm_file";
    script << framesPath;
    script << "--output_file";
    script << bitstreamPath;
    script << "--process_number";
    script << InitialConfig::instance().pid_str;

//    PARSE_BITSTREAM_PATH

    if (parse_bitstream_mode){
        // 如果有 parse_bitstream 的扩展功能
        script << "&&";
        script << "%PARSE_BITSTREAM_PATH%";
        script << "--file";
        script << bitstreamPath;
        if (enableCRC)
            script << "--CRC";
        if (enableCompress)
            script << "--COMPRESS";
        if (generatorBin)
            script << "--bin";
        if (generatorRbt)
            script << "--rbt";
    }

    QString info = "Starting Generate Bitstream Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    const QString phase = "Generate Bitstream";
    ProcessManager::instance().configWorkPath(implPath);
    ProcessManager::instance().executeCommand(phase, script);
    return TCL_OK;
}
