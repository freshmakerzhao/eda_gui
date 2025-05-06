#include "ReportPowerDialog.h"

ReportPowerDialog::ReportPowerDialog(QWidget *parent) :
    QDialog(parent)
{
    resize(1000, 800);
    setWindowTitle("Report Power");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    setStyleSheet("QScrollArea{"
                  " border: none;"
                  "}");

    resultsNameLineEdit = new QLineEdit;
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Results name:", resultsNameLineEdit);
    vLayout->addLayout(formLayout);

    tabWidget = new QTabWidget;
    vLayout->addWidget(tabWidget);

    environmentTab = new EnvironmentTab;
    switchingTab = new SwitchingTab;
    tabWidget->addTab(environmentTab, "Environment");
    tabWidget->addTab(switchingTab, "Switching");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ReportPowerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ReportPowerDialog::reject);
    vLayout->addWidget(buttonBox);
}
