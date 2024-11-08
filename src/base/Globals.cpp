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

void GlobalConfig::initGlobalResourcePath() {
    // 打包用
    const QString PACK_PATH = QString::fromStdString(StringUtilities::concatPath({QCoreApplication::applicationDirPath().toStdString(), "resource_win"}));

    // 测试用
    const QString TEST_PATH1 = "E:/workspace/HybrdLink_test/resource_win";
    const QString TEST_PATH2 = "C:/HybrdLink/resource_win";
    const QString TEST_PATH3 = "C:/Users/INTEL/Desktop/Work/VMwareFileWorkspace/HybrdLink/resource_win";

    QFileInfo fileInfo1(TEST_PATH1);
    QFileInfo fileInfo2(TEST_PATH2);
    QFileInfo fileInfo3(TEST_PATH3);
    QFileInfo fileInfo4(PACK_PATH);

    if(fileInfo1.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH1;
    } else if (fileInfo2.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH2;
    } else if (fileInfo3.exists()) {
        GLOBAL_RESOURCE_PATH = TEST_PATH3;
    } else if (fileInfo4.exists()) {
        GLOBAL_RESOURCE_PATH = PACK_PATH;
    }

#ifdef PACK_DEPLOYED
    GLOBAL_RESOURCE_PATH = PACK_PATH;
#endif

    qDebug() << GLOBAL_RESOURCE_PATH;
}
