/**
  ******************************************************************************
  * @file           : ConfigurationPage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2025/1/11
  ******************************************************************************
  */
#ifndef CONFIGURATIONPAGE_H
#define CONFIGURATIONPAGE_H

#include "Wizard.h"
#include <QCheckBox>

class ConfigurationPage :  public QWizardPage
{
    Q_OBJECT
public:
    ConfigurationPage(QWidget *parent = nullptr);

private slots:
    void onCompatibilityModeCheckBoxClicked(bool checked);
private:
    QCheckBox *compatibilityModeCheckBox;
    QLabel *compatibilityModeLabel;
};

#endif // CONFIGURATIONPAGE_H
