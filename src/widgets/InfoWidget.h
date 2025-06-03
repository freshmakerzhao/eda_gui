/**
  ******************************************************************************
  * @file           : InfoWidget.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/18
  ******************************************************************************
  */
#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QDebug>
#include "component/tclconsole/TclConsole.h"

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    static InfoWidget *instance(QWidget *parent = nullptr);

    // 指定激活index标签
    void setCurrentPage(int index);
    QTabWidget *tabWidget;

    void updateSynthItem(const QString synthPath,
                         const QString status,
                         const QString startTime,
                         const QString Elapsed,
                         const QString partName);

    void updateImplItem(const QString& implPath,
                        const QString& status,
                        const QString& startTime,
                        const QString& Elapsed,
                        const QString& partName);

    void resetInfoWidget();

    void initDesignRunsView(const QString &prjPath = "");
private:
    InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    // QPlainTextEdit *csl;
    TclConsole *tclConsole;

    QPlainTextEdit *msg;
    QPlainTextEdit *log;
    QPlainTextEdit *rpt;

    QTreeView *runsView;
    QStandardItemModel *runsModel;

    // 初始化统计信息
    void initSummary(const QString phase);

    int lut6NumSynth = 0 , lutNumSynth = 0, muxf6NumSynth = 0 , ffNumSynth = 0 , bramNumSynth = 0 , fifo18NumSynth = 0 , ranb18NumSynth = 0 , ranb36NumSynth = 0 , dspNumSynth = 0 ,carry4NumSynth = 0;
    int lut6NumImpl = 0 , lutNumImpl = 0, muxf6NumImpl = 0 , ffNumImpl = 0 , bramNumImpl = 0 , fifo18NumImpl = 0 , ranb18NumImpl = 0 , ranb36NumImpl = 0 , dspNumImpl = 0 ,carry4NumImpl = 0;

    std::unordered_map<std::string, int*> keyMapSynth = {
        {"$lut", &lutNumSynth},
        {"INV", &lutNumSynth},
        {"LUT2", &lutNumSynth},
        {"LUT3", &lutNumSynth},
        {"LUT4", &lutNumSynth},
        {"LUT5", &lutNumSynth},
        {"MUXF6", &muxf6NumSynth},
        {"FDRE", &ffNumSynth},
        {"FDSE", &ffNumSynth},
        {"FDCE", &ffNumSynth},
        {"FDPE", &ffNumSynth},
        {"FIFO18E1", &fifo18NumSynth},
        {"RAMB18E1", &ranb18NumSynth},
        {"RAMB36E1", &ranb36NumSynth},
        {"DSP48E1", &dspNumSynth},
        {"CARRY4", &carry4NumSynth}
    };

    std::unordered_map<std::string, int*> keyMapImpl = {
        {"BLK-TL-DSP48E1", &dspNumImpl},
        {"BLK-TL-BRAM_L", &bramNumImpl}
    };

public slots:
    void generateTimingSummary();

    void generateReportPower();

    void generateIOports();

    void onTabWidgetTabCloseRequested(int index);
};

#endif // INFOWIDGET_H
