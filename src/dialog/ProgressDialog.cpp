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
    m_progressValue = 0;
    m_label = new QLabel(this);
    m_label->move(20, 20); // 设置Label位置
}

void ProgressDialog::setTitle(const QString &title)
{
    m_title = title;
    setWindowTitle(title);
}

void ProgressDialog::setProgressText(const QString &text)
{
    m_progressText = text;
    m_label->setText(text);
    // setLabelText(text);
}

void ProgressDialog::setProgressValue(int value)
{
    m_progressValue = value;
    setValue(value);
}
