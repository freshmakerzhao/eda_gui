/**
  ******************************************************************************
  * @file           : ConstraintPage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#ifndef CONSTRAINTPAGE_H
#define CONSTRAINTPAGE_H

#include "Wizard.h"

class ConstraintPage : public QWizardPage
{
    Q_OBJECT
public:
    ConstraintPage(QWidget *parent = nullptr,
                   const WizardMode &wizardMode = WizardMode::CREATE_PROJECT);

private:
    // QListWidget *filesListWidget;

    QStandardItemModel *model;

    WizTableView *tableView;

    WizardMode _wizardMode;

    const QMap<QString, QString> Map = {
        {"xdc", "xdc"},
        {"hdc", "hdc"}
    };

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();
    // void updateFilesList(const QStringList &files);

// signals:
//     void filesListUpdatedSignal(const QStringList &files);
protected:
    int nextId() const override;
};

#endif // CONSTRAINTPAGE_H
