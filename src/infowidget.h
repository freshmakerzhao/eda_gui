/*
    编译结果显示区
*/

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>


class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

private:

};

#endif // INFOWIDGET_H
