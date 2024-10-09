#include "HardWareManager.h"
#include "TaskManager.h"

HardWareManager &HardWareManager::instance()
{
    static HardWareManager instance;
    return instance;
}

void HardWareManager::openProgramDevice(const int &mode)
{
    ProgramDeviceDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        TaskManager::instance().downloadBit(path);
    }
}

HardWareManager::HardWareManager() {}
