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


XmlRecent::XmlRecent(int idx, std::string pth) : index(idx), path(pth) {
}

int XmlRecent::getIndex() const {
    return this->index;
}

void XmlRecent::setIndex(int idx) {
    this->index = idx;
}

std::string XmlRecent::getPath() const {
    return this->path;
}

void XmlRecent::setPath(std::string pth) {
    this->path = pth;
}