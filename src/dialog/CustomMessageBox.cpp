#include "CustomMessageBox.h"

CustomMessageBox::CustomMessageBox(QWidget *parent) : QMessageBox(parent)
{

}


QMessageBox::StandardButton CustomMessageBox::showInformation(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(Information);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    return static_cast<StandardButton>(messageBox.exec());
}

QMessageBox::StandardButton CustomMessageBox::showQuestion(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(Question);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);

    // 设置整体样式，包括背景和文字颜色
    messageBox.setStyleSheet("QMessageBox { "
                             "   color: black; "
                             "   background: #f7f7f7; "
                             "   border-top: 3px solid #e7e7e7;"  // 顶部边框
                             "}"
                             "QPushButton {"
                             "   background-color: #4f7cce; "   // 背景色
                             "   color: white; "                // 白色文字
                             "   font-size: 10pt;"              // 字号
                             "   border-width: 2px; "           // 边框宽度
                             "   border-color: #4f7cce; "       // 边框颜色
                             "   border-style: solid; "         // 边框样式
                             "   box-shadow: none;"             // 没有阴影
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
                             "   box-shadow: none;"             // 没有阴影
                             "   min-width: 60px; "             // 最小宽度
                             "   padding: 4px;"                // 内边距
                             "   margin: 2px;"
                             "}"
    );

    return static_cast<StandardButton>(messageBox.exec());
}

QMessageBox::StandardButton CustomMessageBox::showSuccess(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    // 设置整体样式，包括背景和文字颜色
    messageBox.setStyleSheet("QMessageBox { "
                             "   color: black; "
                             "   background: #f7f7f7; "
                             "   border-top: 3px solid #e7e7e7;"  // 顶部边框
                             "}"
                             "QLabel { "
                             "   padding-top: 20px; "
                             "}"
                             "QPushButton {"
                             "   background-color: #4f7cce; "   // 背景色
                             "   color: white; "                // 白色文字
                             "   font-size: 10pt;"              // 字号
                             "   border-width: 2px; "           // 边框宽度
                             "   border-color: #4f7cce; "       // 边框颜色
                             "   border-style: solid; "         // 边框样式
                             "   box-shadow: none;"             // 没有阴影
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
                             "   box-shadow: none;"             // 没有阴影
                             "   min-width: 60px; "             // 最小宽度
                             "   padding: 4px;"                // 内边距
                             "   margin: 2px;"
                             "}"
    );
    return static_cast<StandardButton>(messageBox.exec());
}

QMessageBox::StandardButton CustomMessageBox::showError(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(Critical);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    return static_cast<StandardButton>(messageBox.exec());
}

void CustomMessageBox::resizeEvent(QResizeEvent *event){
    QMessageBox::resizeEvent(event);
    this->setMinimumWidth(390);
    this->setMinimumHeight(100);
}

