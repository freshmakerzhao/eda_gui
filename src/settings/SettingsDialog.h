#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include "base/Globals.h"
#include "GeneralPage.h"
#include "TextEditorSettingPage.h"
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
    TextEditorSettingPage *textEditorSettingPage;
    BitstreamSettingPage *bitstreamSettingPage;
};

#endif // SETTINGSDIALOG_H
