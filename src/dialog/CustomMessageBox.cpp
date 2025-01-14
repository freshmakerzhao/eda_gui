#include "CustomMessageBox.h"

const QString CustomMessageBox::OK_AND_YES_BUTTON_STYLESHEET =
    "QPushButton {"
    "   background-color: #4f7cce; "
    "   color: white; "
    "}"
    "QPushButton:hover {"
    "   background-color: #3a5b98; "
    "   color: white; "
    "}";

const QString CustomMessageBox::CANCEL_AND_NO_BUTTON_STYLESHEET =
    "QPushButton {"
    "   background-color: #ffffff; "
    "   color: black; "
    "}"
    "QPushButton:hover {"
    "   background-color: #ededed; "
    "   color: black; "
    "}";

CustomMessageBox::CustomMessageBox(QWidget *parent) : QMessageBox(parent)
{
    // 设置整体样式，包括背景和文字颜色
    setStyleSheet("QMessageBox { "
                  "   color: black; "
                  "   background: #f7f7f7; "
                  "   border-top: 3px solid #e7e7e7;"  // 顶部边框
                  "}"
                  "QLabel{"
                  "   padding-top: 20px;"
                  "}"
                  );
}

QMessageBox::StandardButton CustomMessageBox::showWarning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(Warning);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    setButtonStyleSheet(messageBox);
    return static_cast<StandardButton>(messageBox.exec());
}

QMessageBox::StandardButton CustomMessageBox::showInformation(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(Information);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    setButtonStyleSheet(messageBox);
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
    setButtonStyleSheet(messageBox);
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
    setButtonStyleSheet(messageBox);
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
    setButtonStyleSheet(messageBox);
    return static_cast<StandardButton>(messageBox.exec());
}

void CustomMessageBox::resizeEvent(QResizeEvent *event){
    QMessageBox::resizeEvent(event);
    this->setMinimumWidth(390);
    this->setMinimumHeight(100);
}

void CustomMessageBox::setButtonStyleSheet(CustomMessageBox &messageBox)
{
    if (messageBox.button(QMessageBox::Yes)) {
        messageBox.button(QMessageBox::Yes)->setStyleSheet(OK_AND_YES_BUTTON_STYLESHEET);
    }

    if (messageBox.button(QMessageBox::Ok)) {
        messageBox.button(QMessageBox::Ok)->setStyleSheet(OK_AND_YES_BUTTON_STYLESHEET);
    }

    if (messageBox.button(QMessageBox::No)) {
        messageBox.button(QMessageBox::No)->setStyleSheet(CANCEL_AND_NO_BUTTON_STYLESHEET);
    }

    if (messageBox.button(QMessageBox::Cancel)) {
        messageBox.button(QMessageBox::Cancel)->setStyleSheet(CANCEL_AND_NO_BUTTON_STYLESHEET);
    }
}

