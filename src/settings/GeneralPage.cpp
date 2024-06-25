#include "GeneralPage.h"
#include "utils/ProjectManager.h"

GeneralPage::GeneralPage(QWidget *parent)
{
    setStyleSheet(".QWidget { background-image: url(:/resource/white.png); }"
                  ".QWidget { border:4px solid #DCDCDC; }"
    );
    project_device = new QPushButton(this);
    project_device->setFixedHeight(25);
    project_device->setStyleSheet(
            "QPushButton { "
                "text-align: left; "
                "background-color: rgb(239, 239, 239);"
                "border: None;"
            "} "
    );


    project_device_square_button = new QPushButton(this);
    project_device_square_button->setFixedSize(25, 25); // 设置为方形
    project_device_square_button->setStyleSheet(
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

    // 将 project_device 和 project_device_square_button 放在一起
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(project_device);
    hLayout->addSpacing(5); // 添加一点间距
    hLayout->addWidget(project_device_square_button);

    top_module_name = new QLineEdit(this);
    top_module_name->setFixedHeight(25);

    fLayout = new QFormLayout(this);
    fLayout->addRow("Project device:", hLayout);
    fLayout->addRow("Top module name:", top_module_name);

    QString topName = ProjectManager::instance().getTopModule();
    QString deviceInfo = ProjectManager::instance().getDeviceInfo();
    qDebug() << "[GeneralPage] GeneralPage device info :" << deviceInfo;
    project_device->setText(deviceInfo);
    project_device_square_button->setText("...");
    top_module_name->setText(topName);
}

void GeneralPage::setTopModule()
{
    // TaskManager::instance().setTopModule(top_module_name->text());
    ProjectManager::instance().setTopModule(top_module_name->text());
}
