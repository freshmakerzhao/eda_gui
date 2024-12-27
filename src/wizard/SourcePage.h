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

enum eAddSourceType {
    AddSources,
    AddSimulationSources
};

class SourcesPage : public QWizardPage
{
    Q_OBJECT
public:
    SourcesPage( QWidget *parent = nullptr, const int mode = 0, const eAddSourceType sourceType = eAddSourceType::AddSources);

private:
    QStandardItemModel *model;

    WizTableView *tableView;

    int _mode;
    eAddSourceType mSourceType;

    const QMap<QString, QString> Map = {
        {"v", "verilog"}
    };
    QStringList& getMatchFileList( );

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();

protected:
    int nextId() const override;


};

#endif // SOURCEPAGE_H
