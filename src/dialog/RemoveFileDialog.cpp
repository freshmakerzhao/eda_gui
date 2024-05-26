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
    label->setFixedHeight(25);
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

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    okButton->setStyleSheet("QPushButton {"
                            "   background-color: #4f7cce; "   // 背景色
                            "   color: white; "                // 白色文字
                            "   font-size: 10pt;"              // 字号
                            "   border-width: 2px; "           // 边框宽度
                            "   border-color: #4f7cce; "       // 边框颜色
                            "   border-style: solid; "         // 边框样式
                            "   min-width: 60px; "             // 最小宽度
                            "   padding: 6px; "                // 内边距
                            "}"
                            "QPushButton:hover {"
                            "   background-color: #3a5b98; "   // 背景色
                            "   color: white; "                // 白色文字
                            "   font-size: 10pt;"              // 字号
                            "   border-width: 2px; "           // 边框宽度
                            "   border-color: #4f7cce; "       // 边框颜色
                            "   border-style: solid; "         // 边框样式
                            "   min-width: 60px; "             // 最小宽度
                            "   padding: 4px;"                // 内边距
                            "   margin: 2px;"
                            "}");
    cancelButton->setStyleSheet("QPushButton {"
                                "   background-color: #ffffff; "   // 背景色
                                "   color: black; "                // 白色文字
                                "   font-size: 10pt;"              // 字号
                                "   border-width: 1px; "           // 边框宽度
                                "   border-color: black; "       // 边框颜色
                                "   border-style: solid; "         // 边框样式
                                "   width: 60px; "                 // 宽度
                                "   height: 20px; "                 // 高度
                                "   padding: 6px; "                // 内边距
                                "}"
                                "QPushButton:hover {"
                                "   background-color: #ededed; "   // 背景色
                                "   color: black; "                // 白色文字
                                "   font-size: 10pt;"              // 字号
                                "   border-width: 1px; "           // 边框宽度
                                "   border-color: black; "       // 边框颜色
                                "   border-style: solid; "         // 边框样式
                                "   width: 60px; "                 // 宽度
                                "   height: 20px; "                 // 高度
                                "   padding: 4px;"                // 内边距
                                "}");
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [this](){
        op = 1; // 仅移除
        if (checkBox->isChecked()) { // 移除+删除
            op = 2;
        }
        this->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, [this](){
        op = -1; // 忽略
        this->accept();
    });
}

int RemoveFileDialog::exec()
{
    QDialog::exec();
    return op;
}
