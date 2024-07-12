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

class RemoveFileDialog :  public QDialog
{
    Q_OBJECT
public:
    RemoveFileDialog(QWidget *parent = nullptr, const QString &path = "");

    int exec() override;

    enum Result {
        AcceptedChecked,
        AcceptedUnchecked,
        Rejected
    };

private slots:
    void accept() override;

    void reject() override;

private:
    Result result = Rejected;
    QLabel *filePathLabel;
    QCheckBox *checkBox;
};

#endif // REMOVEFILEDIALOG_H
