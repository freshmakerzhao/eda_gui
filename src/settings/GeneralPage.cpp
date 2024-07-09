#include "GeneralPage.h"
#include "utils/ProjectManager.h"
#include "wizard/Wizard.h"

GeneralPage::GeneralPage(QWidget *parent)
{
    setStyleSheet(".QWidget { background-image: url(:/resource/white.png); }"
                  ".QWidget { border:4px solid #DCDCDC; }"
    );
    projectDeviceButton = new QPushButton(this);
    projectDeviceButton->setFixedHeight(25);
    projectDeviceButton->setStyleSheet(
            "QPushButton { "
                "text-align: left; "
                "background-color: rgb(239, 239, 239);"
                "border: None;"
            "} "
    );

    projectDeviceSquareButton = new QPushButton(this);
    projectDeviceSquareButton->setFixedSize(25, 25); // 设置为方形
    projectDeviceSquareButton->setStyleSheet(
            "QPushButton { "
                "text-align: center; "
                "background-color: rgb(255, 255, 255);"
                "border: 1px solid black;"
            "} "
            "QPushButton:hover { "
                "text-align: center; "
                "background-color: rgb(237, 237, 237);"
                "border: 1px solid black;"
            "} "
    );

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(projectDeviceButton);
    hLayout->addSpacing(5); // 添加一点间距
    hLayout->addWidget(projectDeviceSquareButton);

    topModuleNameLineEdit = new QLineEdit(this);
    topModuleNameLineEdit->setClearButtonEnabled(true);
    topModuleNameLineEdit->setFixedHeight(25);

    fLayout = new QFormLayout(this);
    fLayout->addRow("Project device:", hLayout);
    fLayout->addRow("Top module name:", topModuleNameLineEdit);

    QString topName = ProjectManager::instance().getTopModule();
    QString deviceInfo = ProjectManager::instance().getDeviceInfo();
    qDebug() << "[GeneralPage] GeneralPage device info :" << deviceInfo;
    projectDeviceButton->setText(deviceInfo);
    projectDeviceSquareButton->setText("...");
    topModuleNameLineEdit->setText(topName);
}

void GeneralPage::setTopModule()
{
    ProjectManager::instance().setTopModule(topModuleNameLineEdit->text());
}



