#include "TclConsole.h"
#include "utils/ProcessManager.h"
#include "utils/ProjectManager.h"
#include "base/Globals.h"

const QColor TclConsole::NORMAL_COLOR = QColor::fromRgbF(0, 0, 0);
const QColor TclConsole::ERROR_COLOR = QColor::fromRgbF(1.0, 0, 0);
const QColor TclConsole::OUTPUT_COLOR = QColor::fromRgbF(0, 0, 1.0);
QProcess* TclConsole::process = new QProcess();

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

    output = new QTextEdit(this);
    output->setReadOnly(true);
    layout->addWidget(output);

    input = new LineEditor(this);
    layout->addWidget(input);

    // connect(input, &QLineEdit::returnPressed, this, &TclConsole::onCommandEnter);
    connect(input, &LineEditor::textLineInserted, this, &TclConsole::onCommandEnter);

    interp = Tcl_CreateInterp();
    if (Tcl_Init(interp) == TCL_ERROR) {
        output->append("Error initializing Tcl interpreter: ");
        output->append(Tcl_GetStringResult(interp));
        return;
    }

    // 定义自定义通道类型
    channelType = new Tcl_ChannelType;
    // memset(channelType, 0, sizeof(Tcl_ChannelType));
    channelType->typeName = "qtConsole";
    channelType->outputProc = &QtTclOutput;
    channelType->closeProc = &QtTclClose;
    channelType->watchProc = &QtTclWatch;
    channelType->getHandleProc = &QtTclGetHandle;

    // 创建并注册通道
    Tcl_Channel channel = Tcl_CreateChannel(channelType, "qtConsoleChannel", reinterpret_cast<ClientData>(output), TCL_WRITABLE);
    Tcl_SetStdChannel(channel, TCL_STDOUT);
    Tcl_SetStdChannel(channel, TCL_STDERR);
    Tcl_RegisterChannel(interp, channel);

    output->append("Tcl console initialized. Type your commands in the line edit below.");
    Tcl_CreateCommand(interp, "impl_design", TclImplCmd, nullptr, nullptr);

    Tcl_CreateCommand(interp, "set_device", TclSetDeviceCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_work_dir", TclSetWorkDirCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_top_module", TclSetTopModuleCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "synth_design", TclSynthCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "pack_design", TclPackCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "place_design", TclPlaceCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "route_design", TclRouteCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "update_fileset", TclUpdateFileSetCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "write_bitstream", TclBitstreamCmd, nullptr, nullptr);

    // 读取标准输出并将其写入到 Tcl 标准输出通道
    connect(process, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray outputData = process->readAllStandardOutput();
        Tcl_Channel stdoutChannel = Tcl_GetStdChannel(TCL_STDOUT);
        if (stdoutChannel) {
            // Tcl_Write(stdoutChannel, outputData.constData(), outputData.size());
            // output->setTextColor(OUTPUT_COLOR);
            output->append(QString::fromUtf8(outputData));
            // output->setTextColor(NORMAL_COLOR);
            Tcl_Flush(stdoutChannel);
        }
    });

    // 读取标准错误并将其写入到 Tcl 标准输出通道
    connect(process, &QProcess::readyReadStandardError, [&]() {
        QByteArray errorData = process->readAllStandardError();
        Tcl_Channel stdoutChannel = Tcl_GetStdChannel(TCL_STDOUT);
        if (stdoutChannel) {
            // Tcl_Write(stdoutChannel, errorData.constData(), errorData.size());
            // output->setTextColor(ERROR_COLOR);
            output->append(QString::fromUtf8(errorData));
            // output->setTextColor(NORMAL_COLOR);
            Tcl_Flush(stdoutChannel);
        }
    });

    // 当进程结束时，进行清理
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        // Tcl_Channel stdoutChannel = Tcl_GetStdChannel(TCL_STDOUT);
        // QByteArray errorData = "Synthesis OK";
        // Tcl_Write(stdoutChannel, errorData.constData(), errorData.size());
        // output->append(QString::fromUtf8(errorData));
        // output->moveCursor(QTextCursor::End);
        // process->deleteLater();
    });
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
    QTextEdit *output = reinterpret_cast<QTextEdit*>(clientData);
    if (output) {
        output->moveCursor(QTextCursor::End);
        output->insertPlainText(QString::fromUtf8(buf, toWrite));
        output->moveCursor(QTextCursor::End);
    }
    return toWrite;
}

