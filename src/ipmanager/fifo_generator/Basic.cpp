#include "Basic.h"

Basic::Basic(QWidget *parent) :
    BasePage(parent)
{
    QLabel *interfaceTypeLabel = new QLabel("Interface Type");
    interfaceTypeLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);

    mainLayout->addWidget(interfaceTypeLabel);

    QHBoxLayout *interfaceTypeLayout = new QHBoxLayout;
    interfaceTypeLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addLayout(interfaceTypeLayout);
    interfaceTypeLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    QRadioButton *nativeRadioButton = new QRadioButton("Native");
    QRadioButton *axiMemMapRadioButton = new QRadioButton("AXI Memory Mapped");
    QRadioButton *axiStreamRadioButton = new QRadioButton("AXI Stream");
    QButtonGroup *buttonGroup = new QButtonGroup;
    buttonGroup->addButton(nativeRadioButton);
    buttonGroup->addButton(axiMemMapRadioButton);
    buttonGroup->addButton(axiStreamRadioButton);
    interfaceTypeLayout->addWidget(nativeRadioButton);
    interfaceTypeLayout->addWidget(axiMemMapRadioButton);
    interfaceTypeLayout->addWidget(axiStreamRadioButton);


    QFormLayout *fLayout = new QFormLayout;
    mainLayout->addLayout(fLayout);
    QComboBox *fifoImplComboBox = new QComboBox;
    QStringList fifoImplItems;
    fifoImplItems << "Common Clock Block RAM"
                  << "Common Clock Distributed RAM"
                  << "Common Clock Shift Register"
                  << "Common Clock Builtin FIFO"
                  << "Independent Clocks Block RAM"
                  << "Independent Clocks Distributed RAM"
                  << "Independent Clocks Builtin FIFO";
    fifoImplComboBox->addItems(fifoImplItems);
    fLayout->addRow("Fifo Implementation", fifoImplComboBox);
}
