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
#include "Globals.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

InitialConfig &InitialConfig::instance()
{
    static InitialConfig instance;
    return instance;
}

InitialConfig::InitialConfig()
{
    GlobalConfig::initGlobalResourcePath();
    QSettings settings("HybrdChip", "HybrdLink");
    if (!settings.contains("TextEditor/encoding")) {
        settings.setValue("TextEditor/encoding", "UTF-8");
    }
}

void InitialConfig::initializeApplicationConfig() {
    QCoreApplication::setApplicationName("HybrdLink");
    QCoreApplication::setApplicationVersion("v1.3.2-beta");
    QCoreApplication::setOrganizationName("HybrdChip");
    QCoreApplication::setOrganizationDomain("www.hybrdchip.com");

    qint64 pid = QCoreApplication::applicationPid();
    this->pid_str = QString::number(pid);
    qDebug() << "Current Process ID (PID):" << this->pid_str;
}

void InitialConfig::initializeRoamingPath() {
    // =================== 创建数据文件夹 ===================
    // 获取 AppData 路径
    // "C:/Users/xxx/AppData/Roaming/HybrdChip/HybrdLink"
    this->roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    int pos = this->roamingPath.lastIndexOf("HybrdLink");
    if (pos != -1) {
        this->roamingPath.remove(pos, QString("HybrdLink").length());
    }
    // 常规配置路径
    this->commonPath = QDir(this->roamingPath).filePath("Common");
    // 应用路径
    this->hybrdPath = QDir(this->roamingPath).filePath(QCoreApplication::applicationName());
    // 版本路径
    this->versionPath = QDir(this->hybrdPath).filePath(QCoreApplication::applicationVersion());
    // 确保目录存在,如果不存在，创建路径
    FileHelper::ensureDirectoryExists(this->roamingPath);
    FileHelper::ensureDirectoryExists(this->commonPath);
    FileHelper::ensureDirectoryExists(this->hybrdPath);
    FileHelper::ensureDirectoryExists(this->versionPath);

    // =================== 创建xml文件 ===================

    // xml路径
    this->xmlPath = QDir(versionPath).filePath("hybrdlink.xml");
    if (!FileHelper::fileExists(xmlPath.toStdString())){
        // 文件不存在则创建
        XmlUtilities::instance().createXml(this->xmlPath.toStdString());
    }
    qDebug() << "[InitialConfig] Initial xml" << this->xmlPath;
}
