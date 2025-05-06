/**
  ******************************************************************************
  * @file           : TimeUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/22
  ******************************************************************************
  */
#ifndef HYBRDLINK_TIMEUTILITIES_H
#define HYBRDLINK_TIMEUTILITIES_H


#include <QString>
#include <chrono>
#include <sstream>

namespace TimeUtilities {

    /**
     * 获取当前时间
     * @return 返回当前时间
     */
    std::chrono::system_clock::time_point getCurTime();

    /**
     * 返回当前时间并格式化
     * @return  3/22/24, 7:44 PM
     */
    QString getCurTimeAndFormat();

    /**
     * 将秒数格式化为HH:MM:SS字符串
     * @return  00:00:01
     */
    QString formatTime(int seconds);

    /**
     * 计算两个时间点之间的差异，并返回格式化的字符串
     * @return  00:00:01
     */
    QString calculateTimeDifference(const std::chrono::system_clock::time_point& time1,
                                        const std::chrono::system_clock::time_point& time2);
}

#endif //HYBRDLINK_TIMEUTILITIES_H
