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
#include <unordered_map>

#include "SearchTextEdit.h"
#include "entity/LogPipeContent.h"
#include <QTabWidget>


class SingleLogWidget : public QWidget
{
    Q_OBJECT
public:
    SingleLogWidget(const std::string &phase, QWidget* parent = nullptr);
    QWidget* baseWidget;
    SearchTextEdit *logTextEdit;
};

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    static LogWidget *instance(QWidget *parent = nullptr);

    void appendLog(const QString &phaseType, const QString& str);
    void appendLog(const LogPipeContent& one_log);
    QTabWidget* phaseTabWidget;

    void clearLog();

private:
    LogWidget(QWidget* parent = nullptr);

    SingleLogWidget* synthesisLogWidget;
    SingleLogWidget* implementationLogWidget;
    SingleLogWidget* simulationLogWidget;

};


#endif // LOGWIDGET_H
