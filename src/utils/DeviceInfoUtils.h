/**
  ******************************************************************************
  * @file           : DeviceInfoUtils.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/10
  ******************************************************************************
  */
#ifndef DEVICEINFOUTILS_H
#define DEVICEINFOUTILS_H

#include "yaml-cpp/yaml.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QStandardItemModel>

struct DeviceInfo {
    QString io_pin_count;
    QString iob_count;
    QString lut_count;
    QString ff_count;
    QString bram_count;
    QString dsp_count;
    QString pcie_count;
    QString archName;
    QString speedgrade;
    QString family_name;
    QString arch;
    QString part;
};

class DeviceInfoUtils
{
public:
    DeviceInfoUtils();

    DeviceInfo getDeviceInfo(const QString& displayPart) const;

    QStandardItemModel* getDeviceModel() const;

private:
    QMap<QString, DeviceInfo> deviceMap;

    const QString partFile = ":/resource/parts_hybrdchip_for_display.yaml";
};

#endif // DEVICEINFOUTILS_H
