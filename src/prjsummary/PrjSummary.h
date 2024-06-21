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

class PrjSummary : public QWidget
{
    Q_OBJECT
public:
    static PrjSummary *instance();

public slots:
private:
    PrjSummary(QWidget* parent = nullptr);

    void init();
    int rightlabelWidth = 185;
    int leftLabelWidth = 140;
    int rowHeight = 30;
    QString settingsPrjName = "240605_01_ramb36e1_read_width_72";
    QString settingsPrjLocation = "C:/Users/DELL/Desktop/IP/240605_01_ramb36e1_read_width_72";
    QString settingsPrjFamily = "Artix-7";
    QString settingsPrjPart = "xc7a35tfg484-2";
    QString settingsPrjTopModuleName = "top";

    QString synthesisStatus = "Complete";
    QString synthesisPart = "xc7a35tfgg484-2";

    QString implementationStatus = "Complete";
    QString implementationPart = "xc7a35tfgg484-2";

    // 最差的负的时差
    QString timingWorstNegativeSlack = "NA";
    // 负的时差总数
    QString timingTotalNegativeSlack = "NA";
    // 失败端点的数量
    QString timingNumberOfFailingEndpoints = "NA";
    // 节点总数
    QString timingTotalNumberOfEndpoints = "NA";
};


#endif //HYBRDLINK_PRJSUMMARY_H
