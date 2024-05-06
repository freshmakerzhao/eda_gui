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


class InitialConfig {

public:
    static void initializeApplicationConfig();
    void initializeRoamingPath();
    static InitialConfig& instance();

};


#endif //HYBRDLINK_INITIALCONFIG_H
