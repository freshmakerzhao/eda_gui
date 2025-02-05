#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include "GeneralSettingsPage.h"
#include "TextEditorSettingsPage.h"
#include "BitstreamSettingsPage.h"

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
    GeneralSettingsPage *generalSettingsPage;
    TextEditorSettingsPage *textEditorSettingsPage;
    BitstreamSettingsPage *bitstreamSettingsPage;
};

#endif // SETTINGSDIALOG_H
