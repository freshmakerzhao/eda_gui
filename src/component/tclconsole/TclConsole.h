#ifndef TCLCONSOLE_H
#define TCLCONSOLE_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QColor>
// #include <QProcess>
#include <QDebug>
#include <tcl.h>
#include "LineEditor.h"
#include "OutputEditor.h"

class TclConsole : public QWidget
{
    Q_OBJECT

public:
    static TclConsole *instance();

    void executeTclCommand(const QString &command);

private slots:
    void onCommandEnter(QString text);

signals:
    void simFinish(const QString &VCDJsonFilePath);

private:
    TclConsole(QWidget *parent = nullptr);
    ~TclConsole();

    static const QColor NORMAL_COLOR;
    static const QColor ERROR_COLOR;
    static const QColor OUTPUT_COLOR;

    Tcl_Interp *interp;
    Tcl_ChannelType *channelType;
    OutputEditor *output;
    LineEditor *input;
    static QString mVCD2JsonNamePath;
    static const QString mWaveFileName;
    static const QString mVCD2JsonFileName;
    // static QProcess *process;

    // 自定义输出函数
    static int QtTclOutput(ClientData clientData, const char *buf, int toWrite, int *errorCodePtr);

    // 自定义关闭函数
    static int QtTclClose(ClientData clientData, Tcl_Interp *interp);

    // 自定义 watchProc 函数
    static void QtTclWatch(ClientData clientData, int mask);

    // 自定义 getHandleProc 函数
    static int QtTclGetHandle(ClientData clientData, int direction, ClientData *handlePtr);

    // 自定义 seekProc 函数
    static int QtTclSeekProc(ClientData instanceData, long offset, int seekMode, int *errorCode);

    // 自定义 wideSeekProc 函数
    static Tcl_WideInt QtTclWideSeekProc(ClientData instanceData, Tcl_WideInt offset, int seekMode, int *errorCode);

    // ---------------------------- TCL Command ------------------------------

    static int TclHardwareCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclImplCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclSetDeviceCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclSetWorkDirCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclSetTopModuleCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclSynthCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclWriteBitstreamCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclUpdateFileSetCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);

    static int TclSimCmd(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]);
    static int generateSimWaveConfigFilePath(const QString topName, const QString simPath, QString& configWaveFullFilePath);
    static void getOriginalFile(Tcl_Interp *interp, QStringList& sourceFileList, const char* tclCommand);
};


#endif // TCLCONSOLE_H
