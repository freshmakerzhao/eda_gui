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
        QFileInfo fileInfo(path);
        QString topName = fileInfo.fileName();
        QString projectImplPath = fileInfo.path();
        qDebug() << topName;
        qDebug() << projectImplPath;

        if (mode == 0) {
            TaskManager::instance().downloadBit(projectImplPath, topName);
        } else if (mode == 1) {
            TaskManager::instance().downloadFlash(projectImplPath, topName);
        }
    }
}

HardWareManager::HardWareManager() {}
