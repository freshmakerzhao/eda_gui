#include "HardWareManager.h"
#include "TaskManager.h"

HardWareManager &HardWareManager::instance()
{
    static HardWareManager instance;
    return instance;
}

void HardWareManager::openProgramDevice()
{
    ProgramDeviceDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QFileInfo fileInfo(path);
        QString topName = fileInfo.fileName();
        QString projectImplPath = fileInfo.path();
        // qDebug() << topName;
        // qDebug() << projectImplPath;
        TaskManager::instance().downloadBit(projectImplPath, topName);
    }
}

HardWareManager::HardWareManager() {}
