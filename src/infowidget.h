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

    void updateSynthItem(const QString synthPath);
    void updateImplItem(const QString implPath);
private:
    InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    QPlainTextEdit *csl;
    QPlainTextEdit *msg;
    QPlainTextEdit *log;
    QPlainTextEdit *rpt;

    QTreeView *runsView;
    QStandardItemModel *runsModel;
};

#endif // INFOWIDGET_H
