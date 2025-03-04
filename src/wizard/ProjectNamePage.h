/**
  ******************************************************************************
  * @file           : ProjectNamePage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#ifndef PROJECTNAMEPAGE_H
#define PROJECTNAMEPAGE_H

#include "Wizard.h"

class ProjectNamePage : public QWizardPage
{
    Q_OBJECT
public:
    ProjectNamePage(QWidget *parent = nullptr);

protected:
    bool isComplete() const override;

private:
    QString autoSetName(const QString &projectDir);

    QLineEdit *nameLineEdit;
    QLineEdit *pathLineEdit;
    QLabel *hintLabel;

    bool enableAutoSetName = true;
};

#endif // PROJECTNAMEPAGE_H
