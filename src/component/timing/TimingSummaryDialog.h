#ifndef TIMINGSUMMARYDIALOG_H
#define TIMINGSUMMARYDIALOG_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QGraphicsView>
#include <QToolBar>
#include <QSplitter>
#include <QScrollArea>
#include <QDialogButtonBox>
#include "TimingOptionsWidget.h"

class TimingSummaryDialog : public QDialog
{
public:
    TimingSummaryDialog(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;

    TimingOptionsWidget *timingOptionsWidget;

    QLineEdit *resultsNameLineEdit;
};

#endif // TIMINGSUMMARYDIALOG_H
