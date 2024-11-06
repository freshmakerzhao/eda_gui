#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QScrollBar>
#include <QScrollArea>

class BasePage : public QWidget
{
public:
    BasePage(QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
};

#endif // BASEPAGE_H
