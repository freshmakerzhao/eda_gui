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
#include <string>
#include <map>
#include <QString>

static int GLOBAL_TILE_BLOCK_WIDTH  = 210;
static int GLOBAL_TILE_BLOCK_HEIGHT = 100;

static int GLOBAL_SITE_BLOCK_WIDTH  = 90;
static int GLOBAL_SITE_BLOCK_HEIGHT = 80;

// PART_TO_ARCH
static std::map<std::string,std::string> GLOBAL_PART_TO_ARCH = {
        {"xc7a35tcsg324-1", "50t"},
        {"xc7a35tfgg484-2", "50t"},
        {"xc7a100tcsg324-1", "100t"},
        {"xc7a100tfgg484-1", "100t"},
        {"xc7a100tfgg484-2", "100t"}
};

// 全局资源路径
static QString GLOBAL_RESOURCE_PATH = "E:/workspace/HybrdLink_test/resource_win";
//// 架构映射文件路径
//extern std::string GLOBAL_TILE_INFO_MAP_PATH;
//// 架构文件路径
//extern std::string GLOBAL_TILE_GRID_PATH;
//// part_name
//extern std::string GLOBAL_PART_NAME;
//// family_name
//extern std::string GLOBAL_FAMILY_NAME;
//// arch_name
//extern std::string GLOBAL_ARCH_NAME;
#endif //CHIP_GLOBALS_H
