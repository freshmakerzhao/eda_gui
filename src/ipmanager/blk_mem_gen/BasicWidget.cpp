#include "BasicWidget.h"

BasicWidget::BasicWidget(QWidget *parent) :
    BasePage(parent)
{
    QFormLayout *fLayout = new QFormLayout;
    mainLayout->addLayout(fLayout);
    interfaceTypeComboBox = new QComboBox(this);
    interfaceTypeComboBox->addItem("Native");
    interfaceTypeComboBox->addItem("AXI4");
    interfaceTypeComboBox->setCurrentIndex(1);
    connect(interfaceTypeComboBox, static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
            this, &::BasicWidget::onInterfaceTypeComboBoxIndexChanged);

    interfaceTypeComboBox->setFixedWidth(220);
    interfaceTypeCheckBox = new QCheckBox("Generate address interface with 32 bits", this);
    connect(interfaceTypeCheckBox, &QCheckBox::stateChanged, this, &BasicWidget::onInterfaceTypeCheckBoxStateChanged);

    QHBoxLayout *interfaceTypeLayout = new QHBoxLayout;
    interfaceTypeLayout->addWidget(interfaceTypeComboBox);
    interfaceTypeLayout->addWidget(interfaceTypeCheckBox);
    fLayout->addRow("Interface Type", interfaceTypeLayout);

    memoryTypeComboBox = new QComboBox(this);
    connect(memoryTypeComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this, &BasicWidget::memoryTypeComboBoxChangeSignal);
    memoryTypeComboBox->setFixedWidth(220);
    memoryTypeComboBox->addItem("Single Port RAM");
    memoryTypeComboBox->addItem("Simple Dual Port RAM");
    memoryTypeComboBox->addItem("True Dual Port RAM");
    memoryTypeComboBox->addItem("Single Port ROM");
    memoryTypeComboBox->addItem("Dual Port ROM");
    memoryTypeComboBox->setCurrentIndex(3);

    memoryTypeCheckBox = new QCheckBox("Common Clock", this);
    connect(memoryTypeCheckBox, &QCheckBox::stateChanged, this, &BasicWidget::memoryTypeCheckBoxStateChanged);
    QHBoxLayout *memoryTypeLayout = new QHBoxLayout;
    memoryTypeLayout->addWidget(memoryTypeComboBox);
    memoryTypeLayout->addWidget(memoryTypeCheckBox);
    fLayout->addRow("Memory Type", memoryTypeLayout);
    mainLayout->addSpacing(20);
    // ------------------------- ECC Options -------------------------
    QLabel *eccOptLabel = new QLabel("ECC Options", this);
    eccOptLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(eccOptLabel);
    QFormLayout *eccOptionfLayout = new QFormLayout;
    eccOptionfLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(eccOptionfLayout);
    eccTypeComboBox = new QComboBox(this); eccTypeComboBox->setEnabled(false);
    eccTypeComboBox->setFixedWidth(220);
    eccTypeComboBox->addItem("No ECC");
    eccOptionfLayout->addRow("ECC Type", eccTypeComboBox);
    errorInjectionPinsCheckBox = new QCheckBox("Error Injection Pins", this); errorInjectionPinsCheckBox->setEnabled(false);
    QHBoxLayout *eccOptionvLayout = new QHBoxLayout;
    eccOptionvLayout->setAlignment(Qt::AlignLeft);
    eccOptionfLayout->addRow(eccOptionvLayout);
    eccOptionvLayout->addWidget(errorInjectionPinsCheckBox);
    errorInjectionPinsComboBox = new QComboBox(this); errorInjectionPinsComboBox->setEnabled(false);
    eccOptionvLayout->addWidget(errorInjectionPinsComboBox);
    errorInjectionPinsComboBox->addItem("Single Bit Error Injection");
    mainLayout->addSpacing(20);
    // ------------------------ Write Enable -------------------------
    QLabel *writeEnableLabel = new QLabel("Write Enable", this);
    writeEnableLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(writeEnableLabel);
    mainLayout->addSpacing(20);
    // ----------------------- Algorithm Options ---------------------
    QLabel *algorithmOptionsLabel = new QLabel("Algorithm Options", this);
    algorithmOptionsLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(algorithmOptionsLabel);

}

void BasicWidget::onInterfaceTypeComboBoxIndexChanged(const int &index)
{
    qDebug() << "Interface Type : " << interfaceTypeComboBox->itemText(index);
}

void BasicWidget::onInterfaceTypeCheckBoxStateChanged(const int &state)
{
    if (state == Qt::Checked) {
        // TODO
        qDebug() << "Interface Type : [Generate address interface with 32 bits] Enabled";
    }

    if (state == Qt::Unchecked) {
        // TODO
        qDebug() << "Interface Type : [Generate address interface with 32 bits] Disabled";
    }
}

void BasicWidget::memoryTypeCheckBoxStateChanged(const int &state)
{
    if (state == Qt::Checked) {
        // TODO
        qDebug() << "Memory Type : [Common Clock] Enabled";
    }

    if (state == Qt::Unchecked) {
        // TODO
        qDebug() << "Memory Type : [Common Clock] Disabled";
    }
}
