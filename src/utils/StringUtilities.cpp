/**
  ******************************************************************************
  * @file           : StringUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/7
  ******************************************************************************
  */
#include "StringUtilities.h"
#include <vector>
#include <QString>
/**
 * 拼接paths中的字符串，用sep分割
 * @param paths 字符串列表
 * @param sep 分隔符 默认为"/"
 * @return 返回一个拼接后的字符串
 */
namespace StringUtilities {

    // 在命名空间内实现 concatPath 函数
    std::string concatPath(const std::vector<std::string>& paths, const std::string& sep){
        std::string totalPath;
        bool isFirst = true;
        for (const std::string& p : paths) {
            if (!isFirst) {
                totalPath += sep;
            }
            totalPath += p;
            isFirst = false;
        }
        return totalPath;
    }

}