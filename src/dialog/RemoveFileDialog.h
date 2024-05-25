/**
  ******************************************************************************
  * @file           : RemoveFileDialog.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/24
  ******************************************************************************
  */
#ifndef REMOVEFILEDIALOG_H
#define REMOVEFILEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class RemoveFileDialog : QDialog
{
    Q_OBJECT
public:
    RemoveFileDialog(QWidget *parent = nullptr, const QString &path = "");

    int exec() override;

private:
    QLabel *filePathLabel;
    QCheckBox *checkBox;
    int op = 0;
};

#endif // REMOVEFILEDIALOG_H
