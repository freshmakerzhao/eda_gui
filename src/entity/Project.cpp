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
#include "utils/DeviceInfoUtils.h"

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
 * @parameters name
 * @parameters path
 * @parameters part
 * @parameters arch
 * @parameters archName
 * @parameters displayPart
 * @parameters familyName
 */
void Project::initProject(const QString &name,
                          const QString &path,
                          const QString &part,
                          const QString &arch,
                          const QString &archName,
                          const QString &displayPart,
                          const QString &familyName)
{
    parameters[Project::Name] = name;         // 工程名称
    parameters[Project::Path] = path;         // 工程路径(绝对)
    parameters[Project::Part] = part;
    parameters[Project::Arch] = arch;
    parameters[Project::ArchName] = archName;
    parameters[Project::TopModule] = "top";
    parameters[Project::DisplayPart] = displayPart;
    parameters[Project::FamilyName] = familyName;

    parameters[Project::BinFile]        = "disable";
    parameters[Project::RbtFile]        = "disable";
    parameters[Project::CRCOption]      = "disable";
    parameters[Project::CompressOption] = "disable";
}

bool Project::writeProject(){

    // ------------------ 基本信息 开始 --------------------
    //! 工程文件的路径为: path/name.hpr
    QString hprPath = QString("%1/%2.hpr").arg(parameters[Project::Path], parameters[Project::Name]);
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);
    // 版本注释
    tinyxml2::XMLComment* versionComment = doc.NewComment("               Product Version: HybrdLink v2024.1                   ");
    doc.InsertEndChild(versionComment);
    // 版权注释
    tinyxml2::XMLComment* copyrightComment = doc.NewComment(" © Copyright 2024 HybrdChip Technology (Zhuhai) Co., Ltd. ");
    doc.InsertEndChild(copyrightComment);
    // ------------------ 基本信息 结束 --------------------

    // ------------------ Configuration 开始 --------------------
    tinyxml2::XMLElement* project = doc.NewElement("Project");
    doc.InsertEndChild(project);

    tinyxml2::XMLElement* configuration = doc.NewElement("Configuration");
    project->InsertEndChild(configuration);

    tinyxml2::XMLElement* prjNameOption = doc.NewElement("Option");
    prjNameOption->SetAttribute("Name", "PrjName");
    prjNameOption->SetAttribute("Val", parameters[Project::Name].toStdString().c_str());
    configuration->InsertEndChild(prjNameOption);

    // <Option Name="Part" Val="xc7a35tfgg484-2"/>
    tinyxml2::XMLElement* partOption = doc.NewElement("Option");
    partOption->SetAttribute("Name", "Part");
    partOption->SetAttribute("Val", parameters[Project::Part].toStdString().c_str());
    configuration->InsertEndChild(partOption);

    tinyxml2::XMLElement* archOption = doc.NewElement("Option");
    archOption->SetAttribute("Name", "Arch");
    archOption->SetAttribute("Val", parameters[Project::Arch].toStdString().c_str());
    configuration->InsertEndChild(archOption);

    tinyxml2::XMLElement* archNameOption = doc.NewElement("Option");
    archNameOption->SetAttribute("Name", "ArchName");
    archNameOption->SetAttribute("Val", parameters[Project::ArchName].toStdString().c_str());
    configuration->InsertEndChild(archNameOption);

    tinyxml2::XMLElement* familyNameOption = doc.NewElement("Option");
    familyNameOption->SetAttribute("Name", "FamilyName");
    familyNameOption->SetAttribute("Val", parameters[Project::FamilyName].toStdString().c_str());
    configuration->InsertEndChild(familyNameOption);

    tinyxml2::XMLElement* displayPartOption = doc.NewElement("Option");
    displayPartOption->SetAttribute("Name", "DisplayPart");
    displayPartOption->SetAttribute("Val", parameters[Project::DisplayPart].toStdString().c_str());
    configuration->InsertEndChild(displayPartOption);
    // ------------------ Configuration 结束 --------------------

    // ------------------ FileSet Design Sources 开始 --------------------
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
    TopModuleOption->SetAttribute("Val", parameters[Project::TopModule].toStdString().c_str());
    config->InsertEndChild(TopModuleOption);
    // ------------------ FileSet Design Sources 结束 --------------------

    // ------------------ FileSet Constraints Sources 开始 --------------------
    tinyxml2::XMLElement* constrsfileSet = doc.NewElement("FileSet");
    constrsfileSet->SetAttribute("Name", "constraints");
    project->InsertEndChild(constrsfileSet);

    foreach (const QString constr, constraintList) {
        tinyxml2::XMLElement* fileElement = doc.NewElement("File");
        const QString item = "$PrjDir/constraints/" + QFileInfo(constr).fileName();
        fileElement->SetAttribute("Name", item.toStdString().c_str());
        constrsfileSet->InsertEndChild(fileElement);
    }
    // ------------------ FileSet Constraints Sources 结束 --------------------

    // ------------------ Runs 开始 --------------------
    tinyxml2::XMLElement* runsRoot = doc.NewElement("Runs");
    project->InsertEndChild(runsRoot); // Runs 插入 Project
    // SYNTHESIS
    tinyxml2::XMLElement* synthRunItem = doc.NewElement("Run");
    synthRunItem->SetAttribute("Type", "SYNTHESIS");
    synthRunItem->SetAttribute("SrcSet", "sources");
    synthRunItem->SetAttribute("Part", parameters[Project::Part].toStdString().c_str());
    synthRunItem->SetAttribute("ConstrsSet", "constraints");
    runsRoot->InsertEndChild(synthRunItem); // Run 插入 Runs
    // SYNTHESIS RUN 添加 Strategy 节点
    tinyxml2::XMLElement* synthRunstrategyElement = doc.NewElement("Strategy");
    synthRunItem->InsertEndChild(synthRunstrategyElement); // Strategy 插入 Run
    // SYNTHESIS RUN Strategy 添加 Step 节点
    tinyxml2::XMLElement* stepSynthElement = doc.NewElement("Step");
    stepSynthElement->SetAttribute("Id", "SYNTHESIS");
    synthRunstrategyElement->InsertEndChild(stepSynthElement); // Step 插入 Strategy

    // IMPLEMENTATION
    tinyxml2::XMLElement* implRunItem = doc.NewElement("Run");
    implRunItem->SetAttribute("Type", "IMPLEMENTATION");
    implRunItem->SetAttribute("SrcSet", "sources");
    implRunItem->SetAttribute("Part", parameters[Project::Part].toStdString().c_str());
    implRunItem->SetAttribute("ConstrsSet", "constraints");
    runsRoot->InsertEndChild(implRunItem); // Run 插入 Runs
    // IMPLEMENTATION RUN 添加 Strategy 节点
    tinyxml2::XMLElement* implRunstrategyElement = doc.NewElement("Strategy");
    implRunItem->InsertEndChild(implRunstrategyElement); // Strategy 插入 Run
    // IMPLEMENTATION RUN Strategy 添加 Step 节点
    tinyxml2::XMLElement* stepPackElement = doc.NewElement("Step");
    stepPackElement->SetAttribute("Id", "PACK");
    tinyxml2::XMLElement* stepPlaceElement = doc.NewElement("Step");
    stepPlaceElement->SetAttribute("Id", "PLACE");
    tinyxml2::XMLElement* stepRouteElement = doc.NewElement("Step");
    stepRouteElement->SetAttribute("Id", "ROUTE");
    tinyxml2::XMLElement* stepGenerateBitstreamElement = doc.NewElement("Step");
    stepGenerateBitstreamElement->SetAttribute("Id", "GENERATE_BITSTREAM");
    // 根据工程参数写入IMPLEMENTATION各阶段参数
    saveGenerateBitstreamOptions(stepGenerateBitstreamElement);
    implRunstrategyElement->InsertEndChild(stepPackElement);
    implRunstrategyElement->InsertEndChild(stepPlaceElement);
    implRunstrategyElement->InsertEndChild(stepRouteElement);
    implRunstrategyElement->InsertEndChild(stepGenerateBitstreamElement); // Step 插入 Strategy
    // ------------------ Runs 结束 --------------------

    // ------------------ 保存并退出 开始 --------------------
    if (doc.SaveFile(hprPath.toStdString().c_str()) == tinyxml2::XML_SUCCESS) {
        return true;
    }
    return false;
    // ------------------ 保存并退出 开始 --------------------
}

