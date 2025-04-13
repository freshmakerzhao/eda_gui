/**
  ******************************************************************************
  * @file           : Globals.cpp
  * @author         : zs
  * @description      : None
  * @date           : 21/12/2023
  ******************************************************************************
  */
#include "Globals.h"
#include "utils/StringUtilities.h"

//// 架构映射文件路径
//std::string GLOBAL_TILE_INFO_MAP_PATH;
//// 架构文件路径
//std::string GLOBAL_TILE_GRID_PATH;
//// part信息
//std::string GLOBAL_PART_NAME;
//// family信息
//std::string GLOBAL_FAMILY_NAME;
//// arch name
//std::string GLOBAL_ARCH_NAME;

QString GlobalConfig::GLOBAL_RESOURCE_PATH = "";
qreal GlobalConfig::SCALE_FACTOR = 1.0f;

void GlobalConfig::initGlobalResourcePath() {

#ifdef RESOURCE_WIN_PATH
    // 通过编译选项设置资源路径
    GLOBAL_RESOURCE_PATH = QString::fromStdString(RESOURCE_WIN_PATH);
#else
    // 使用默认路径
    GLOBAL_RESOURCE_PATH = QString::fromStdString(StringUtilities::concatPath({ QCoreApplication::applicationDirPath().toStdString(), "resource_win" }));
#endif

#ifdef PACK_DEPLOYED
    // 打包模式下，resource_win的路径固定为根目录下的 “resource_win”
    GLOBAL_RESOURCE_PATH = QString::fromStdString(StringUtilities::concatPath({ QCoreApplication::applicationDirPath().toStdString(), "resource_win" }));
#endif
    qDebug() << "[Globals] GLOBAL_RESOURCE_PATH " << GLOBAL_RESOURCE_PATH;
}

void GlobalConfig::initScaleFactor()
{
    try {
        QScreen *screen = QApplication::primaryScreen();
        qreal dpi = screen->logicalDotsPerInch();
        SCALE_FACTOR = dpi / 96.0;
        qDebug() << "Scale Factor: " << SCALE_FACTOR;
    } catch (...) {
        qDebug() << "Scale Factor calculation failed.";
    }
}
