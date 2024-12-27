/**
  ******************************************************************************
  * @file           : Wizard.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/9
  ******************************************************************************
  */

#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QFormLayout>
#include <QDebug>
#include "WizCommon.h"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    Wizard(QWidget *parent = nullptr, const int &mode = 0);
    ~Wizard();

private slots:
    void onNewFinish();

    void onAddFinish();

    // void onPartFinish();

public:
    QStringList getDeviceInfo() const;

    QStringList sourcesFilesList;
    QStringList constraintFilesList;
    QStringList simFileList;
    QString part;
    QString displayPart;
    QString familyName;
    QString archName;
    QString arch;

    enum {
        Page_AddGuide,
        Page_NewGuide,
        Page_ProjectName,
        Page_Source,
        Page_Constraint,
        Page_Simulation_Source,
        Page_DefaultPart
    };

private:

};

#endif // WIZARD_H
