#include "GeneralPage.h"
#include "utils/ProjectManager.h"
#include "wizard/Wizard.h"

GeneralPage::GeneralPage(QWidget *parent)
{
    setStyleSheet(".QWidget { background-image: url(:/resource/white.png); }"
                  ".QWidget { border:4px solid #DCDCDC; }"
    );
    // QLabel *label = new QLabel("General Specify values for various settings used throughout the design flow. "
    //                            "These settingsapply to the current project.", this);
    // label->setWordWrap(true);
    projectDeviceLineEdit = new QLineEdit(this);
    projectDeviceLineEdit->setReadOnly(true);
    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/icons/resource/icons/28-icon_grid_view.png"));
    projectDeviceLineEdit->addAction(action, QLineEdit::LeadingPosition);

    projectDeviceSquareButton = new QPushButton(this);
    connect(projectDeviceSquareButton, &QPushButton::clicked, this, &GeneralPage::startWizard);
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
    hLayout->addWidget(projectDeviceLineEdit);
    hLayout->addSpacing(5); // 添加一点间距
    hLayout->addWidget(projectDeviceSquareButton);

    topModuleNameLineEdit = new QLineEdit(this);
    topModuleNameLineEdit->setClearButtonEnabled(true);
    topModuleNameLineEdit->setFixedHeight(25);

    fLayout = new QFormLayout(this);
    // fLayout->addWidget(label);
    fLayout->addRow("Project device:", hLayout);
    fLayout->addRow("Top module name:", topModuleNameLineEdit);

    QString topName = ProjectManager::instance().getParameter(Project::TopModule);
    QString deviceInfo = QString("%1 (active)").arg(ProjectManager::instance().getParameter(Project::DisplayPart));
    projectDeviceLineEdit->setText(deviceInfo);
    projectDeviceSquareButton->setText("...");
    topModuleNameLineEdit->setText(topName);
}

void GeneralPage::setTopModule()
{
    ProjectManager::instance().setTopModule(topModuleNameLineEdit->text());
}

void GeneralPage::setDevicePart()
{
    if (deviceInfo.isEmpty()) return;
    if (deviceInfo.first().isEmpty()) return;
    ProjectManager::instance().setDevicePart(deviceInfo);
}

void GeneralPage::startWizard()
{
    Wizard w(this, 2);
    if (w.exec() != QWizard::Accepted) {
        return;
    }
    deviceInfo = w.getDeviceInfo();
    qDebug() << "DeviceInfo: " << deviceInfo;
    projectDeviceLineEdit->setText(deviceInfo.at(3));
}



