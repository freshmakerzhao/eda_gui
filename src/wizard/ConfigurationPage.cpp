#include "ConfigurationPage.h"

ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Configuration");

    compatibilityModeCheckBox = new QCheckBox("Enable compatibility mode");
    compatibilityModeCheckBox->setChecked(true); // 默认启用

    compatibilityModeLabel = new QLabel("This option cannot be changed after creating the project.", this);
    compatibilityModeLabel->setStyleSheet("color: gray;");

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(compatibilityModeCheckBox);
    vBoxLayout->addWidget(compatibilityModeLabel);
    connect(compatibilityModeCheckBox, &QCheckBox::clicked,
            this, &ConfigurationPage::onCompatibilityModeCheckBoxClicked);
}

void ConfigurationPage::onCompatibilityModeCheckBoxClicked(bool checked)
{
    Wizard *wizard = qobject_cast<Wizard*>(this->wizard());
    wizard->compatibilityMode = checked ? "enable" : "disable";
}

