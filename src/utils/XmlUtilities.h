/**
  ******************************************************************************
  * @file           : XmlUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#ifndef HYBRDLINK_XMLUTILITIES_H
#define HYBRDLINK_XMLUTILITIES_H

#include <vector>
#include "tinyxml2/tinyxml2.h"
#include "entity/XmlRecent.h"

class XmlUtilities {

public:
    static XmlUtilities& instance();

    void createXml(const std::string xmlPath);

    /**
     * 一键清空 fatherElementName 下的所有信息
     * @param xmlPath
     * @param fatherElementName
     * @return
     */
    bool clearNodesFromFatherElementName(
            const char *xmlPath,
            const char *fatherElementName
    );

    /**
     * 获取node下最后一个节点的index
     * @param node
     * @return
     */
    int getLastIndexFromNode(tinyxml2::XMLElement* node);

    /**
     * 插入recent
     * @param xmlPath xml地址
     * @param fatherElementName recent父元素地址
     * @param recentList recent列表，待插入数据
     * @return 插入状态
     */
    bool insertHybrdLinkXmlRecent(
            const char *xmlPath,
            const char *fatherElementName,
            const std::vector<XmlRecent>& recentList
    );

    /**
     * 获取recent列表，从新到旧，已反转
     * @param xmlPath xml地址
     * @param fatherElementName recent父元素地址
     * @return recent列表，从新到旧
     */
    std::vector<XmlRecent> getRecentListFromFatherElementName(
            const char *xmlPath,
            const char *fatherElementName);
};


#endif //HYBRDLINK_XMLUTILITIES_H
