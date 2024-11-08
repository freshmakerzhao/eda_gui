#include "NativePorts.h"

NativePorts::NativePorts(QWidget *parent) :
    BasePage(parent)
{
    QLabel *readModeLabel = new QLabel("Read Mode");
    readModeLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(readModeLabel);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setAlignment(Qt::AlignLeft);
    QRadioButton *standardFIFORadioButton = new QRadioButton("Standard FIFO");
    QRadioButton *firstWordFallThroughRadioButton = new QRadioButton("First Word Fall Through");
    hBoxLayout->addWidget(standardFIFORadioButton);
    hBoxLayout->addWidget(firstWordFallThroughRadioButton);
    mainLayout->addLayout(hBoxLayout);

    QLabel *dataPortParametersLabel = new QLabel("Data Port Parameters");
    dataPortParametersLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(dataPortParametersLabel);

    QGridLayout *gridLayout = new QGridLayout;
    mainLayout->addLayout(gridLayout);
    QLineEdit *writeWidthLineEdit = new QLineEdit;
    QComboBox *writeDepthLineEdit = new QComboBox;
    QComboBox *readWidthLineEdit = new QComboBox;
    QLineEdit *readDepthLineEdit = new QLineEdit;

    gridLayout->addWidget(new QLabel("Write Width"), 0, 0, 1, 1);
    gridLayout->addWidget(new QLabel("Write Depth"), 1, 0, 1, 1);
    gridLayout->addWidget(new QLabel("Read Width"), 2, 0, 1, 1);
    gridLayout->addWidget(new QLabel("Read Depth"), 3, 0, 1, 1);

    gridLayout->addWidget(writeWidthLineEdit, 0, 1);
    gridLayout->addWidget(writeDepthLineEdit, 1, 1);
    gridLayout->addWidget(readWidthLineEdit, 2, 1);
    gridLayout->addWidget(readDepthLineEdit, 3, 1);

    gridLayout->addWidget(new QLabel("1,2,3,...1024"), 0, 4);
    gridLayout->addWidget(new QLabel("Actual Write Depth: N/A"), 1, 4);
    gridLayout->addWidget(new QLabel("Actual Read Depth: N/A"), 3, 4);
}
