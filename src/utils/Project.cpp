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

Project::~Project()
{
    qDebug() << "[Project] Distructing...";
}

/**
 * 初始化工程参数，仅在新建工程时使用
 * @param name
 * @param path
 * @param part
 * @param arch
 * @param archName
 * @param displayPart
 * @param familyName
 */
void Project::initProject(const QString &name,
                          const QString &path,
                          const QString &part,
                          const QString &arch,
                          const QString &archName,
                          const QString &displayPart,
                          const QString &familyName)
{
    param[Project::Name] = name;         // 工程名称
    param[Project::Path] = path;         // 工程路径(绝对)
    param[Project::Part] = part;
    param[Project::Arch] = arch;
    param[Project::ArchName] = archName;
    param[Project::TopModule] = "top";
    param[Project::DisplayPart] = displayPart;
    param[Project::FamilyName] = familyName;
}

bool Project::writeProject(){
    //! 工程文件的路径为: path/name.hpr
    QString hprPath = QString("%1/%2.hpr").arg(param[Project::Path], param[Project::Name]);
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);
    // 版本注释
    tinyxml2::XMLComment* versionComment = doc.NewComment("               Product Version: HybrdLink v2024.1                   ");
    doc.InsertEndChild(versionComment);
    // 版权注释
    tinyxml2::XMLComment* copyrightComment = doc.NewComment(" © Copyright 2024 HybrdChip Technology (Zhuhai) Co., Ltd. ");
    doc.InsertEndChild(copyrightComment);
    // Root
    tinyxml2::XMLElement* project = doc.NewElement("Project");
    doc.InsertEndChild(project);

    tinyxml2::XMLElement* configuration = doc.NewElement("Configuration");
    project->InsertEndChild(configuration);

    tinyxml2::XMLElement* prjNameOption = doc.NewElement("Option");
    prjNameOption->SetAttribute("Name", "PrjName");
    prjNameOption->SetAttribute("Val", param[Project::Name].toStdString().c_str());
    configuration->InsertEndChild(prjNameOption);

    // <Option Name="Part" Val="xc7a35tfgg484-2"/>
    tinyxml2::XMLElement* partOption = doc.NewElement("Option");
    partOption->SetAttribute("Name", "Part");
    partOption->SetAttribute("Val", param[Project::Part].toStdString().c_str());
    configuration->InsertEndChild(partOption);

    tinyxml2::XMLElement* archOption = doc.NewElement("Option");
    archOption->SetAttribute("Name", "Arch");
    archOption->SetAttribute("Val", param[Project::Arch].toStdString().c_str());
    configuration->InsertEndChild(archOption);

    tinyxml2::XMLElement* archNameOption = doc.NewElement("Option");
    archNameOption->SetAttribute("Name", "ArchName");
    archNameOption->SetAttribute("Val", param[Project::ArchName].toStdString().c_str());
    configuration->InsertEndChild(archNameOption);

    tinyxml2::XMLElement* familyNameOption = doc.NewElement("Option");
    familyNameOption->SetAttribute("Name", "FamilyName");
    familyNameOption->SetAttribute("Val", param[Project::FamilyName].toStdString().c_str());
    configuration->InsertEndChild(familyNameOption);

    tinyxml2::XMLElement* displayPartOption = doc.NewElement("Option");
    displayPartOption->SetAttribute("Name", "DisplayPart");
    displayPartOption->SetAttribute("Val", param[Project::DisplayPart].toStdString().c_str());
    configuration->InsertEndChild(displayPartOption);

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
    TopModuleOption->SetAttribute("Val", param[Project::TopModule].toStdString().c_str());
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
    param[Project::Path] = prjDir;  // 获取项目文件夹绝对路径

    // ------------------------------ Configuration --------------------------------
    tinyxml2::XMLElement* configuration = root->FirstChildElement("Configuration");
    if (!configuration) {
        return false;
    }
    tinyxml2::XMLElement* option = configuration->FirstChildElement("Option");
    while (option) {
        if (std::string(option->Attribute("Name")) == "PrjName")
            param[Project::Name] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Part")
            param[Project::Part] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Arch")
            param[Project::Arch] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "ArchName")
            param[Project::ArchName] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "FamilyName")
            param[Project::FamilyName] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "DisplayPart")
            param[Project::DisplayPart] = option->Attribute("Val");
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
                param[Project::TopModule] = value;
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
    qDebug() << "Project Name:" << param[Project::Name];
    qDebug() << "Part        :" << param[Project::Part];
    qDebug() << "Arch        :" << param[Project::Arch];
    qDebug() << "ArchName    :" << param[Project::ArchName];
    qDebug() << "FamilyName    :" << param[Project::FamilyName];
    qDebug() << "DisplayPart    :" << param[Project::DisplayPart];
    qDebug() << "TopModule   :" << param[Project::TopModule];
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
QString Project::getParam(ParamKey key) const
{
    return param[key];
}

/**
 * 获取所有工程参数
 * @return
 */
QMap<Project::ParamKey, QString> Project::getAllParams() const
{
    return param;
}

void Project::setTopModule(const QString &topName)
{
    param[Project::TopModule] = topName;
    writeProject();
}

