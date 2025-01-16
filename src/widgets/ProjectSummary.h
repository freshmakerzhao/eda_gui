/**
  ******************************************************************************
  * @file           : ProjectSummary.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/19
  ******************************************************************************
  */
#ifndef PROJECTSUMMARY_H
#define PROJECTSUMMARY_H


#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include "entity/Project.h"

class ProjectSummary : public QWidget
{
    Q_OBJECT
public:
    static ProjectSummary *instance();

    /**
     * 设置工程参数
     */
    void setParams(const QMap<Project::ParamKey, QString> &params);
public slots:
private:
    ProjectSummary(QWidget* parent = nullptr);

    void init();

    QLabel *settingsPrjName = new QLabel("NA");
    QLabel *settingsPrjLocation = new QLabel("NA");
    QLabel *settingsPrjFamily = new QLabel("NA");
    QPushButton *settingsPrjPart = new QPushButton("NA");
    QPushButton *settingsPrjTopModuleName = new QPushButton("NA");

    QLabel *synthesisStatus = new QLabel("NA");
    QLabel *synthesisPart = new QLabel("NA");

    QLabel *implementationStatus = new QLabel("NA");
    QLabel *implementationPart = new QLabel("NA");

    // 最差的负的时差
    QLabel *timingWorstNegativeSlack = new QLabel("NA");
    // 负的时差总数
    QLabel *timingTotalNegativeSlack = new QLabel("NA");
    // 失败端点的数量
    QLabel *timingNumberOfFailingEndpoints = new QLabel("NA");
    // 节点总数
    QLabel *timingTotalNumberOfEndpoints = new QLabel("NA");

    const QString TITLE_LABEL_STYLESHEET = ".QLabel {"
                                           "    font-weight: bold; "
                                           "    background-color: rgb(237, 237, 237);"
                                           "}";

    const QString CONTENT_WIDGET_STYLESHEET = ".QWidget {"
                                      "     background-color: rgb(247, 247, 247);"
                                      "}";

    QSplitter *splitter;
};


#endif //HYBRDLINK_PRJSUMMARY_H
