#include "NativePorts.h"

NativePorts::NativePorts(QWidget *parent) :
    BasePage(parent)
{
    // -------------------------- Read Mode --------------------------------
    QLabel *readModeLabel = new QLabel("Read Mode");
    readModeLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(readModeLabel);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setAlignment(Qt::AlignLeft);
    hBoxLayout->setContentsMargins(25, 0, 0, 0);
    QRadioButton *standardFIFORadioButton = new QRadioButton("Standard FIFO");
    QRadioButton *firstWordFallThroughRadioButton = new QRadioButton("First Word Fall Through");
    hBoxLayout->addWidget(standardFIFORadioButton);
    hBoxLayout->addWidget(firstWordFallThroughRadioButton);
    mainLayout->addLayout(hBoxLayout);
    mainLayout->addSpacing(20);
    // --------------------- Data Port Parameters --------------------------
    QLabel *dataPortParametersLabel = new QLabel("Data Port Parameters");
    dataPortParametersLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(dataPortParametersLabel);

    QGridLayout *gridLayout1 = new QGridLayout;
    gridLayout1->setAlignment(Qt::AlignLeft);
    gridLayout1->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout1);
    QLineEdit *writeWidthLineEdit = new QLineEdit;
    QComboBox *writeDepthComboBox = new QComboBox;
    QComboBox *readWidthComboBox = new QComboBox;
    QLineEdit *readDepthLineEdit = new QLineEdit;

    writeWidthLineEdit->setClearButtonEnabled(true);

    int writeDepth = 16;
    QStringList writeDepthList;
    for (int i = 0; i < 14; i ++) {
        writeDepthList << QString::number(writeDepth);
        writeDepth = writeDepth *2;
    }
    writeDepthComboBox->addItems(writeDepthList);

    int readWidth = 9;
    QStringList readWidthList;
    for (int i = 0; i < 5; i ++) {
        readWidthList << QString::number(readWidth);
        readWidth = readWidth *2;
    }
    readWidthComboBox->addItems(readWidthList);

    readDepthLineEdit->setEnabled(false);

    writeWidthLineEdit->setFixedWidth(220);
    writeDepthComboBox->setFixedWidth(220);
    readWidthComboBox->setFixedWidth(220);
    readDepthLineEdit->setFixedWidth(220);

    QLabel *writeWidthLabel = new QLabel("Write Width");
    QLabel *writeDepthLabel = new QLabel("Write Depth");
    QLabel *readWidthLabel = new QLabel("Read Width");
    QLabel *readDepthLabel = new QLabel("Read Depth");

    writeWidthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    writeDepthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    readWidthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    readDepthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    gridLayout1->addWidget(writeWidthLabel, 0, 0);
    gridLayout1->addWidget(writeDepthLabel, 1, 0);
    gridLayout1->addWidget(readWidthLabel, 2, 0);
    gridLayout1->addWidget(readDepthLabel, 3, 0);

    gridLayout1->addWidget(writeWidthLineEdit, 0, 1);
    gridLayout1->addWidget(writeDepthComboBox, 1, 1);
    gridLayout1->addWidget(readWidthComboBox, 2, 1);
    gridLayout1->addWidget(readDepthLineEdit, 3, 1);

    gridLayout1->addWidget(new QLabel("1,2,3,...1024"), 0, 4);
    gridLayout1->addWidget(new QLabel("Actual Write Depth: N/A"), 1, 4);
    gridLayout1->addWidget(new QLabel("Actual Read Depth: N/A"), 3, 4);
    mainLayout->addSpacing(20);
    // --------- ECC, Output Register and Power Gating Options -------------
    QLabel *optionsLabel = new QLabel("ECC, Output Register and Power Gating Options");
    optionsLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(optionsLabel);

    QGridLayout *gridLayout2 = new QGridLayout;
    gridLayout2->setAlignment(Qt::AlignLeft);
    gridLayout2->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout2);

    QCheckBox *outputRegistersCheckBox = new QCheckBox("Output Registers");
    QComboBox *outputRegistersComboBox = new QComboBox;
    outputRegistersComboBox->addItems({"Embedded Registers", "Fabric Registers", "Embedded Reg AND Fabric Reg"});

    outputRegistersComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    gridLayout2->addWidget(outputRegistersCheckBox, 0, 0);
    gridLayout2->addWidget(outputRegistersComboBox, 0, 1);

    mainLayout->addSpacing(20);
    // --------------------- Data Port Parameters --------------------------
    QLabel *initializationLabel = new QLabel("Initialization");
    initializationLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(initializationLabel);

    QGridLayout *gridLayout3 = new QGridLayout;
    gridLayout3->setAlignment(Qt::AlignLeft);
    gridLayout3->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout3);

    QCheckBox *resetPinCheckBox = new QCheckBox("Reset Pin");
    QLabel *resetTypeLabel = new QLabel("Reset Type");
    QComboBox *resetTypeComboBox = new QComboBox;
    resetTypeComboBox->addItems({"Synchronous Reset", "Asynchronous Reset"});
    QLabel *fullFlagsResetValueLabel = new QLabel("Full Flags Reset Value");
    QComboBox *fullFlagsResetValueComboBox = new QComboBox;
    fullFlagsResetValueComboBox->addItems({"0", "1"});
    QCheckBox *doutResetCheckBox = new QCheckBox("Dout Reset Value");
    QComboBox *doutResetComboBox = new QComboBox;
    QLabel *readLatencyLabel = new QLabel("Read Latency :");

    resetTypeComboBox->setFixedWidth(220);
    fullFlagsResetValueComboBox->setFixedWidth(220);
    doutResetComboBox->setFixedWidth(220);

    resetTypeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    fullFlagsResetValueLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    gridLayout3->addWidget(resetPinCheckBox, 0, 0, 1, 2);
    gridLayout3->addWidget(resetTypeLabel, 1, 0);
    gridLayout3->addWidget(resetTypeComboBox, 1, 1);
    gridLayout3->addWidget(fullFlagsResetValueLabel, 2, 0);
    gridLayout3->addWidget(fullFlagsResetValueComboBox, 2, 1);
    gridLayout3->addWidget(doutResetCheckBox, 3, 0);
    gridLayout3->addWidget(doutResetComboBox, 3, 1);
    gridLayout3->addWidget(new QLabel("(Hex)"), 3, 2);

    mainLayout->addWidget(readLatencyLabel);
}
