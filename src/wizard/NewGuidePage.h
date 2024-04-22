/**
  ******************************************************************************
  * @file           : NewGuidePage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/20
  ******************************************************************************
  */

#ifndef NEWGUIDEPAGE_H
#define NEWGUIDEPAGE_H

#include <QWizardPage>
#include <QDebug>

class NewGuidePage : public QWizardPage
{
    Q_OBJECT
public:
    NewGuidePage(QWidget *parent = nullptr);
};

#endif // NEWGUIDEPAGE_H
