/**
  ******************************************************************************
  * @file           : XmlUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/6
  ******************************************************************************
  */
#include <QDebug>
#include "XmlUtilities.h"

XmlUtilities &XmlUtilities::instance()
{
    static XmlUtilities instance;
    return instance;
}

void XmlUtilities::createXml(const std::string xmlPath){
    //------------
    //声明要创建的xml文件
    //------------
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLDeclaration* declaration = xml.NewDeclaration();
    xml.InsertFirstChild(declaration);

    //------------
    //创建根节点
    //------------
    tinyxml2::XMLElement* rootNode = xml.NewElement("preferences");
    xml.InsertEndChild(rootNode);

    //------------
    //为 rootNode 插入 general 节点
    //------------
    tinyxml2::XMLElement* generalNode = xml.NewElement("general");
    rootNode->InsertEndChild(generalNode);

    //------------
    //为 rootNode 插入 RECENT_FILES RECENT_DIRECTORIES RECENT_PROJECTS 节点
    //------------
    tinyxml2::XMLElement* recentFilesNode = xml.NewElement("RECENT_FILES");
    tinyxml2::XMLElement* recentDirectionsNode = xml.NewElement("RECENT_DIRECTORIES");
    tinyxml2::XMLElement* recentProjectsNode = xml.NewElement("RECENT_PROJECTS");
    generalNode->InsertEndChild(recentFilesNode);
    generalNode->InsertEndChild(recentDirectionsNode);
    generalNode->InsertEndChild(recentProjectsNode);

    //------------
    //将xml保存到当前项目中
    //------------
    xml.SaveFile(xmlPath.c_str());
}

/**
 * 一键清空 fatherElementName 下的所有信息
 * @param xmlPath
 * @param fatherElementName
 * @return
 */
bool XmlUtilities::clearNodesFromFatherElementName(
        const char *xmlPath,
        const char *fatherElementName) {

    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile(xmlPath);
    if(eResult != tinyxml2::XML_SUCCESS) {
        qDebug() << "[XmlUtilities] load xml file failed";
        return false;
    }

    //------------
    //找到导入的xml的根节点
    //------------
    tinyxml2::XMLElement* rootNode = xml.RootElement();
    if (rootNode == nullptr) {
        return false;
    }

    //------------
    //读取根节点下的 general 节点信息
    //------------
    tinyxml2::XMLElement* generalNode = rootNode->FirstChildElement("general");
    if (generalNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find 'general' element.";
        return false;
    }

    //------------
    //读取general节点下的 fatherElementName 节点信息
    //------------
    tinyxml2::XMLElement* fatherElementNode = generalNode->FirstChildElement(fatherElementName);
    if (fatherElementNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find '" << fatherElementName << "' element.";
        return false;
    } else {
        // 存在则删除
        generalNode->DeleteChild(fatherElementNode);

        // 创建一个新的空的 RECENT_FILES 元素
        tinyxml2::XMLElement* newElementNode = xml.NewElement(fatherElementName);
        generalNode->InsertEndChild(newElementNode);

        eResult = xml.SaveFile(xmlPath);
        if (eResult != tinyxml2::XML_SUCCESS) {
            qDebug() << "[XmlUtilities] Error saving file!\n";
            return false;
        }
        return true;
    }
}

/**
 * 获取node下最后一个节点的index
 * @param node
 * @return
 */
int XmlUtilities::getLastIndexFromNode(tinyxml2::XMLElement* node){
    // 获取当前节点最后一个子节点
    tinyxml2::XMLElement* lastNode = node->LastChildElement();
    int index = -1;
    if (lastNode == nullptr){
        // 为空则返回-1
        return -1;
    }
    tinyxml2::XMLError eResult = lastNode->QueryIntAttribute("index", &index);
    if (eResult != tinyxml2::XML_SUCCESS) {
        // 读取失败
        qDebug() << "[XmlUtilities] Error reading index.";
        return -1;
    }
    return index;
}

/**
 * 插入recent
 * @param xmlPath xml地址
 * @param fatherElementName recent父元素地址
 * @param recentList recent列表，待插入数据
 * @return 插入状态
 */
bool XmlUtilities::insertHybrdLinkXmlRecent(
        const char *xmlPath,
        const char *fatherElementName,
        const std::vector<XmlRecent>& recentList) {
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile(xmlPath);
    if(eResult != tinyxml2::XML_SUCCESS) {
        qDebug() << "[XmlUtilities] load xml file failed";
        return false;
    }

    //------------
    //找到导入的xml的根节点
    //------------
    tinyxml2::XMLElement* rootNode = xml.RootElement();
    if (rootNode == nullptr) {
        return false;
    }

    //------------
    //读取根节点下的 general 节点信息
    //------------
    tinyxml2::XMLElement* generalNode = rootNode->FirstChildElement("general");
    if (generalNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find 'general' element.";
        return false;
    }

    //------------
    //读取general节点下的 fatherElementName 节点信息
    //------------
    tinyxml2::XMLElement* fatherElementNode = generalNode->FirstChildElement(fatherElementName);
    if (fatherElementNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find '" << fatherElementName << "' element.";
        return false;
    }

    // 根据最后一个节点的index值，更新待插入recent
    int lastIndex = getLastIndexFromNode(fatherElementNode);
    if (lastIndex + recentList.size() > 9){
        // Todo:如果加上插入内容大于10
        // 计算多余部分
        // 其他位置的index相应变化
        // 修改lastIndex
        lastIndex = 0;
    }

    for (XmlRecent recent : recentList) {
        ++lastIndex;
        // 创建一个新的 recent 元素并设置属性
        tinyxml2::XMLElement* newRecent = xml.NewElement("recent");
        newRecent->SetAttribute("index", lastIndex);
        newRecent->SetAttribute("path", recent.getPath());
        // 添加新的 recent 元素到 RECENT_FILES 元素
        fatherElementNode->InsertEndChild(newRecent);
    }

    // 保存修改后的 XML 文件
    eResult = xml.SaveFile(xmlPath);
    if (eResult != tinyxml2::XML_SUCCESS) {
        qDebug() << "[XmlUtilities] Failed to save\n";
        return false;
    }
    return true;
}