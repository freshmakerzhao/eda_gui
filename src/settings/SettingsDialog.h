#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include "GeneralPage.h"
#include "BitstreamSettingPage.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void accept() override;

signals:
public slots:

private:
    QStackedWidget *stackedWidget;
    QTreeWidget *treeWidget;
    GeneralPage *generalPage;
    BitstreamSettingPage *bitstreamSettingPage;
};

#endif // SETTINGSDIALOG_H
