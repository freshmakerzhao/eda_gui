#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include <QObject>
#include <dialog/ProgramDeviceDialog.h>

class HardWareManager :public QObject
{
    Q_OBJECT
public:
    static HardWareManager& instance();

    void openProgramDevice();

    QString path;
private:
    HardWareManager();

};

#endif // HARDWAREMANAGER_H