void Project::saveGenerateBitstreamOptions(tinyxml2::XMLElement *stepGenerateBitstreamElement) {
    // 检查并添加 BinFile
    if (parameters.value(Project::BinFile, "disable") == "enable") {
        tinyxml2::XMLElement* optionBinFile = stepGenerateBitstreamElement->GetDocument()->NewElement("Option");
        optionBinFile->SetAttribute("Id", "BinFile");
        optionBinFile->SetText("1"); // "1" 表示启用
        stepGenerateBitstreamElement->InsertEndChild(optionBinFile);
    }

    if (parameters.value(Project::RbtFile, "disable") == "enable") {
        tinyxml2::XMLElement* optionRbtFile = stepGenerateBitstreamElement->GetDocument()->NewElement("Option");
        optionRbtFile->SetAttribute("Id", "RbtFile");
        optionRbtFile->SetText("1");
        stepGenerateBitstreamElement->InsertEndChild(optionRbtFile);
    }

    if (parameters.value(Project::CRCOption, "disable") == "enable") {
        tinyxml2::XMLElement* optionCRCFile = stepGenerateBitstreamElement->GetDocument()->NewElement("Option");
        optionCRCFile->SetAttribute("Id", "CRCOption");
        optionCRCFile->SetText("1");
        stepGenerateBitstreamElement->InsertEndChild(optionCRCFile);
    }

    if (parameters.value(Project::CompressOption, "disable") == "enable") {
        tinyxml2::XMLElement* optionCompressFile = stepGenerateBitstreamElement->GetDocument()->NewElement("Option");
        optionCompressFile->SetAttribute("Id", "CompressOption");
        optionCompressFile->SetText("1");
        stepGenerateBitstreamElement->InsertEndChild(optionCompressFile);
    }
}