int TclConsole::QtTclClose(ClientData clientData, Tcl_Interp *interp) {
    // 处理通道关闭时的清理工作
    QTextEdit *output = reinterpret_cast<QTextEdit*>(clientData);
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
    // output->setTextColor(OUTPUT_COLOR);
    Tcl_Flush(channel);
    // output->setTextColor(NORMAL_COLOR);
    // Tcl_Seek(channel, 0, SEEK_SET);
    // char buffer[1024];
    // int bytesRead = Tcl_Read(channel, buffer, sizeof(buffer) - 1);
    // // Null-terminate the buffer
    // if (bytesRead >= 0) {
    //     buffer[bytesRead] = '\0';
    //     // Convert to QString
    //     QString result = QString::fromUtf8(buffer, bytesRead);
    //     qDebug() << result << 1;
    //     output->setTextColor(OUTPUT_COLOR);
    //     output->append("\n" + result);
    //     output->setTextColor(NORMAL_COLOR);
    // }
    // output->moveCursorToEnd();
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

    QStringList script;
    script << "--chipdb";
    script << GLOBAL_RESOURCE_PATH + "/common/archs/" + "xc7a100t.bin";

    QStringList resultList;
    const char *tclVarValue = Tcl_GetVar(interp, "constrs", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);

        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    script << "--xdc";
    script << resultList;
    script << "--json";
    script << workDir + "/runs/synth/" + topName + ".json";
    script << "--write";
    script << workDir + "/runs/impl/route.json";
    script << "--fasm";
    script << workDir + "/runs/impl/" + topName + ".fasm";

    QString info = "Starting Implementation Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    QString phase = "Implementation";
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclSetDeviceCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    if (argc != 2) {
        Tcl_SetResult(interp, const_cast<char*>("Usage: set_device <part>"), TCL_STATIC);
        return TCL_ERROR;
    }
    const char* varName = "device_model";
    const char* part = argv[1];
    const char* result = Tcl_SetVar(interp, varName, part, TCL_GLOBAL_ONLY);
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
    Tcl_SetResult(interp, const_cast<char*>(topName), TCL_VOLATILE);
    return TCL_OK;
}

int TclConsole::TclSynthCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    QString phase = "Synthesis";
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

    const char* deviceModel = Tcl_GetVar(interp, "device_model", TCL_GLOBAL_ONLY);
    if (deviceModel == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Device model not set"), TCL_STATIC);
        return TCL_ERROR;
    }


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
    QString jsonPath = dir.filePath("runs/synth/" + topName + ".json");
    QString edifPath = dir.filePath("runs/synth/" + topName + ".edn");

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

    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclPackCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }

    const QString topName = QString(topVar);

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QStringList script;
    script << "--chipdb";
    script << GLOBAL_RESOURCE_PATH + "/common/archs/" + "xc7a100t.bin";

    QStringList resultList;
    const char *tclVarValue = Tcl_GetVar(interp, "constrs", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);

        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    script << "--xdc";
    script << resultList;
    script << "--json";

    script << workDir + "/runs/synth/" + topName + ".json";
    script << "--write";
    script << workDir + "/runs/impl/pack.json";
    script << "--pack-only";

    QString info = "Starting Pack Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    QString phase = "Implementation";
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclPlaceCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }

    const QString topName = QString(topVar);

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QStringList script;
    script << "--chipdb";
    script << GLOBAL_RESOURCE_PATH + "/common/archs/" + "xc7a100t.bin";

    QStringList resultList;
    const char *tclVarValue = Tcl_GetVar(interp, "constrs", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);

        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    script << "--xdc";
    script << resultList;
    script << "--json";

    script << workDir + "/runs/impl/pack.json";
    script << "--write";
    script << workDir + "/runs/impl/place.json";
    script << "--no-pack";
    script << "--no-route";

    QString info = "Starting Place Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    QString phase = "Implementation";
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}

