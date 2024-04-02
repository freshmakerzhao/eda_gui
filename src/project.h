/**
  ******************************************************************************
  * @file           : project.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/8
  ******************************************************************************
  */

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
    Project(QString name = "", QString path = "", QString part = "", QString arch = "", QString archName = "");

    bool makeProject();
    bool openProject(const QString &path);

    QString name; // 工程名称
    QString path; // 工程路径(绝对)
    QStringList sourceList;
    QStringList constraintList;
    QString part;
    QString archName;
    QString arch;
};

#endif // PROJECT_H
