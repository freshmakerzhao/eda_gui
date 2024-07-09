#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include "GeneralPage.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    // int exec() override;

signals:
public slots:

private:
    QStackedWidget *stackedWidget;
    QTreeWidget *treeWidget;
    GeneralPage *generalPage;
};

#endif // SETTINGSDIALOG_H
