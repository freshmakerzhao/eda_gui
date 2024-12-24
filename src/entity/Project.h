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
#include "tinyxml2/tinyxml2.h"
// #include "tinyxml2.h"

class Project
{
public:
    Project();

    ~Project();

    enum ParamKey {
        Name,
        Path,
        Part,
        Arch,
        ArchName,
        TopModule,
        DisplayPart,
        FamilyName,
        BinFile,
        RbtFile,
        CRCOption,
        CompressOption,
    };

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
    void initProject(const QString &name,
                     const QString &path,
                     const QString &part,
                     const QString &arch,
                     const QString &archName,
                     const QString &displayPart = QString("MC1P110-FC484L-1"),
                     const QString &familyName = QString("MgiCubo"));

    bool writeProject();
    void saveGenerateBitstreamOptions(tinyxml2::XMLElement *stepGenerateBitstreamElement);
    /**
     * 解析工程文件，将工程参数保存在Map
     * @param hprPath 工程文件(*.hpr)路径
     * @return
     */
    bool parseProject(const QString &hprPath);
    bool parseRunsOptionValue(tinyxml2::XMLElement *runsRoot);
    void parseStepOptions(tinyxml2::XMLElement *step);
    /**
     * 获取工程参数
     * @param key
     * @return
     */
    QString getParameter(const Project::ParamKey key) const;
    void updateBinFileOption(const QString &binFileOptionStatus);
    void updateRbtFileOption(const QString &rbtFileOptionStatus);
    void updateCRCOption(const QString &crcOptionStatus);
    void updateCompressOption(const QString &compressOptionStatus);

    /**
     * 获取所有工程参数
     * @return
     */
    QMap<Project::ParamKey, QString> getAllParameters() const;

    void setTopModule(const QString &topName);

    void setDevicePart(const QString &displayPart);

    QStringList sourceList;     // Sources(绝对路径)
    QStringList constraintList; // Constraints(绝对路径)
    QStringList simList; // 仿真文件(绝对路径)



private:
    QMap<Project::ParamKey, QString> parameters;    // 工程参数

    // QMap<QString, QStringList> files; // 工程文件
};

#endif // PROJECT_H
