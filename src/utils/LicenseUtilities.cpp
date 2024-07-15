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
#include "base/Globals.h"
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QDate>
#include <cmath>
#include <QFile>

QString LicenseUtilities::getMacAddress() {
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        // 判断接口是否为有效的网卡并且处于活动状态
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.hardwareAddress().isEmpty()) {
            return interface.hardwareAddress();
        }
    }
    return {};
}

bool LicenseUtilities::checkMacAddress(const QString& expectedMacAddress) {
    // 获取 mac 地址
    QString localMacAddress = getMacAddress();
    qDebug() << "[LicenseUtilities] Local MAC Address:" << localMacAddress;
    // 判读 mac 地址是否一致
    if (localMacAddress != expectedMacAddress) {
        qDebug() << "[LicenseUtilities] Error: MAC address does not match the expected value.";
        // 校验失败
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Error: Software activation failed.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }
    // 校验成功
    qDebug() << "[LicenseUtilities] MAC address matches the expected value.";
    return true;
}

int LicenseUtilities::isWithinValidPeriod(const QString& compileDateTimeStr, int validDays) {
    // 解析 compileDateTimeStr
    QDateTime compileDateTime = QLocale::c().toDateTime(compileDateTimeStr, "MMM  d yyyy HH:mm:ss");
    // 获取当前系统时间
    QDateTime curDateTime = QDateTime::currentDateTime();

    // compileDateTime 转换失败
    if (!compileDateTime.isValid()) {
        qWarning() << "[LicenseUtilities] Invalid compile date-time format.";
        return 0;
    }
    // 计算 validDays 后，的时间
    QDateTime expiryDateTime = compileDateTime.addDays(validDays);
    if (curDateTime > expiryDateTime) {
        // 已过期
        return 0;
    } else {
        // 未过期，计算差值
        // 计算差值（以秒为单位），然后转换为天数
        qint64 secondsToExpiry = curDateTime.secsTo(expiryDateTime); // 得到秒数
        // 计算天数，向上取整那，如0.1为1
        int daysToExpiry = static_cast<int>(std::ceil(static_cast<double>(secondsToExpiry) / (24 * 3600)));
        return daysToExpiry;
    }
}

void LicenseUtilities::writeExpiryDateToFile(const QString& expiryDate) {
    QFile file("expiry_date.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8"); // 设置编码为 UTF-8
        out << expiryDate << "\n";
        file.close();
    } else {
        qWarning() << "[LicenseUtilities] Failed to write expiry date to file.";
    }
}
