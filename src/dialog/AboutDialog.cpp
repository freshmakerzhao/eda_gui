/**
  ******************************************************************************
  * @file           : AboutDialog.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/21
  ******************************************************************************
  */

#include <QMessageBox>
#include <iostream>
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
#elif defined(Q_CC_MSVC)
    compiler = QLatin1String("MSVC ") + QString::number(_MSC_VER);
#else
    compiler = QLatin1String("unknown");
#endif
    QString compilerString = QString("%1-%2bit").arg(compiler, QString::number(QSysInfo::WordSize));
    QString version = QApplication::applicationVersion().isEmpty() ? "unknown" : QApplication::applicationVersion();
    QString information = "<html>"
                          "<h2>About HybrdLink</h2>"
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
    imageLabel->setPixmap(image.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    int *clickCount = new int(0);
    imageLabel->installEventFilter(this);

    this->setProperty("easterEgg_clickCount", QVariant::fromValue(static_cast<void*>(clickCount)));
    this->setProperty("easterEgg_label", QVariant::fromValue(static_cast<void*>(imageLabel)));

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(textLabel);
    layout->addWidget(imageLabel);
}

static QString decodeBase64(const QString &encoded) {
    return QString::fromUtf8(QByteArray::fromBase64(encoded.toUtf8()));
}

bool AboutDialog::eventFilter(QObject *obj, QEvent *event)
{
    auto imageLabel = static_cast<QLabel*>(this->property("easterEgg_label").value<void*>());
    auto clickCount = static_cast<int*>(this->property("easterEgg_clickCount").value<void*>());

    if (obj == imageLabel && event->type() == QEvent::MouseButtonPress) {
        (*clickCount)++;
        if (*clickCount >= 7) {
            QString title = decodeBase64("8J+RqOKAjfCfkrsg5byA5Y+R6ICF5ZCN5Y2V");
            QString content = decodeBase64("8J+OiSDlvanom4vop6blj5HvvIEKCvCfkaQg6LW15biFCvCfkaQg5p+v5bCa5rKF");
            QMessageBox::information(this, title, content);
            *clickCount = 0;
        }
        return true;
    }
    return QDialog::eventFilter(obj, event);
}