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

Project::Project(QString name,
                 QString path,
                 QString part,
                 QString arch,
                 QString archName){
    param["name"] = name;          // 工程名称
    param["path"] = path;          // 工程路径(绝对)
    param["part"] = part;
    param["arch"] = arch;
    param["archName"] = archName;
}


bool Project::makeProject()
{
    QString hprfile = param["path"] +  "/" + param["name"] + ".hpr";
    QFile file(hprfile);
    if(!file.remove()) { // 删除已有hpr文件
        qDebug() << "remove error:" << file.errorString();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法创建XML文件";
        return false;
    }

    // ================== Debug ==================
    qDebug() << "sourceList";
    for (const QString& source : sourceList) {
        qDebug() << " " << source;
    }
    qDebug() << "constraintList";
    for (const QString& constraint : constraintList) {
        qDebug() << " " << constraint;
    }
    // 创建XML写入器
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // 写入XML内容
    xmlWriter.writeStartElement("root");
    // ================== 写入工程名称 ==================
    xmlWriter.writeAttribute("Name", "Project");
    xmlWriter.writeAttribute("Val", param["name"]);
    // ========= 写入资源列表路径(以相对路径保存) ==========
    for (const QString& source : sourceList) {
        QString relative = "/sources/" + QFileInfo(source).fileName();
        xmlWriter.writeTextElement("sourcePath", relative);
    }
    for (const QString& constraint : constraintList) {
        QString relative = "/constraints/" + QFileInfo(constraint).fileName();
        xmlWriter.writeTextElement("constraintPath", relative);
    }
    // =================== 写入Part ====================
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "Part");
    xmlWriter.writeAttribute("Val", param["part"]);

    // =================== 写入Arch =====================
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "Arch");
    xmlWriter.writeAttribute("Val", param["arch"]);

    // ================== 写入ArchName ==================
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "ArchName");
    xmlWriter.writeAttribute("Val", param["archName"]);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
    return true;
}

/**
     * 解析工程文件，将工程参数保存在Map
     * @param path 工程文件(*.hpr)路径
     * @return
     */
bool Project::parseProject(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开XML文件";
        return false;
    }
    param["path"] = QFileInfo(path).path();  // 获取项目文件夹绝对路径
    QString basePath = param["path"];
    // 创建XML读取器
    QXmlStreamReader xmlReader(&file);

    // 解析XML内容
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        // 逐行解析
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef elementName = xmlReader.name();
            if (elementName == "sourcePath") {
                xmlReader.readNext();
                QString absolute = basePath + xmlReader.text().toString();
                this->sourceList.append(absolute);
            } else if (elementName == "constraintPath") {
                xmlReader.readNext();
                QString absolute = basePath + xmlReader.text().toString();
                this->constraintList.append(absolute);
            } else if (elementName == "Option") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                QString name = attributes.value("Name").toString();
                QString val = attributes.value("Val").toString();

                if (!name.isEmpty()) {
                    if (name == "Part") {
                        param["part"] = val;
                    } else if (name == "ArchName") {
                        param["archName"] = val;
                    } else if (name == "Arch") {
                        param["arch"] = val;
                    }
                    // 可以根据需要添加更多的条件分支
                }

            } else if (elementName == "root") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                QString val = attributes.value("Val").toString();
                param["name"] = val;
            }
        }
    }

    // 检查XML解析是否成功
    if (xmlReader.hasError()) {
        qDebug() << "解析XML文件时发生错误：" << xmlReader.errorString();
        return false;
    }

    // 输出解析结果
    qDebug() << "=======================================";
    qDebug() << "Project Name " << param["name"];
    qDebug() << "Part Val:    " << param["part"];
    qDebug() << "ArchName Val:" << param["archName"];
    qDebug() << "Arch Val:    " << param["arch"];
    qDebug() << "Source Path List:======================";
    for (const QString& source : this->sourceList) {
        qDebug() << " " << source;
    }
    qDebug() << "Constrain Path List:===================";
    for (const QString& constraint : this->constraintList) {
        qDebug() << " " << constraint;
    }
    qDebug() << "=======================================";
    // 关闭文件
    file.close();
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

QMap<QString, QString> Project::getAllParams()
{
    return param;
}

