/**
  ******************************************************************************
  * @file           : BasePage.h
  * @author         : ksy
  * @description    : Base class for all ip configuration pages.
  * @attention      : None
  * @date           : 2024/11/7
  ******************************************************************************
  */
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

    const QString TITLE_LABEL_STYLESHEET = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;"
                                  "}";
};

#endif // BASEPAGE_H
