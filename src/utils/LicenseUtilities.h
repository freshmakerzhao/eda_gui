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

class LicenseUtilities {
public:
    static bool checkLicense();

    static QString getLicensePath();

private:
    static int loadLicense();

    static bool checkMacAddress(const QString& expectedMacAddress);

    static int isWithinValidPeriod(const QString& compileDateTimeStr);

    static QString getMacAddress();
};


#endif //HYBRDLINK_LICENSEUTILITIES_H
