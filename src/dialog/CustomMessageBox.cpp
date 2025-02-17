#include "CustomMessageBox.h"
#include <QDebug>
#include <algorithm>

const QString CustomMessageBox::DEFAULT_BUTTON_STYLESHEET =
    "QPushButton {"
    "   background-color: #ffffff; "
    "   color: black; "
    "   border: 1px solid #8c8c8c; "
    "}"
    "QPushButton:hover {"
    "   background-color: #ededed; "
    "   color: black; "
    "}";

const QString CustomMessageBox::HIGHLIGHT_BUTTON_STYLESHEET =
    "QPushButton {"
    "   background-color: #4f7cce; "
    "   color: white; "
    "   border: 1px solid #4f7cce; "
    "}"
    "QPushButton:hover {"
    "   background-color: #3a5b98; "
    "   color: white; "
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

QMessageBox::StandardButton CustomMessageBox::information(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    return showCustomMessage(parent, title, text, Icon::Information, buttons, defaultButton);
}

CustomMessageBox::StandardButton CustomMessageBox::warning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    return showCustomMessage(parent, title, text, Icon::Warning, buttons, defaultButton);
}

CustomMessageBox::StandardButton CustomMessageBox::question(QWidget *parent, const QString &title,
                                                            const QString &text, StandardButtons buttons,
                                                            StandardButton defaultButton)
{
    return showCustomMessage(parent, title, text, Icon::Question, buttons, defaultButton);
}

CustomMessageBox::StandardButton CustomMessageBox::critical(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons, StandardButton defaultButton)
{
    return showCustomMessage(parent, title, text, Icon::Critical, buttons, defaultButton);
}

CustomMessageBox::StandardButton CustomMessageBox::showCustomMessage(QWidget *parent, const QString &title, const QString &text, Icon icon, StandardButtons buttons, StandardButton defaultButton)
{
    CustomMessageBox messageBox(parent);
    messageBox.setIcon(icon);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
    messageBox.setButtonStyleSheet(messageBox);
    return static_cast<StandardButton>(messageBox.exec());
}

void CustomMessageBox::setButtonStyleSheet(CustomMessageBox &messageBox)
{
    QSize sizeHint = QSize(0, 0);

    QList<QAbstractButton*> buttons = messageBox.buttons();
    for (QAbstractButton* button : buttons) {
        sizeHint.setWidth(std::max(sizeHint.width(), button->sizeHint().width()));
        sizeHint.setHeight(std::max(sizeHint.height(), button->sizeHint().height()));
        button->setStyleSheet(DEFAULT_BUTTON_STYLESHEET);
    }

    auto *yesButton = messageBox.button(QMessageBox::Yes);
    if (yesButton) {
        yesButton->setStyle(QApplication::style());
        yesButton->setStyleSheet(HIGHLIGHT_BUTTON_STYLESHEET);
    }

    auto *okButton = messageBox.button(QMessageBox::Ok);
    if (okButton) {
        okButton->setStyle(QApplication::style());
        okButton->setStyleSheet(HIGHLIGHT_BUTTON_STYLESHEET);
    }

    for (QAbstractButton* button : buttons) {
        button->setFixedSize(sizeHint);
    }
}

