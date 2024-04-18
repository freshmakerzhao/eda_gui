/**
  ******************************************************************************
  * @file           : ProgressDialog.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/14
  ******************************************************************************
  */

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QtWidgets>

class ProgressDialog : public QProgressDialog
{
    Q_OBJECT
public:
    ProgressDialog(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setProgressText(const QString &text);
    void setProgressValue(int value);

private:
    QString m_title;
    QString m_progressText;
    int m_progressValue;
    QLabel *m_label;
};

#endif // PROGRESSDIALOG_H
