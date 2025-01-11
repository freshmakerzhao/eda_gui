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

enum AddSourceType {
    AddSources,
    AddSimulationSources
};

class SourcesPage : public QWizardPage
{
    Q_OBJECT
public:
    SourcesPage(QWidget *parent = nullptr,
                const WizardMode &wizardMode = WizardMode::CREATE_PROJECT,
                const AddSourceType sourceType = AddSourceType::AddSources);

private:
    QStandardItemModel *model;

    WizTableView *tableView;

    WizardMode _wizardMode;
    AddSourceType _sourceType;

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
