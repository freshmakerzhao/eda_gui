/**
  ******************************************************************************
  * @file           : AboutDialog.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/21
  ******************************************************************************
  */

#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setStyleSheet("QWidget { background-image: url(:/resource/white.png); }");
    setFixedSize(720, 540);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    setWindowTitle("About Software");
    QLabel *textLabel = new QLabel(this);
    textLabel->setText("<html><h2>About HybrdLink</h2"
                       "<p>© 2024 Power by HybrdChip</p>"
                       "<p>Zhongke Xin Magnetic Technology Co., Ltd.</p>"
                       "<p><a href='https://www.hybrdchip.com/about'>https://www.hybrdchip.com/about</a>"
                       "</p></html>");
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textLabel->setOpenExternalLinks(true);

    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/resource/logo.png");
    imageLabel->setPixmap(image.scaled(700, 496));
    imageLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(textLabel);
    layout->addWidget(imageLabel);
}

int AboutDialog::exec()
{
    QDialog::exec();
    return 0;
}
