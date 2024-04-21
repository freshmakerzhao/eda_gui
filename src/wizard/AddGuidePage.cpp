/**
  ******************************************************************************
  * @file           : AddGuidePage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/20
  ******************************************************************************
  */

#include "AddGuidePage.h"
#include "Wizard.h"

AddGuidePage::AddGuidePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Add Sources");
    setSubTitle("This guides you through the process of adding"
                " and creating sources for your project.");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(40);
    QButtonGroup *buttonGroup = new QButtonGroup(layout);
    radioBtn1 = new QRadioButton("Add or create design sources");
    radioBtn2 = new QRadioButton("Add or create constraints");

    radioBtn1->setChecked(true);

    buttonGroup->addButton(radioBtn1);
    buttonGroup->addButton(radioBtn2);
    // 将按钮组的 exclusive 属性设置为 true，确保只能选中一个选项
    buttonGroup->setExclusive(true);

    layout->addWidget(radioBtn1);
    layout->addSpacing(15);     // 设置间距
    layout->addWidget(radioBtn2);
}

int AddGuidePage::nextId() const
{
    if (radioBtn1->isChecked()) {
        return Wizard::Page_Source;
    } else if (radioBtn2->isChecked()) {
        return Wizard::Page_Constraint;
    }

    return -1;  // 或者返回其他值，表示没有下一页
}



