#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(640, 480);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    setWindowTitle("About Software");
    QLabel *textLabel = new QLabel(this);
    textLabel->setText("<html><h2>About Software</h2"
                       "<p>© 2024 Power by HybrdChip</p>"
                       "<p><a href='https://www.hybrdchip.com/about'>https://www.hybrdchip.com/about</a>"
                       "</p></html>");
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textLabel->setOpenExternalLinks(true);

    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/resource/logo.png");
    imageLabel->setPixmap(image.scaled(500, 300));
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
