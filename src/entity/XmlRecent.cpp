/**
  ******************************************************************************
  * @file           : XmlRecent.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#include "XmlRecent.h"

#include <utility>

XmlRecent::XmlRecent(int idx, const char *pth) : index(idx), path(pth) {
}

int XmlRecent::getIndex() const {
    return this->index;
}

void XmlRecent::setIndex(int idx) {
    this->index = idx;
}

const char* XmlRecent::getPath() const {
    return this->path;
}

void XmlRecent::setPath(const char *pth) {
    this->path = pth;
}