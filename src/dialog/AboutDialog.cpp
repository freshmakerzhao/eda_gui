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
    QString buildDateTime = QString("%1 %2").arg(__DATE__, __TIME__);
    QString compiler = "<unknown>";
    #if defined(Q_CC_GNU)
        compiler = QLatin1String("GCC ") + QLatin1String(__VERSION__);
    #endif
    QString compilerString = QString("%1-%2bit").arg(compiler, QString::number(QSysInfo::WordSize));
    QString version = QApplication::applicationVersion().isEmpty() ? "unknown" : QApplication::applicationVersion();
    QString information = "<html>"
                          "<h2>About HybrdLink</h2"
                          "<p>Build Date Time: %1</p>"
                          "<p>Compiler: %2</p>"
                          "<p>Version: %3</p>"
                          "<p>© 2024 Power by HybrdChip Technology (Zhuhai) Co., Ltd.</p>"
                          "<p><a href='https://www.hybrdchip.com/about'>https://www.hybrdchip.com/about</a></p>"
                          "</html>";
    QLabel *textLabel = new QLabel(this);
    textLabel->setText(information.arg(buildDateTime, compilerString, version));
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textLabel->setOpenExternalLinks(true);

    QLabel *imageLabel = new QLabel(this);
    imageLabel->setFixedHeight(300);
    QPixmap image(":/resource/logo.png");
    // 缩放图片到当前分辨率下的显示大小，SmoothTransformation平滑处理。
    imageLabel->setPixmap(image.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
