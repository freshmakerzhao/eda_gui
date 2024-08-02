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

    void initDesignRunsView(const QString &prjPath = "");
private:
    InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    QPlainTextEdit *csl;
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
        {"MUXF6", &muxf6NumSynth},
        {"FDRE_ZINI", &ffNumSynth}, {"FDSE_ZINI", &ffNumSynth},
        {"FDCE_ZINI", &ffNumSynth}, {"FDPE_ZINI", &ffNumSynth},
        {"FDRE_ZINI_1", &ffNumSynth}, {"FDSE_ZINI_1", &ffNumSynth},
        {"FDCE_ZINI_1", &ffNumSynth}, {"FDPE_ZINI_1", &ffNumSynth},
        {"FIFO18E1_VPR", &fifo18NumSynth},
        {"RAMB18E1_VPR", &ranb18NumSynth},
        {"RAMB36E1_PRIM", &ranb36NumSynth},
        {"DSP48E1_VPR", &dspNumSynth},
        {"CARRY4_VPR", &carry4NumSynth}
    };

    std::unordered_map<std::string, int*> keyMapImpl = {
        {"BLK-TL-DSP48E1", &dspNumImpl},
        {"BLK-TL-BRAM_L", &bramNumImpl}
    };
};

#endif // INFOWIDGET_H
