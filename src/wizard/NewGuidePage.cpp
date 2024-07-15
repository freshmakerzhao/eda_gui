/**
  ******************************************************************************
  * @file           : NewGuidePage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/20
  ******************************************************************************
  */

#include <QLabel>
#include <QVBoxLayout>
#include "NewGuidePage.h"

NewGuidePage::NewGuidePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Create a New HybrdLink Project");
    setSubTitle("This wizard will guide you through the creation of a new project.");

    QLabel *bodyLabel = new QLabel("To create a HybrdLink project you will need to provide a name and a "
                                   "location for your project files. Finally, you will specify your project "
                                   "sources and choose a default part.");
//    QPixmap pixmap(":/resource/leftPixmap.jpg"); // 确保将图片路径替换为实际路径
//    setPixmap(QWizard::WatermarkPixmap, pixmap);
    bodyLabel->setWordWrap(true);
    bodyLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(bodyLabel);
    setLayout(layout);
}
