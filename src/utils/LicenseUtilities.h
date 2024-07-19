/**
  ******************************************************************************
  * @file           : LicenseUtilities.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/7/18
  ******************************************************************************
  */
#ifndef HYBRDLINK_LICENSEUTILITIES_H
#define HYBRDLINK_LICENSEUTILITIES_H


#include <QString>
#include <QDateTime>
#include <QRegularExpression>
#include "dialog/LicenseDialog.h"


class LicenseUtilities {
public:
    static LicenseUtilities *instance();

    bool checkLicense();

private:
    int loadLicense();

    bool checkMacAddress(const QString& expectedMacAddress);

    int isWithinValidPeriod(const QString& compileDateTimeStr);

    QString getMacAddress();
};


#endif //HYBRDLINK_LICENSEUTILITIES_H
