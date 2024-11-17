#include "DataCounts.h"

DataCounts::DataCounts(QWidget *parent) :
    BasePage(parent)
{
    // ---------------------- Data Count Options ---------------------------
    QLabel *dataCountOptionsLabel = new QLabel("Read Mode");
    dataCountOptionsLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(dataCountOptionsLabel);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignLeft);
    gridLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout);

    QCheckBox *moreAccurateDataCountsCheckBox = new QCheckBox("More Accurate Data Counts");
    QCheckBox *dataCountCheckBox = new QCheckBox("Data Count");
    QLineEdit *dataCountWidthLineEdit = new QLineEdit;
    QCheckBox *writeDataCountCheckBox = new QCheckBox("Write Data Count(Synchronized with Write Clk)");
    QLineEdit *writeDataCountWidthLineEdit = new QLineEdit;
    QCheckBox *readDataCountCheckBox = new QCheckBox("Read Data Count(Synchronized with Write Clk)");
    QLineEdit *readDataCountWidthLineEdit = new QLineEdit;

    dataCountWidthLineEdit->setFixedWidth(220);
    writeDataCountWidthLineEdit->setFixedWidth(220);
    readDataCountWidthLineEdit->setFixedWidth(220);

    QLabel *dataCountWidthLabel = new QLabel("Data Count Width");
    QLabel *writeDataCountWidthLabel = new QLabel("Write Data Count Width");
    QLabel *readDataCountWidthLabel = new QLabel("Read Data Count Width");

    moreAccurateDataCountsCheckBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    dataCountCheckBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    writeDataCountCheckBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    readDataCountCheckBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    dataCountWidthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    writeDataCountWidthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    readDataCountWidthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    gridLayout->addWidget(moreAccurateDataCountsCheckBox, 0, 0);
    gridLayout->addWidget(dataCountCheckBox, 1, 0, 1, 2);
    gridLayout->addWidget(dataCountWidthLabel, 2, 0, 1, 2);
    gridLayout->addWidget(dataCountWidthLineEdit, 2, 1);

    gridLayout->addWidget(writeDataCountCheckBox, 3, 0, 1, 2);
    gridLayout->addWidget(writeDataCountWidthLabel, 4, 0, 1, 2);
    gridLayout->addWidget(writeDataCountWidthLineEdit, 4, 1);

    gridLayout->addWidget(readDataCountCheckBox, 5, 0, 1, 2);
    gridLayout->addWidget(readDataCountWidthLabel, 6, 0, 1, 2);
    gridLayout->addWidget(readDataCountWidthLineEdit, 6, 1);
}
