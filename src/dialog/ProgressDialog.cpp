/**
  ******************************************************************************
  * @file           : ProgressDialog.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/14
  ******************************************************************************
  */

#include "ProgressDialog.h"

ProgressDialog::ProgressDialog(QWidget *parent)
    : QProgressDialog(parent)
{
    setFixedSize(QSize(800, 140));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setValue(0);    //初始化值为0
    _progressValue = 0;
    _label = new QLabel(this);
    _label->move(20, 20); // 设置Label位置
}

void ProgressDialog::setTitle(const QString &title)
{
    _title = title;
    setWindowTitle(title);
}

void ProgressDialog::setProgressText(const QString &text)
{
    _progressText = text;
    _label->setText(text);
    // setLabelText(text);
}

void ProgressDialog::setProgressValue(int value)
{
    _progressValue = value;
    setValue(value);
}
