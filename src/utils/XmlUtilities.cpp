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
 * 插入recent，index越往下越新，最多不超过 expectRecentLength
 * @param xmlPath xml地址
 * @param fatherElementName recent父元素地址
 * @param recentList recent列表，待插入数据
 * @return 插入状态
 */
bool XmlUtilities::insertHybrdLinkXmlRecent(
        const char *xmlPath,
        const char *fatherElementName,
        std::vector<XmlRecent> recentList) {
    int expectRecentLength = 10;
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

    // 存储所有recent
    std::vector<XmlRecent> recentAllList;
    // 读取 fatherElementNode 下所有的recent，存入list
    tinyxml2::XMLElement* recentFile = fatherElementNode->FirstChildElement("recent");
    while (recentFile != nullptr) {
        int index;
        const char* path;
        eResult = recentFile->QueryIntAttribute("index", &index);
        if (eResult != tinyxml2::XML_SUCCESS) {
            // 读取失败
            return false;
        }
        // 读取path属性，如果为空则读取失败
        path = recentFile->Attribute("path");
        if (path == nullptr) {
            return false;
        }
        recentAllList.emplace_back(index,path);
        // 查询下一个recent
        recentFile = recentFile->NextSiblingElement("recent");
    }

    // 路径标准化
    for (auto & recent : recentList) {
        recent.setPath(normalizePathSeparators(recent.getPath()));
    }

    // 合并前去重
    // 使用迭代器进行循环，安全删除元素
    for (auto & it1 : recentList) {
        for (auto it2 = recentAllList.begin(); it2 != recentAllList.end();) {
            if (it1.getPath() == it2->getPath()) {
                // 使用 erase 删除元素，并更新迭代器 it2
                it2 = recentAllList.erase(it2);
            } else {
                // 不删除元素时,移动迭代器
                ++it2;
            }
        }
    }

    // 合并待插入数据
    recentAllList.insert(recentAllList.end(),recentList.begin(),recentList.end());

    // 删除原有数据
    generalNode->DeleteChild(fatherElementNode);

    // 创建一个新的空的 newFatherNode
    tinyxml2::XMLElement* newFatherNode = xml.NewElement(fatherElementName);
    generalNode->InsertEndChild(newFatherNode);

    unsigned recentAllListSize = recentAllList.size();

    if (recentAllListSize > expectRecentLength-1){
        // 超过9，则取后10个
        // 从前往后移除
        recentAllList.erase(recentAllList.begin(), recentAllList.begin() + recentAllListSize - expectRecentLength);
    }
    // 根据最后一个节点的index值，更新待插入recent
    int index = 0;

    for (const XmlRecent& recent : recentAllList) {
        // 创建一个新的 recent 元素并设置属性
        tinyxml2::XMLElement* newRecent = xml.NewElement("recent");
        newRecent->SetAttribute("index", index);
        newRecent->SetAttribute("path", recent.getPath().c_str());
        // 添加新的 recent 元素到 newFatherNode 元素
        newFatherNode->InsertEndChild(newRecent);
        ++index;
    }

    // 保存修改后的 XML 文件
    eResult = xml.SaveFile(xmlPath);
    if (eResult != tinyxml2::XML_SUCCESS) {
        qDebug() << "[XmlUtilities] Failed to save";
        return false;
    }
    return true;
}

std::vector<XmlRecent> XmlUtilities::getRecentListFromFatherElementName(
        const char *xmlPath,
        const char *fatherElementName) {
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile(xmlPath);
    // 存储所有recent
    std::vector<XmlRecent> recentAllList;
    if(eResult != tinyxml2::XML_SUCCESS) {
        qDebug() << "[XmlUtilities] load xml file failed";
        return recentAllList;
    }

    //------------
    //找到导入的xml的根节点
    //------------
    tinyxml2::XMLElement* rootNode = xml.RootElement();
    if (rootNode == nullptr) {
        return recentAllList;
    }

    //------------
    //读取根节点下的 general 节点信息
    //------------
    tinyxml2::XMLElement* generalNode = rootNode->FirstChildElement("general");
    if (generalNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find 'general' element.";
        return recentAllList;
    }

    //------------
    //读取general节点下的 fatherElementName 节点信息
    //------------
    tinyxml2::XMLElement* fatherElementNode = generalNode->FirstChildElement(fatherElementName);
    if (fatherElementNode == nullptr) {
        qDebug() << "[XmlUtilities] Failed to find '" << fatherElementName << "' element.";
        return recentAllList;
    }

    // 读取 fatherElementNode 下所有的recent，存入list
    tinyxml2::XMLElement* recentFile = fatherElementNode->FirstChildElement("recent");
    while (recentFile != nullptr) {
        int index;
        const char* path;
        eResult = recentFile->QueryIntAttribute("index", &index);
        if (eResult != tinyxml2::XML_SUCCESS) {
            // 读取失败
            std::reverse(recentAllList.begin(),recentAllList.end());
            return recentAllList;
        }
        // 读取path属性，如果为空则读取失败
        path = recentFile->Attribute("path");
        if (path == nullptr) {
            std::reverse(recentAllList.begin(),recentAllList.end());
            return recentAllList;
        }
        recentAllList.emplace_back(index,path);
        // 查询下一个recent
        recentFile = recentFile->NextSiblingElement("recent");
    }
    std::reverse(recentAllList.begin(),recentAllList.end());
    return recentAllList;
}

/**
 * 比较两个路径是否指向同一个位置
 * @param path1
 * @param path2
 * @return
 */
bool XmlUtilities::isSamePath(std::string path1, std::string path2){
    // 将路径中的 '\\' 转为 '/'，统一分隔符后进行比较
    std::transform(path1.begin(), path1.end(), path1.begin(),
                   [](char c) { return (c == '\\') ? '/' : c; });
    std::transform(path2.begin(), path2.end(), path2.begin(),
                   [](char c) { return (c == '\\') ? '/' : c; });

    // 转小写，忽略大小写
    std::transform(path1.begin(), path1.end(), path1.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::transform(path2.begin(), path2.end(), path2.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return path1 == path2;
}

/**
 * 将路径中的右斜替换成左斜，统一分隔符。
 * @param path
 * @return
 */
std::string XmlUtilities::normalizePathSeparators(std::string path){
    // 将路径中的 '\\' 转为 '/'，统一分隔符后进行比较
    std::transform(path.begin(), path.end(), path.begin(),
                   [](char c) { return (c == '\\') ? '/' : c; });
    return path;
}