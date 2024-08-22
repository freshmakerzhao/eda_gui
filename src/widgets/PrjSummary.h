/**
  ******************************************************************************
  * @file           : PrjSummary.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 19/6/2024
  ******************************************************************************
  */
#ifndef HYBRDLINK_PRJSUMMARY_H
#define HYBRDLINK_PRJSUMMARY_H


#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "utils/Project.h"

class PrjSummary : public QWidget
{
    Q_OBJECT
public:
    static PrjSummary *instance();

    /**
     * 设置工程参数
     */
    void setParams(const QMap<Project::ParamKey, QString> &params);
public slots:
private:
    PrjSummary(QWidget* parent = nullptr);

    void init();

    int rightlabelWidth = 265;
    int leftLabelWidth = 160;
    int rowHeight = 30;
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
};


#endif //HYBRDLINK_PRJSUMMARY_H
