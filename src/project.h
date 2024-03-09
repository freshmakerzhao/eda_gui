#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QXmlStreamWriter>

class Project
{
public:
    Project(QString name = "", QString path = "", QString part = "");

    bool makeProject();
    bool openProject(const QString &path);

    QString name; // 工程名称
    QString path; // 工程路径(绝对)
    QStringList sourceList;
    QStringList constraintList;
    QString part;
};

#endif // PROJECT_H
