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
    ConstraintPage(QWidget *parent = nullptr);

private:
    // QListWidget *filesListWidget;

    QStandardItemModel *model;

    QTableView *tableView;

    const QMap<QString, QString> Map = {
        {"xdc", "xdc"}
    };

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();
    // void updateFilesList(const QStringList &files);

// signals:
//     void filesListUpdatedSignal(const QStringList &files);
};

#endif // CONSTRAINTPAGE_H
