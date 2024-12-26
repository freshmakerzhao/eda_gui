/**
  ******************************************************************************
  * @File           : Global.h
  * @Author         : zs
  * @Description    : 声明全局参数
  * @Date           : 23-11-29
  ******************************************************************************
  */

#ifndef CHIP_GLOBALS_H
#define CHIP_GLOBALS_H
#include <QString>
#include <QFileInfo>
#include <QCoreApplication>
#include <QApplication>
#include <QScreen>
#include <QDebug>

static int SITE_NUB_ON_TILE = 2;
static int GLOBAL_TILE_BLOCK_WIDTH  = 2100;
static int GLOBAL_TILE_BLOCK_HEIGHT = 1000;

static int SITE_GAP = 100;
static int GLOBAL_SITE_BLOCK_WIDTH  = (GLOBAL_TILE_BLOCK_WIDTH - (SITE_NUB_ON_TILE+1) * SITE_GAP) / SITE_NUB_ON_TILE;
static int GLOBAL_SITE_BLOCK_HEIGHT = GLOBAL_TILE_BLOCK_HEIGHT - 2 * SITE_GAP;

// 全局资源路径
namespace GlobalConfig {
    extern QString GLOBAL_RESOURCE_PATH;

    extern qreal SCALE_FACTOR;

    void initGlobalResourcePath();

    void initScaleFactor();
}

#endif //CHIP_GLOBALS_H
