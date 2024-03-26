#ifndef SOURCEPAGE_H
#define SOURCEPAGE_H

#include "Wizard.h"

class SourcesPage : public QWizardPage
{
    Q_OBJECT
public:
    SourcesPage(QWidget *parent = nullptr);

private:
    QListWidget *filesListWidget;

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();
    void updateFilesList(const QStringList &files);

signals:
    void filesListUpdatedSignal(const QStringList &files);
};

#endif // SOURCEPAGE_H
