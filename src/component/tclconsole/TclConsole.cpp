#include "TclConsole.h"
#include "utils/DeviceInfoUtils.h"
#include "utils/ProcessManager.h"
#include "utils/ProjectManager.h"
#include "base/Globals.h"

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
    Tcl_CreateCommand(interp, "impl_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "pack_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "place_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "route_design", TclImplCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "update_fileset", TclUpdateFileSetCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "write_bitstream", TclWriteBitstreamCmd, nullptr, nullptr);
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
        addCommonArgs(synthJsonPath, packJsonPath, {"--pack-only"});
        script << "&&";
        addCommonArgs(packJsonPath, placeJsonPath, {"--no-pack", "--no-route"});
        script << "&&";
        addCommonArgs(placeJsonPath, routeJsonPath, {"--fasm", fasmPath, "--no-pack", "--no-place"});
        info = "Starting Implementation Task";
    } else if (task == "pack_design") {
        addCommonArgs(synthJsonPath, packJsonPath, {"--pack-only"});
        info = "Starting Pack Task";
    } else if (task == "place_design") {
        addCommonArgs(packJsonPath, placeJsonPath, {"--no-pack", "--no-route"});
        info = "Starting Place Task";
    } else if (task == "route_design") {
        addCommonArgs(placeJsonPath, routeJsonPath, {"--fasm", fasmPath, "--no-pack", "--no-place"});
        info = "Starting Route Task";
    } else {
        info = "Unknown implement command";
    }


    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    const QString phase = "Implementation";
    ProcessManager::instance().configWorkPath(implPath);
    ProcessManager::instance().excuteCommand(phase, script);
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
    const char *tclVarValue = Tcl_GetVar(interp, "source", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);

        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QDir dir(workDir);
    const QString jsonPath = dir.filePath("runs/synth/" + topName + ".json");
    const QString edifPath = dir.filePath("runs/synth/" + topName + ".edn");
    const QString synthPath = dir.filePath("runs/synth");

    QStringList script;
    script << "-p";
    script << QString("synth_xilinx -flatten -nowidelut -abc9 -arch xc7 -top %1; write_json %2; write_edif -pvector bra %3;")
                  .arg(topName, jsonPath, edifPath);
    // Design Sources
    for (const QString &item : resultList) {
        script << item;
    }

    QString info = QString("Starting synth_design\n"
                           "Using part: %1\n"
                           "Top: %2").arg(deviceModel, topName);
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    ProcessManager::instance().configWorkPath(synthPath);
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
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
    } else {
        std::string errorMsg = "Error: Option " + fileset +  ".";
        Tcl_SetResult(interp, (char*)errorMsg.c_str(), TCL_VOLATILE);
        return TCL_ERROR;
    }

    return TCL_OK;
}

int TclConsole::TclWriteBitstreamCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {


    QStringList script;
    QString part_name = ProjectManager::instance().getParameter(Project::Part);

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

    script << "%FASM2FRAMES%";
    script << "--part";
    script << part_name;
    script << "--db-root";
    script << GlobalConfig::GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\hybrdlink_db\MC7F)";
    script << "--fn_out";
    script << framesPath;
    script << "--fn_in";
    script << fasmPath;
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

    QString info = "Starting Generate Bitstream Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    const QString phase = "Generate Bitstream";
    ProcessManager::instance().configWorkPath(implPath);
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}
