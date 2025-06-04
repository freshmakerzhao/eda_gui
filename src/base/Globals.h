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

static int GLOBAL_TILE_BLOCK_WIDTH = 210;
static int GLOBAL_TILE_BLOCK_HEIGHT = 100;

static int GLOBAL_SITE_BLOCK_WIDTH = 90;
static int GLOBAL_SITE_BLOCK_HEIGHT = 80;

static std::string COMPRESS_TOOL_KEY = "lwh123456";

// 全局资源路径
namespace GlobalConfig {
    extern QString GLOBAL_RESOURCE_PATH;

    extern qreal SCALE_FACTOR;

    void initGlobalResourcePath();

    void initScaleFactor();
}

struct SITE;

struct TILE {
    struct Vertex {
        float x;
        float y;
    };

    Vertex topLeft;
    Vertex topRight;
    Vertex bottomLeft;
    Vertex bottomRight;

    std::string tile_name;

    float r = 48 / 255.0, g = 48 / 255.0, b = 48 / 255.0;

    // 初始化矩形顶点
    TILE(float pos_x, float pos_y, float tile_width, float tile_height) {
        topLeft = { pos_x, pos_y + tile_height };
        topRight = { pos_x + tile_width, pos_y + tile_height };
        bottomLeft = { pos_x, pos_y };
        bottomRight = { pos_x + tile_width, pos_y };
    }

    // 检查点是否在矩形内
    bool contains(float px, float py) const {
        return (px >= bottomLeft.x && px <= bottomRight.x &&
            py >= bottomLeft.y && py <= topLeft.y);
    }

    QVector<SITE> sites;
};

struct SITE {
    struct Vertex {
        float x;
        float y;
    };

    Vertex topLeft;
    Vertex topRight;
    Vertex bottomLeft;
    Vertex bottomRight;

    std::string site_name;

    // bool isSel = false;

    float r = 48 / 255.0, g = 48 / 255.0, b = 48 / 255.0;

    SITE() {}

    SITE(SITE const& site) {
        topLeft = site.topLeft;
        topRight = site.topRight;
        bottomLeft = site.bottomLeft;
        bottomRight = site.bottomRight;
        site_name = site.site_name;
        r = site.r;
        g = site.g;
        b = site.b;

        // isSel = site.isSel;
    }

    // 初始化矩形顶点
    SITE(float pos_x, float pos_y, float site_width, float site_height) {
        topLeft = { pos_x, pos_y + site_height };
        topRight = { pos_x + site_width, pos_y + site_height };
        bottomLeft = { pos_x, pos_y };
        bottomRight = { pos_x + site_width, pos_y };
    }

    // 检查点是否在矩形内
    bool contains(float px, float py) const {
        return (px >= bottomLeft.x && px <= bottomRight.x &&
            py >= bottomLeft.y && py <= topLeft.y);
    }
};

#endif //CHIP_GLOBALS_H
