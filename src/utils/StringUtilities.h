/**
  ******************************************************************************
  * @file           : StringUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/7
  ******************************************************************************
  */
#ifndef EDA_GUI_STRINGUTILITIES_H
#define EDA_GUI_STRINGUTILITIES_H

#include <QString>
#include <vector>
namespace StringUtilities {

    /**
     * 拼接paths中的字符串，用sep分割
     * @param paths 字符串列表
     * @param sep 分隔符 默认为空
     * @return 返回一个拼接后的字符串
     */
    std::string concatPath(const std::vector<std::string> &paths, const std::string& sep = "/");
}
#endif //EDA_GUI_STRINGUTILITIES_H
