#ifndef TEXTEDITORSETTINGSPAGE_H
#define TEXTEDITORSETTINGSPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QComboBox>
#include <QSettings>

class TextEditorSettingsPage : public QWidget
{
public:
    TextEditorSettingsPage(QWidget *parent = nullptr);

    void setEncoding();

private:
    QComboBox *encodingComboBox;
};

#endif // TEXTEDITORSETTINGSPAGE_H
