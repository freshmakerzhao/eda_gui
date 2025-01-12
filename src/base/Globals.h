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

static int GLOBAL_TILE_BLOCK_WIDTH  = 210;
static int GLOBAL_TILE_BLOCK_HEIGHT = 100;

static int GLOBAL_SITE_BLOCK_WIDTH  = 90;
static int GLOBAL_SITE_BLOCK_HEIGHT = 80;

// 全局资源路径
namespace GlobalConfig {
    extern QString GLOBAL_RESOURCE_PATH;

    extern qreal SCALE_FACTOR;

    void initGlobalResourcePath();

    void initScaleFactor();
}

#endif //CHIP_GLOBALS_H
