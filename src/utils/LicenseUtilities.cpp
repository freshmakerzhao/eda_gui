/**
  ******************************************************************************
  * @file           : LicenseUtilities.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/7/18
  ******************************************************************************
  */
#include "LicenseUtilities.h"
#include "AESUtilities.h"
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QDate>

LicenseUtilities *LicenseUtilities::instance()
{
    static LicenseUtilities *_instance = nullptr;
    if (!_instance) {
        _instance = new LicenseUtilities;
    }
    return _instance;
}

bool LicenseUtilities::checkLicense() {
    while (true) {
        const int result = LicenseUtilities::instance()->loadLicense();
        if (result == 0) {
            return true;  // License check passed
        }

        LicenseDialog dialog(nullptr, result);
        if (dialog.exec() == QFileDialog::Rejected) {
            return false;  // User rejected the dialog
        }
    }
}

int LicenseUtilities::loadLicense()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString licensePath = QFileInfo(appDataPath).path() + "/HybrdChip/Common/license.lic";
    if (!QFileInfo(licensePath).isFile()) {
        // 证书不存在
        return -2;
    }
    const std::string str = licensePath.toStdString();
    qDebug() << "[LicenseUtilities] LicensePath: " <<licensePath;
    unsigned char* decrypt_text = AESUtilities::aesDecrypt(str);
    if (!decrypt_text) {
        // 证书解码失败
        qDebug() << "[LicenseUtilities] Decryption failed: null pointer returned.";
        return -1;
    }
    qDebug() << "[LicenseUtilities] Decode: " << (char*)decrypt_text;
    QString decryptText = QString((char*)decrypt_text);
    free(decrypt_text);

#ifdef ENABLE_MAC_CHECK
    static QRegularExpression macRegex("MAC_ADDRESS->([0-9A-Fa-f]{2}:[0-9A-Fa-f]{2}:[0-9A-Fa-f]{2}:[0-9A-Fa-f]{2}:[0-9A-Fa-f]{2}:[0-9A-Fa-f]{2})");
    QRegularExpressionMatch macMatch = macRegex.match(decryptText);
    QString macAddress;
    if (macMatch.hasMatch()) macAddress = macMatch.captured(1);

    if (!LicenseUtilities::checkMacAddress(macAddress)) {
        // MAC_ADDRESS检查失败
        return -3;
    }
#else
    qDebug() << "MAC address check is disabled.";
#endif

#ifdef ENABLE_EXPIRATION_CHECK
    int remainingDays = LicenseUtilities::isWithinValidPeriod(decryptText);
    if (remainingDays == -1) {
        // 证书过期
        return -4;
    }
#else
    qDebug() << Software expiration check is disabled.
#endif
    return 0;
}

QString LicenseUtilities::getMacAddress() {
    foreach (QNetworkInterface intf, QNetworkInterface::allInterfaces()) {
        // 判断接口是否为有效的网卡并且处于活动状态
        if (intf.flags().testFlag(QNetworkInterface::IsUp) &&
            intf.flags().testFlag(QNetworkInterface::IsRunning) &&
            !intf.hardwareAddress().isEmpty()) {
            return intf.hardwareAddress();
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
        return false;
    }
    // 校验成功
    qDebug() << "[LicenseUtilities] MAC address matches the expected value.";
    return true;
}

int LicenseUtilities::isWithinValidPeriod(const QString& compileDateTimeStr) {
    int daysDiff = 0;
    QDate expirationDate;
    static QRegularExpression regex("EXPIRATION->(\\d{4}\\.\\d{2}\\.\\d{2})");
    QRegularExpressionMatch match = regex.match(compileDateTimeStr);
    if (!match.hasMatch()) {
        return -1;
    }
    // 获取匹配的日期部分
    QString expirationDateStr = match.captured(1);
    expirationDate = QDate::fromString(expirationDateStr, "yyyy.MM.dd");
    if (!expirationDate.isValid()) {
        return -1;
    }
    // 获取当前日期
    QDate currentDate = QDate::currentDate();
    // 计算日期差值
    daysDiff = currentDate.daysTo(expirationDate);
    qDebug() << "Days to expiration:" << daysDiff;
    if (daysDiff <= 0) {
        return -1;
    }
    return daysDiff;
}

