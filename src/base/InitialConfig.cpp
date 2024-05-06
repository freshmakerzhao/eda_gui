/**
  ******************************************************************************
  * @file           : InitialConfig.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#include "InitialConfig.h"
#include "utils/FileHelper.h"
#include "utils/XmlUtilities.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

InitialConfig &InitialConfig::instance()
{
    static InitialConfig instance;
    return instance;
}

void InitialConfig::initializeApplicationConfig() {
    QCoreApplication::setApplicationName("HybrdLink");
    QCoreApplication::setApplicationVersion("2024.0.1.3");
    QCoreApplication::setOrganizationName("HybrdChip");
    QCoreApplication::setOrganizationDomain("www.hybrdchip.com");
}

void InitialConfig::initializeRoamingPath() {
    // =================== 创建数据文件夹 ===================
    // 获取 AppData 路径
    // "C:/Users/xxx/AppData/Roaming/HybrdChip/HybrdLink"
    QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    int pos = roamingPath.lastIndexOf("HybrdLink");
    if (pos != -1) {
        roamingPath.remove(pos, QString("HybrdLink").length());
    }
    // 常规配置路径
    QString commonPath = QDir(roamingPath).filePath("Common");
    // 应用路径
    QString hybrdPath = QDir(roamingPath).filePath(QCoreApplication::applicationName());
    // 版本路径
    QString versionPath = QDir(hybrdPath).filePath(QCoreApplication::applicationVersion());
    // 确保目录存在,如果不存在，创建路径
    FileHelper::ensureDirectoryExists(roamingPath);
    FileHelper::ensureDirectoryExists(commonPath);
    FileHelper::ensureDirectoryExists(hybrdPath);
    FileHelper::ensureDirectoryExists(versionPath);

    // =================== 创建xml文件 ===================

    // xml路径
    QString xmlPath = QDir(versionPath).filePath("hybrdlink.xml");
    if (!FileHelper::fileExists(xmlPath.toStdString())){
        // 文件不存在则创建
        XmlUtilities::instance().createXml(xmlPath.toStdString());
    }
    qDebug() << "[InitialConfig] Initial xml" << xmlPath;
}
