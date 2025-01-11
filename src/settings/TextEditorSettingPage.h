#ifndef TEXTEDITORSETTINGPAGE_H
#define TEXTEDITORSETTINGPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QComboBox>
#include <QSettings>

class TextEditorSettingPage : public QWidget
{
public:
    TextEditorSettingPage(QWidget *parent = nullptr);

    void setEncoding();

private:
    QComboBox *encodingComboBox;
};

#endif // TEXTEDITORSETTINGPAGE_H
