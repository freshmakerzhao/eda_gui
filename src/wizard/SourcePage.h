/**
  ******************************************************************************
  * @file           : SourcesPage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#ifndef SOURCEPAGE_H
#define SOURCEPAGE_H

#include "Wizard.h"

class SourcesPage : public QWizardPage
{
    Q_OBJECT
public:
    SourcesPage(QWidget *parent = nullptr, const int mode = 0);

private:
    QStandardItemModel *model;

    QTableView *tableView;

    int _mode;

    const QMap<QString, QString> Map = {
        {"v", "verilog"}
    };

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();

protected:
    int nextId() const override;


};

#endif // SOURCEPAGE_H
