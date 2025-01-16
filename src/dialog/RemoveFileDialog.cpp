/**
  ******************************************************************************
  * @file           : RemoveFileDialog.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/24
  ******************************************************************************
  */
#include "RemoveFileDialog.h"

RemoveFileDialog::RemoveFileDialog(QWidget *parent, const QString &path)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    setFixedWidth(800);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setWindowTitle("Remove File");
    QLabel *label = new QLabel("Do you want to remove this file from project?", this);
    filePathLabel = new QLabel(path, this); // 显示目标文件的路径
    filePathLabel->setGeometry(QRect(1, 1, 1, 1));
    filePathLabel->setWordWrap(true);
    filePathLabel->setAlignment(Qt::AlignTop);
    // filePathLabel->adjustSize();
    checkBox = new QCheckBox("Completely delete this file", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->insertSpacing(1, 20); // 在两个Label之间插入间距
    // layout->setSpacing(0);
    layout->addWidget(filePathLabel);
    layout->addWidget(checkBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &RemoveFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &RemoveFileDialog::reject);
}

int RemoveFileDialog::exec()
{
    QDialog::exec();
    return result;
}

void RemoveFileDialog::accept()
{
    if (checkBox->isChecked()) {
        result = AcceptedChecked;
    } else {
        result = AcceptedUnchecked;
    }
    QDialog::accept();
}

void RemoveFileDialog::reject()
{
    result = Rejected;
    QDialog::reject();
}
