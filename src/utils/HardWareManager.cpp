#include "HardWareManager.h"
#include "TaskManager.h"
#include "component/tclconsole/TclConsole.h"

HardWareManager &HardWareManager::instance()
{
    static HardWareManager instance;
    return instance;
}

void HardWareManager::autoConnect() {
    QString arguments = buildAutoConnectScript();
    publishScript(arguments);
}

void HardWareManager::openProgramDeviceAndDownload(const int &mode)
{
    ProgramDeviceDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        TaskManager::instance().downloadBit(path, this->cable_name);
    }
}

HardWareManager::HardWareManager() {
    // Todo 这里后续增加检测cable的代码
    this->cable_name = "digilent_hs3";
}

// 自动连接，拿到寄存器信息
QString HardWareManager::buildAutoConnectScript(const QString &cable) {
    return "auto_connect -cable " + cable;
}

// 将命令提交给tcl console
void HardWareManager::publishScript(const QString &tclCommand) {
    TclConsole::instance()->executeTclCommand(tclCommand);
}
