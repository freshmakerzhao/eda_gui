#include "TclConsole.h"
#include "utils/ProcessManager.h"

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
    Tcl_CreateCommand(interp, "synthesizer", TclCmdParse, nullptr, nullptr);
    Tcl_CreateCommand(interp, "implementation", TclCmdParse, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_device", TclSetDeviceCmd, nullptr, nullptr);
    Tcl_CreateCommand(interp, "set_work_dir", TclSetDeviceCmd, nullptr, nullptr);

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

int TclConsole::TclCmdParse(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
    QStringList command;
    for (int i = 1; i < argc; ++i) {
        command << argv[i];
    }

    QString phase = argv[0];
    ProcessManager::instance().excuteCommand(phase, command);
    return TCL_OK;
}

int TclConsole::TclSetDeviceCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
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

int TclConsole::TclSetWorkDirCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
    if (argc != 2) {
        Tcl_SetResult(interp, const_cast<char*>("Usage: set_device <part>"), TCL_STATIC);
        return TCL_ERROR;
    }
    const char* varName = "work_dir";
    const char* workDir = argv[1];
    const char* result = Tcl_SetVar(interp, varName, workDir, TCL_GLOBAL_ONLY);
    Tcl_SetResult(interp, const_cast<char*>(workDir), TCL_VOLATILE);
    return TCL_OK;
}