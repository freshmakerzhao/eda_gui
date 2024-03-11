#include "project.h"


Project::Project(QString name, QString path, QString part)
{
    this->name = name;
    this->path = path;
    this->part = part;
}

bool Project::makeProject()
{
    QString filePath = this->path;
    QFile file(filePath + "/" + name + "/" + name + ".hpr");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法创建XML文件";
        return -1;
    }

    // 创建XML写入器
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // 写入sourcePath
    xmlWriter.writeStartElement("root");
    xmlWriter.writeAttribute("Name", "Project");
    xmlWriter.writeAttribute("Val", this->name);
    for (const QString& s_path : sourcePathList) {
        xmlWriter.writeTextElement("sourcePath", s_path);
    }
    qDebug() << "Source Path List:";

    // 写入constraintPath
    for (const QString& c_path : constraintPathList) {
        xmlWriter.writeTextElement("constraintPath", c_path);
    }
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "Part");
    xmlWriter.writeAttribute("Val", this->part);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    // 关闭文件
    file.close();
    return false;
}

bool Project::openProject(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开XML文件";
        return -1;
    }
    // 创建XML读取器
    QXmlStreamReader xmlReader(&file);

    // 解析XML内容
    QStringList sourceTmp;
    QStringList constraintTmp;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef elementName = xmlReader.name();
            if (elementName == "sourcePath") {
                xmlReader.readNext();
                QString relativePath = xmlReader.text().toString();
                sourceTmp.append(relativePath);
            } else if (elementName == "constraintPath") {
                xmlReader.readNext();
                QString relativePath = xmlReader.text().toString();
                constraintTmp.append(relativePath);
            } else if (elementName == "Option") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                // QString xname = attributes.value("Name").toString();
                QString xval = attributes.value("Val").toString();
                this->part = xval;
            } else if (elementName == "root") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                // QString xname = attributes.value("Name").toString();
                QString xval = attributes.value("Val").toString();
                this->name = xval;
            }
        }
    }

    // 检查XML解析是否成功
    if (xmlReader.hasError()) {
        qDebug() << "解析XML文件时发生错误：" << xmlReader.errorString();
        return -1;
    }

    // 输出解析结果
    qDebug() << "Project Name" << this->name;
    qDebug() << "Part Val:" << this->part;
    qDebug() << "Source Path List:";
    for (const QString& path : sourceTmp) {
        qDebug() << path;
    }
    sourcePathList = sourceTmp;
    qDebug() << "Source Path List:";
    for (const QString& path : constraintTmp) {
        qDebug() << path;
    }
    constraintPathList = constraintTmp;
    // 关闭文件
    file.close();
    return true;
}

