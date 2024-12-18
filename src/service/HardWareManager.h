#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include <QObject>
#include "dialog/ProgramDeviceDialog.h"

class HardWareManager :public QObject
{
    Q_OBJECT
public:
    static HardWareManager& instance();

    void openProgramDeviceAndDownload(const int &mode = 0);
    void autoConnect();

    QString path;
    QString cable_name = "digilent_hs3";

private:
    HardWareManager();
    QString buildAutoConnectScript(const QString &cable="digilent_hs3");
    void publishScript(const QString &tclCommand);
};

#endif // HARDWAREMANAGER_H
