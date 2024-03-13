#include "project.h"


Project::Project(QString name, QString path, QString part, QString arch)
{
    this->name = name;
    this->path = path;
    this->part = part;
    this->arch = arch;
}

bool Project::makeProject()
{
    QString hprfile = this->path +  "/" + name + ".hpr";
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
    xmlWriter.writeAttribute("Val", this->name);
    // ================== 写入资源列表路径 ==================
    for (const QString& source : sourceList) {
        QString relative = "/sources/" + QFileInfo(source).fileName();
        xmlWriter.writeTextElement("sourcePath", relative);
    }
    for (const QString& constraint : constraintList) {
        QString relative = "/constraints/" + QFileInfo(constraint).fileName();
        xmlWriter.writeTextElement("constraintPath", relative);
    }
    // ================== 写入Part ==================
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "Part");
    xmlWriter.writeAttribute("Val", this->part);

    // ================== 写入Arch ==================
    xmlWriter.writeStartElement("Option");
    xmlWriter.writeAttribute("Name", "Arch");
    xmlWriter.writeAttribute("Val", this->arch);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
    return true;
}

bool Project::openProject(const QString &path)
{
    // this->sourceList.clear();
    // this->constraintList.clear();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开XML文件";
        return false;
    }
    this->path = QFileInfo(path).path();
    QString basePath = this->path;
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
                // QString absolute = QDir(basePath).absoluteFilePath(xmlReader.text().toString());
                // QString relative = xmlReader.text().toString();
                QString absolute = basePath + xmlReader.text().toString();
                this->sourceList.append(absolute);
            } else if (elementName == "constraintPath") {
                xmlReader.readNext();
                // QString absolute = QDir(basePath).absoluteFilePath(xmlReader.text().toString());
                QString absolute = basePath + xmlReader.text().toString();
                this->constraintList.append(absolute);
            } else if (elementName == "Option") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                QString name = attributes.value("Name").toString();
                QString val = attributes.value("Val").toString();

                if (!name.isEmpty()) {
                    if (name == "Part") {
                        this->part = val; 
                    } else if (name == "Arch") {
                        this->arch = val;
                    }
                    // 可以根据需要添加更多的条件分支
                }
            } else if (elementName == "root") {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                // QString xname = attributes.value("Name").toString();
                QString val = attributes.value("Val").toString();
                this->name = val;
            }
        }
    }

    // 检查XML解析是否成功
    if (xmlReader.hasError()) {
        qDebug() << "解析XML文件时发生错误：" << xmlReader.errorString();
        return false;
    }

    // 输出解析结果
    qDebug() << "Project Name" << this->name;
    qDebug() << "Part Val:" << this->part;
    qDebug() << "Arch Val:" << this->arch;
    qDebug() << "=======================================";
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

