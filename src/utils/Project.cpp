/**
  ******************************************************************************
  * @file           : Project.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/8
  ******************************************************************************
  */
#include "Project.h"

Project::Project()
{
    qDebug() << "[Project] Constructing...";
}

/**
 * 初始化工程参数，仅在新建工程时使用
 * @param name
 * @param path
 * @param part
 * @param arch
 * @param archName
 */
void Project::initProject(QString &name,
                          QString &path,
                          QString &part,
                          QString &arch,
                          QString &archName)
{
    param["name"] = name;          // 工程名称
    param["path"] = path;          // 工程路径(绝对)
    param["part"] = part;
    param["arch"] = arch;
    param["archName"] = archName;
    param["top"] = "top";
}

bool Project::writeProject()
{
    QString hprPath = QString("%1/%2.hpr").arg(param["path"], param["name"]);
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);
    // 版本注释
    tinyxml2::XMLComment* versionComment = doc.NewComment("               Product Version: HybrdLink v2024.1                   ");
    doc.InsertEndChild(versionComment);
    // 版权注释
    tinyxml2::XMLComment* copyrightComment = doc.NewComment(" © Copyright 2024 Zhongke Xin Magnetic Technology(Zhuhai) Co., Ltd. ");
    doc.InsertEndChild(copyrightComment);
    // Root
    tinyxml2::XMLElement* project = doc.NewElement("Project");
    doc.InsertEndChild(project);

    tinyxml2::XMLElement* configuration = doc.NewElement("Configuration");
    project->InsertEndChild(configuration);

    tinyxml2::XMLElement* prjNameOption = doc.NewElement("Option");
    prjNameOption->SetAttribute("Name", "PrjName");
    prjNameOption->SetAttribute("Val", param["name"].toStdString().c_str());
    configuration->InsertEndChild(prjNameOption);

    // <Option Name="Part" Val="xc7a35tfgg484-2"/>
    tinyxml2::XMLElement* partOption = doc.NewElement("Option");
    partOption->SetAttribute("Name", "Part");
    partOption->SetAttribute("Val", param["part"].toStdString().c_str());
    configuration->InsertEndChild(partOption);

    tinyxml2::XMLElement* archOption = doc.NewElement("Option");
    archOption->SetAttribute("Name", "Arch");
    archOption->SetAttribute("Val", param["arch"].toStdString().c_str());
    configuration->InsertEndChild(archOption);

    tinyxml2::XMLElement* archNameOption = doc.NewElement("Option");
    archNameOption->SetAttribute("Name", "ArchName");
    archNameOption->SetAttribute("Val", param["archName"].toStdString().c_str());
    configuration->InsertEndChild(archNameOption);

    // ------------------ Design Sources --------------------
    tinyxml2::XMLElement* designSrcsfileSet = doc.NewElement("FileSet");
    designSrcsfileSet->SetAttribute("Name", "sources");
    project->InsertEndChild(designSrcsfileSet);

    foreach (const QString designSrc, sourceList) {
        tinyxml2::XMLElement* fileElement = doc.NewElement("File");
        const QString item = "$PrjDir/sources/" + QFileInfo(designSrc).fileName();
        fileElement->SetAttribute("Name", item.toStdString().c_str());
        designSrcsfileSet->InsertEndChild(fileElement);
    }

    // TopModule
    tinyxml2::XMLElement* config = doc.NewElement("Config");
    designSrcsfileSet->InsertEndChild(config);
    tinyxml2::XMLElement* TopModuleOption = doc.NewElement("Option");
    TopModuleOption->SetAttribute("Name", "TopModule");
    TopModuleOption->SetAttribute("Val", param["top"].toStdString().c_str());
    config->InsertEndChild(TopModuleOption);

    // ------------------- Constraints -----------------------
    tinyxml2::XMLElement* constrsfileSet = doc.NewElement("FileSet");
    constrsfileSet->SetAttribute("Name", "constraints");
    project->InsertEndChild(constrsfileSet);

    foreach (const QString constr, constraintList) {
        tinyxml2::XMLElement* fileElement = doc.NewElement("File");
        const QString item = "$PrjDir/constraints/" + QFileInfo(constr).fileName();
        fileElement->SetAttribute("Name", item.toStdString().c_str());
        constrsfileSet->InsertEndChild(fileElement);
    }

    if (doc.SaveFile(hprPath.toStdString().c_str()) == tinyxml2::XML_SUCCESS) {
        return true;
    }
    return false;
}

