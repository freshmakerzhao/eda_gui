/**
  ******************************************************************************
  * @file           : InitialConfig.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#ifndef HYBRDLINK_INITIALCONFIG_H
#define HYBRDLINK_INITIALCONFIG_H


#include <QString>
#include <QSettings>

class InitialConfig {

public:
    InitialConfig();
    void initializeApplicationConfig();
    void initializeRoamingPath();
    static InitialConfig& instance();
    // "C:/Users/xxx/AppData/Roaming/HybrdChip/HybrdLink"
    QString roamingPath;
    // roaming xml文件
    QString xmlPath;
    // 常规配置路径
    QString commonPath;
    // 应用路径
    QString hybrdPath;
    // 版本路径
    QString versionPath;
    // 进程id
    QString pid_str;
};


#endif //HYBRDLINK_INITIALCONFIG_H
