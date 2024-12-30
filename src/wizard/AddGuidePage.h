/**
  ******************************************************************************
  * @file           : AddGuidePage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/20
  ******************************************************************************
  */

#ifndef ADDGUIDEPAGE_H
#define ADDGUIDEPAGE_H

#include <QButtonGroup>
#include <QRadioButton>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QDebug>

class AddGuidePage : public QWizardPage
{
    Q_OBJECT
public:
    AddGuidePage(QWidget *parent = nullptr);

protected:
    int nextId() const override;

private:
    QRadioButton *radioBtn1;
    QRadioButton *radioBtn2;
    QRadioButton *radioBtn3;
};

#endif // ADDGUIDEPAGE_H