int TclConsole::TclRouteCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }

    const QString topName = QString(topVar);

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);

    QStringList script;
    script << "--chipdb";
    script << GLOBAL_RESOURCE_PATH + "/common/archs/" + "xc7a100t.bin";

    QStringList resultList;
    const char *tclVarValue = Tcl_GetVar(interp, "constrs", 0);
    if (tclVarValue != nullptr) {
        // 将 Tcl 列表字符串转换为 QString
        QString qStringList = QString::fromUtf8(tclVarValue);

        // 将 QString 转换为 QStringList
        resultList = qStringList.split(' ', Qt::SkipEmptyParts);
    }

    script << "--xdc";
    script << resultList;
    script << "--json";
    script << workDir + "/runs/impl/place.json";
    script << "--write";
    script << workDir + "/runs/impl/route.json";
    script << "--fasm";
    script << workDir + "/runs/impl/" + topName + ".fasm";
    script << "--no-pack";
    script << "--no-place";

    QString info = "Starting Route Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    QString phase = "Implementation";
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
    } else if (fileset == "constrs") {
        stringList = ProjectManager::instance().getConstraintsList();
        QString listStr;
        listStr = stringList.join(" ");
        listStr = listStr.trimmed();
        Tcl_SetVar(interp, "constrs", listStr.toUtf8().constData(), 0);
    } else {
        std::string errorMsg = "Error: Option " + fileset +  ".";
        Tcl_SetResult(interp, (char*)errorMsg.c_str(), TCL_VOLATILE);
        return TCL_ERROR;
    }

    return TCL_OK;
}

int TclConsole::TclBitstreamCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {


    QStringList script;
    QString part_name = "xc7a100tfgg484-2";

    const char *workDirVar = Tcl_GetVar(interp, "work_dir", 0);
    const QString workDir = QString(workDirVar);
    const char* topVar = Tcl_GetVar(interp, "top_module", TCL_GLOBAL_ONLY);
    if (topVar == nullptr) {
        Tcl_SetResult(interp, const_cast<char*>("Top module not set"), TCL_STATIC);
        return TCL_ERROR;
    }
    const QString topName = QString(topVar);

    QDir dir(workDir);
    QString fasmPath = dir.filePath("runs/impl/" + topName + ".fasm");
    QString framesPath = dir.filePath("runs/impl/" + topName + ".frames");
    QString bitstreamPath = dir.filePath("runs/impl/" + topName + ".bit");

    script << "%FASM2FRAMES%";
    script << "--part";
    script << part_name;
    script << "--db-root";
    script << GLOBAL_RESOURCE_PATH + R"(\bitstreamTools\hybrdlink_db\MC7F)";
    script << "--fn_out";
    script << framesPath;
    script << "--fn_in";
    script << fasmPath;
    script << "&&";
    script << "%FRAMES2BIT%";
    script << "--part_file";
    script << GLOBAL_RESOURCE_PATH + "/bitstreamTools/hybrdlink_db/MC7F/" + part_name + "/part.yaml";
    script << "--part_name";
    script << part_name;
    script << "--frm_file";
    script << framesPath;
    script << "--output_file";
    script << bitstreamPath;

    QString info = "Starting Gen Bit Task\n";
    Tcl_SetResult(interp, const_cast<char*>(info.toStdString().c_str()), TCL_VOLATILE);

    QString phase = "GeneratorBitstream";
    ProcessManager::instance().excuteCommand(phase, script);
    return TCL_OK;
}
