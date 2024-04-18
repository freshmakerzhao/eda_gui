/**
  ******************************************************************************
  * @file           : TimeUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/22
  ******************************************************************************
  */
#include "TimeUtilities.h"
#include <iomanip>
#include <chrono>
#include <ctime>
#include <QString>

namespace TimeUtilities {

    // 获取当前时间点
    QString getCurTimeAndFormat(){
        // 获取当前时间点
        auto now = std::chrono::system_clock::now();
        // 转换为time_t，然后转换为tm结构
        std::time_t now_t = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_t);

        // 使用ostringstream和put_time进行格式化
        std::ostringstream oss;
        oss << std::put_time(now_tm, "%m/%d/%y, %I:%M %p");

        // 将格式化的时间转换为string
        std::string formatted_time = oss.str();

        return QString::fromStdString(formatted_time);
    }

    // 获取当前时间点
    std::chrono::system_clock::time_point getCurTime(){
        return std::chrono::system_clock::now();
    }


    // 将秒数格式化为HH:MM:SS字符串
    QString formatTime(int seconds) {
        std::ostringstream oss;
        std::chrono::seconds sec(seconds);
        std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(sec);
        sec -= hours;
        std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(sec);
        sec -= minutes;

        oss << std::setfill('0') << std::setw(2) << hours.count() << ":"
            << std::setfill('0') << std::setw(2) << minutes.count() << ":"
            << std::setfill('0') << std::setw(2) << sec.count();
        return QString::fromStdString(oss.str());
    }

    // 计算两个时间点之间的差异，并返回格式化的字符串
    QString calculateTimeDifference(const std::chrono::system_clock::time_point& time1,
                                        const std::chrono::system_clock::time_point& time2) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(time2 - time1).count();
        return formatTime(static_cast<int>(diff));
    }
}