#ifndef SWITCHINGTAB_H
#define SWITCHINGTAB_H

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
#include <QPushButton>
#include <QScrollBar>
#include <QScrollArea>
#include <QSpinBox>

class SwitchingTab : public QWidget
{
public:
    SwitchingTab(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;

    const QString TITLE_LABEL_STYLESHEET = "QLabel{"
                                           "font-size: 20px;"
                                           "font-weight: bold;"
                                           "border-bottom: 2px solid #DCDCDC;"
                                           "border-radius: 0px;"
                                           "}";
};

#endif // SWITCHINGTAB_H
