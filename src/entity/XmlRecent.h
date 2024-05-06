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
    const char *path;

public:
    XmlRecent(int idx, const char *pth);  // 构造函数
    int getIndex() const;
    void setIndex(int idx);
    const char* getPath() const;
    void setPath(const char *pth);
};

#endif //WORKSPACE_CPP_XMLRECENT_H
