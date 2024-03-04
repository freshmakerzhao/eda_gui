/*
    编译结果显示区
*/

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QDebug>


class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    static InfoWidget *instance(QWidget *parent = nullptr);

    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    void appendMsg(const QString& str);

private:
    QPlainTextEdit *msg;
    QPlainTextEdit *log;
    QPlainTextEdit *rpt;
};

#endif // INFOWIDGET_H