/**
 * 解析工程文件，将工程参数保存在Map
 * @param hprPath 工程文件(*.hpr)路径
 * @return
 */
bool Project::parseProject(const QString &hprPath)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(hprPath.toStdString().c_str()) != tinyxml2::XML_SUCCESS) {
        qDebug() << "[Project] Error loading XML file";
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Project");
    if (!root) {
        return false;
    }
    QString prjDir = QFileInfo(hprPath).path();
    param["path"] = prjDir;  // 获取项目文件夹绝对路径

    // ------------------------------ Configuration --------------------------------
    tinyxml2::XMLElement* configuration = root->FirstChildElement("Configuration");
    if (!configuration) {
        return false;
    }
    tinyxml2::XMLElement* option = configuration->FirstChildElement("Option");
    while (option) {
        if (std::string(option->Attribute("Name")) == "PrjName")
            param["name"] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Part")
            param["part"] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Arch")
            param["arch"] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "ArchName")
            param["archName"] = option->Attribute("Val");
        option = option->NextSiblingElement("Option");
    }


    // ----------------------- DesignSources FileSet ---------------------------
    QStringList designSrcs;
    tinyxml2::XMLElement* fileSet = root->FirstChildElement("FileSet");
    if (!fileSet->Attribute("Name", "sources")) {
        return false;
    }

    tinyxml2::XMLElement* fileElement = fileSet->FirstChildElement("File");
    while (fileElement) {
        const char* name = fileElement->Attribute("Name");
        const QString item = QString(name).replace("$PrjDir", prjDir);
        designSrcs.append(item);
        fileElement = fileElement->NextSiblingElement("File");
    }
    sourceList = designSrcs;
    // ---------------------------- TopModule ----------------------------------
    tinyxml2::XMLElement* config = fileSet->FirstChildElement("Config");
    if (config) {
        tinyxml2::XMLElement* option = config->FirstChildElement("Option");
        if (option) {
            const char* name = option->Attribute("Name");
            const char* value = option->Attribute("Val");
            if (std::string(option->Attribute("Name")) == "TopModule")
                param["top"] = value;
            option = option->NextSiblingElement("Option");
        }
    }

    if (!fileSet) {
        return false;
    }
    fileSet = fileSet->NextSiblingElement("FileSet");
    if (!fileSet->Attribute("Name", "constraints")) {
        return false;
    }

    // ------------------------ constraints FileSet ----------------------------
    QStringList constrs;
    tinyxml2::XMLElement* fileElement1 = fileSet->FirstChildElement("File");
    while (fileElement1) {
        const char* name = fileElement1->Attribute("Name");
        const QString item = QString(name).replace("$PrjDir", prjDir);
        constrs.append(item);
        fileElement1 = fileElement1->NextSiblingElement("File");
    }
    constraintList = constrs;
    
    // 输出解析结果
    qDebug() << "---------------------------------------------------------------";
    qDebug() << "Project Name:" << param["name"];
    qDebug() << "Part        :" << param["part"];
    qDebug() << "ArchName    :" << param["archName"];
    qDebug() << "Arch        :" << param["arch"];
    qDebug() << "TopModule   :" << param["top"];
    qDebug() << "Design Sources-------------------------------------------------";
    foreach (const QString& source , this->sourceList) {
        qDebug() << " " << source;
    }
    qDebug() << "Constraints----------------------------------------------------";
    foreach (const QString& constraint , this->constraintList) {
        qDebug() << " " << constraint;
    }
    qDebug() << "---------------------------------------------------------------";
    return true;
}

/**
 * 获取工程参数
 * @param key
 * @return
 */
QString Project::getParam(const QString &key)
{
    return param[key];
}

/**
 * 获取所有工程参数
 * @return
 */
QMap<QString, QString> Project::getAllParams()
{
    return param;
}

void Project::setTopModule(const QString &topName)
{
    param["top"] = topName;
    writeProject();
}

