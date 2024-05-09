/**
  ******************************************************************************
  * @file           : XmlRecent.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#ifndef WORKSPACE_CPP_XMLRECENT_H
#define WORKSPACE_CPP_XMLRECENT_H

#include <string>

class XmlRecent {
private:
    int index;
    std::string path;
    std::string projectName;

public:
    XmlRecent(int idx, std::string pth);  // 构造函数
    int getIndex() const;
    void setIndex(int idx);
    std::string getPath() const;
    void setPath(std::string pth);
};
#endif //WORKSPACE_CPP_XMLRECENT_H
