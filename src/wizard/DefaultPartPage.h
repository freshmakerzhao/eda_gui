/**
  ******************************************************************************
  * @file           : DefaultPartPage.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#ifndef DEFAULTPARTPAGE_H
#define DEFAULTPARTPAGE_H

#include "Wizard.h"
#include <QRegExp>

class DefaultPartPage :  public QWizardPage
{
    Q_OBJECT
public:
    DefaultPartPage(QWidget *parent = nullptr);

protected:
    bool isComplete()  const override;

private:
    QTableView *tableView;
    QString partFile = ":/resource/parts_hybrdchip_for_display.yaml";
    QLineEdit *lineEdit;
    QLabel *matchesLabel;
    QRadioButton *compatibility_mode_button;
    QLabel *compatibility_mode_label;
    void onCompatibilityModeToggled(bool checked);
private slots:
    void selectPart(const QModelIndex &index);

};

#endif // DEFAULTPARTPAGE_H
