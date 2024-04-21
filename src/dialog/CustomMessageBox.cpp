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
    this->setFixedWidth(600);
    this->setFixedHeight(300);
}

