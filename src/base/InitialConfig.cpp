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
#include "Globals.h"
#include "utils/StringUtilities.h"

InitialConfig &InitialConfig::instance()
{
    static InitialConfig instance;
    return instance;
}

void InitialConfig::initializeApplicationConfig() {
    QCoreApplication::setApplicationName("HybrdLink");
    QCoreApplication::setApplicationVersion("v1.0.0-beta");
    QCoreApplication::setOrganizationName("HybrdChip");
    QCoreApplication::setOrganizationDomain("www.hybrdchip.com");

//    // 测试用
//    QString TEST_PATH1 = "E:/workspace/hybrdlink/resource_win";
//    QString TEST_PATH2 = "C:/HybrdLink/resource_win";
//    QString TEST_PATH3 = "C:/Users/INTEL/Desktop/Work/VMwareFileWorkspace/HybrdLink/resource_win";
//    // 打包用
//    QString PACK_PATH = QString::fromStdString(StringUtilities::concatPath({QCoreApplication::applicationDirPath().toStdString(), "resource_win"}));
//
//    QFileInfo fileInfo1(TEST_PATH1);
//    QFileInfo fileInfo2(TEST_PATH2);
//    QFileInfo fileInfo3(TEST_PATH3);
//    QFileInfo fileInfo4(PACK_PATH);
//
//    if(fileInfo1.exists()) {
//        GLOBAL_RESOURCE_PATH = TEST_PATH1;
//    } else if (fileInfo2.exists()) {
//        GLOBAL_RESOURCE_PATH = TEST_PATH2;
//    } else if (fileInfo3.exists()) {
//        GLOBAL_RESOURCE_PATH = TEST_PATH3;
//    } else if (fileInfo4.exists()) {
//        GLOBAL_RESOURCE_PATH = PACK_PATH;
//    }
    qDebug() << GLOBAL_RESOURCE_PATH;
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
