#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    static InfoWidget *instance(QWidget *parent = nullptr);

    void appendMsg(const QString& str);
    void appendLog(const QString& str);
    // 指定激活index标签
    void setCurrentPage(int index);
    QTabWidget *tabWidget;

    void updateSynthItem(const QString synthPath, const QString status, const QString startTime, const QString Elapsed , const QString partName);
    void updateImplItem(const QString& implPath, const QString& status, const QString& startTime, const QString& Elapsed , const QString& partName);
private:
    InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    QPlainTextEdit *csl;
    QPlainTextEdit *msg;
    QPlainTextEdit *log;
    QPlainTextEdit *rpt;

    QTreeView *runsView;
    QStandardItemModel *runsModel;


    int lut6NumSynth = 0 , lutNumSynth = 0, muxf6NumSynth = 0 , ffNumSynth = 0 , bramNumSynth = 0 , fifo18NumSynth = 0 , ranb18NumSynth = 0 , ranb36NumSynth = 0 , dspNumSynth = 0 ,carry4NumSynth = 0;
    int lut6NumImpl = 0 , lutNumImpl = 0, muxf6NumImpl = 0 , ffNumImpl = 0 , bramNumImpl = 0 , fifo18NumImpl = 0 , ranb18NumImpl = 0 , ranb36NumImpl = 0 , dspNumImpl = 0 ,carry4NumImpl = 0;
};

#endif // INFOWIDGET_H
