/**
  ******************************************************************************
  * @file           : LicenseUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2/7/2024
  ******************************************************************************
  */
#include "LicenseUtilities.h"
#include <QCoreApplication>
#include <QNetworkInterface>

QString LicenseUtilities::getMacAddress() {
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        // 判断接口是否为有效的网卡并且处于活动状态
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.hardwareAddress().isEmpty()) {
            return interface.hardwareAddress();
        }
    }
    return QString();
}