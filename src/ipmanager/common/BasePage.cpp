/**
  ******************************************************************************
  * @file           : BasePage.cpp
  * @author         : ksy
  * @description    : Base class for all ip configuration pages.
  * @attention      : None
  * @date           : 2024/11/7
  ******************************************************************************
  */
#include "BasePage.h"

BasePage::BasePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    vBoxLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);
    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addSpacing(10);
}
