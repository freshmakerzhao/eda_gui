#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPixmap>
#include <QHBoxLayout>
#include <QDebug>
#include <QLineEdit>
#include <QFormLayout>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QClipboard>

#include "SearchTextEdit.h"

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    static LogWidget *instance(QWidget *parent = nullptr);

    void appendLog(const QString& str);

private:
    LogWidget(QWidget* parent = nullptr);

    void init();

    QWidget* baseWidget;
    SearchTextEdit *logTextEdit;

public slots:
    // 处理日志信号
    void handleLogArrived(int level, const QString &message, const QString &phase);
};


#endif // LOGWIDGET_H
