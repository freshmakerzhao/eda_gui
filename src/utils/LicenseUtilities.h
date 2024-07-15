/**
  ******************************************************************************
  * @file           : LicenseUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2/7/2024
  ******************************************************************************
  */
#ifndef HYBRDLINK_LICENSEUTILITIES_H
#define HYBRDLINK_LICENSEUTILITIES_H


#include <QString>
#include <QDateTime>

class LicenseUtilities {
public:
    static bool checkMacAddress(const QString& expectedMacAddress);
    static int isWithinValidPeriod(const QString& compileDateTimeStr, int validDays = 7);
    static void writeExpiryDateToFile(const QString& expiryDate);
private:
    static QString getMacAddress();
};


#endif //HYBRDLINK_LICENSEUTILITIES_H