/**
 * 解析工程文件，将工程参数保存在Map
 * @parameters hprPath 工程文件(*.hpr)路径
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
    parameters[Project::Path] = prjDir;  // 获取项目文件夹绝对路径

    // ------------------------------ Configuration 开始 --------------------------------
    tinyxml2::XMLElement* configuration = root->FirstChildElement("Configuration");
    if (!configuration) {
        return false;
    }
    tinyxml2::XMLElement* option = configuration->FirstChildElement("Option");
    while (option) {
        if (std::string(option->Attribute("Name")) == "PrjName")
            parameters[Project::Name] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Part")
            parameters[Project::Part] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "Arch")
            parameters[Project::Arch] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "ArchName")
            parameters[Project::ArchName] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "FamilyName")
            parameters[Project::FamilyName] = option->Attribute("Val");
        if (std::string(option->Attribute("Name")) == "DisplayPart")
            parameters[Project::DisplayPart] = option->Attribute("Val");
        option = option->NextSiblingElement("Option");
    }
    // ------------------------------ Configuration 结束 --------------------------------

    // ----------------------- DesignSources FileSet 开始 ---------------------------
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
                parameters[Project::TopModule] = value;
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
    // ----------------------- DesignSources FileSet 结束 ---------------------------

    // ------------------------ Constraints FileSet 开始 ----------------------------
    QStringList constrs;
    tinyxml2::XMLElement* fileElement1 = fileSet->FirstChildElement("File");
    while (fileElement1) {
        const char* name = fileElement1->Attribute("Name");
        const QString item = QString(name).replace("$PrjDir", prjDir);
        constrs.append(item);
        fileElement1 = fileElement1->NextSiblingElement("File");
    }
    constraintList = constrs;
    // ------------------------ Constraints FileSet 结束 ----------------------------

    // ------------------------ 解析 Runs 开始 ----------------------------
    tinyxml2::XMLElement* runsRoot = root->FirstChildElement("Runs");
    tinyxml2::XMLElement* runElement = runsRoot->FirstChildElement("Run");
    parseRunsOptionValue(runsRoot);
    // ------------------------ 解析 Runs 结束 ----------------------------

    // 输出解析结果
    qDebug() << "---------------------------------------------------------------";
    qDebug() << "Project Name:" << parameters[Project::Name];
    qDebug() << "Part        :" << parameters[Project::Part];
    qDebug() << "Arch        :" << parameters[Project::Arch];
    qDebug() << "ArchName    :" << parameters[Project::ArchName];
    qDebug() << "FamilyName    :" << parameters[Project::FamilyName];
    qDebug() << "DisplayPart    :" << parameters[Project::DisplayPart];
    qDebug() << "TopModule   :" << parameters[Project::TopModule];
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

bool Project::parseRunsOptionValue(tinyxml2::XMLElement *runsRoot) {
    // 获取 <Runs> 根节点
    if (!runsRoot) {
        qWarning() << "No <Runs> element found.";
        return false;
    }
    // 遍历 <Run> 节点
    tinyxml2::XMLElement *runElement = runsRoot->FirstChildElement("Run");
    while (runElement) {
        // 获取 <Strategy> 节点
        tinyxml2::XMLElement *strategy = runElement->FirstChildElement("Strategy");
        if (strategy) {
            // 遍历 <Step> 节点
            tinyxml2::XMLElement *step = strategy->FirstChildElement("Step");
            while (step) {
                const char *stepIdAttr = step->Attribute("Id");
                if (stepIdAttr && QString(stepIdAttr) == "GENERATE_BITSTREAM") {
                    // 找到目标 <Step> 节点，检查其 <Option> 子节点
                    tinyxml2::XMLElement *option = step->FirstChildElement("Option");
                    while (option) {
                        const char *optionIdAttr = option->Attribute("Id");
                        const char *optionValue = option->GetText();
                        if (optionIdAttr && QString(optionIdAttr) == "BinFile") {
                            parameters[Project::BinFile] = "enable";
                        } else if (optionIdAttr && QString(optionIdAttr) == "RbtFile") {
                            parameters[Project::RbtFile] = "enable";
                        } else if (optionIdAttr && QString(optionIdAttr) == "CRCOption") {
                            parameters[Project::CRCOption] = "enable";
                        } else if (optionIdAttr && QString(optionIdAttr) == "CompressOption") {
                            parameters[Project::CompressOption] = "enable";
                        }
                        option = option->NextSiblingElement("Option");
                    }
                }
                step = step->NextSiblingElement("Step");
            }
        }
        runElement = runElement->NextSiblingElement("Run");
    }
    return true;
}

/**
 * 获取工程参数
 * @parameters key
 * @return
 */
