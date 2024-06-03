/**
  ******************************************************************************
  * @file           : Project.h
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
#include <tinyxml2/tinyxml2.h>

class Project
{
public:
    Project(QString name = "",
            QString path = "",
            QString part = "",
            QString arch = "",
            QString archName = "");

    bool writeProject();

    /**
     * 解析工程文件，将工程参数保存在Map
     * @param hprPath 工程文件(*.hpr)路径
     * @return
     */
    bool parseProject(const QString &hprPath);

    /**
     * 获取工程参数
     * @param key
     * @return
     */
    QString getParam(const QString& key);

    /**
     * 获取所有工程参数
     * @return
     */
    QMap<QString, QString> getAllParams();

    void setTopModule(const QString &topName);

    QStringList sourceList;     // Sources(绝对路径)
    QStringList constraintList; // Constraints(绝对路径)

private:
    QMap<QString, QString> param;    // 工程参数

    // QMap<QString, QStringList> files; // 工程文件
};

#endif // PROJECT_H
