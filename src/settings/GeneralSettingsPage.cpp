#include "GeneralSettingsPage.h"
#include "utils/ProjectManager.h"
#include "wizard/Wizard.h"

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("GeneralSettingsPage");

    QLabel *label = new QLabel("<b>General</b><br> Specify values for various settings used throughout the design flow. "
                               "These settings apply to the current project.", this);
    label->setStyleSheet("border-bottom: 3px dashed #DCDCDC;"
                        "border-radius: 0px;");
    label->setWordWrap(true);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(label);

    projectDeviceLineEdit = new QLineEdit;
    projectDeviceLineEdit->setReadOnly(true);
    QAction *action = new QAction;
    action->setIcon(QIcon(":/icons/resource/icons/28-icon_grid_view.png"));
    projectDeviceLineEdit->addAction(action, QLineEdit::LeadingPosition);

    projectDeviceSquareButton = new QPushButton;
    connect(projectDeviceSquareButton, &QPushButton::clicked, this, &GeneralSettingsPage::startWizard);
    // projectDeviceSquareButton->setFixedSize(projectDeviceSquareButton->height(), projectDeviceSquareButton->height()); // 设置为方形
    // projectDeviceSquareButton->setObjectName("squareButton");

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(projectDeviceLineEdit);
    // hLayout->addSpacing(5); // 添加一点间距
    hLayout->addWidget(projectDeviceSquareButton);

    topModuleNameLineEdit = new QLineEdit;
    topModuleNameLineEdit->setClearButtonEnabled(true);

    fLayout = new QFormLayout(this);
    fLayout->addRow(hBoxLayout);
    fLayout->addRow("Project device:", hLayout);
    fLayout->addRow("Top module name:", topModuleNameLineEdit);

    QString topName = ProjectManager::instance().getParameter(Project::TopModule);
    tempDeviceName = ProjectManager::instance().getParameter(Project::DisplayPart);
    QString deviceInfo = QString("%1 (active)").arg(tempDeviceName);
    projectDeviceLineEdit->setText(deviceInfo);
    projectDeviceSquareButton->setText("...");
    topModuleNameLineEdit->setText(topName);
}

void GeneralSettingsPage::setTopModule()
{
    ProjectManager::instance().setTopModule(topModuleNameLineEdit->text());
}

void GeneralSettingsPage::setDevicePart()
{
    if (deviceInfo.isEmpty()) return;
    if (deviceInfo.first().isEmpty()) return;
    ProjectManager::instance().setDevicePart(deviceInfo.at(3));
}

void GeneralSettingsPage::startWizard()
{
    Wizard wizard(this, WizardMode::SET_DEVICE);
    if (wizard.exec() != QWizard::Accepted) {
        return;
    }
    deviceInfo = wizard.getDeviceInfo();
    qDebug() << "DeviceInfo: " << deviceInfo;
    if (deviceInfo.at(3) == tempDeviceName) {
        projectDeviceLineEdit->setText(QString("%1 (active)").arg(deviceInfo.at(3)));
    } else {
        projectDeviceLineEdit->setText(deviceInfo.at(3));
    }
}