QString Project::getParameter(const ParamKey key) const
{
    return parameters[key];
}

/**
 * 获取所有工程参数
 * @return
 */
QMap<Project::ParamKey, QString> Project::getAllParameters() const
{
    return parameters;
}

void Project::setTopModule(const QString &topName)
{
    parameters[Project::TopModule] = topName;
    writeProject();
}

void Project::setDevicePart(const QString &displayPart)
{
    DeviceInfoUtils util;
    DeviceInfo info = util.getDeviceInfo(displayPart);
    parameters[Project::Part] = info.part;
    parameters[Project::Arch] = info.arch;
    parameters[Project::ArchName]= info.archName;
    parameters[Project::DisplayPart] = displayPart;
    parameters[Project::FamilyName] = info.family_name;
    writeProject();
}

void Project::updateBinFileOption(const QString &binFileOptionStatus){
    parameters[Project::BinFile] = binFileOptionStatus;
}
void Project::updateRbtFileOption(const QString &rbtFileOptionStatus){
    parameters[Project::RbtFile] = rbtFileOptionStatus;
}
void Project::updateCompressOption(const QString &compressOptionStatus){
    parameters[Project::CompressOption] = compressOptionStatus;
}
void Project::updateCRCOption(const QString &crcOptionStatus){
    parameters[Project::CRCOption] = crcOptionStatus;
}

