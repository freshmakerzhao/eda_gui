#ifndef REPORTPOWERDIALOG_H
#define REPORTPOWERDIALOG_H

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
#include "EnvironmentTab.h"
#include "SwitchingTab.h"

class ReportPowerDialog : public QDialog
{
public:
    ReportPowerDialog(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;

    EnvironmentTab *environmentTab;

    SwitchingTab *switchingTab;

    QLineEdit *resultsNameLineEdit;
};

#endif // REPORTPOWERDIALOG_H
