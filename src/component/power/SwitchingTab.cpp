#include "SwitchingTab.h"

SwitchingTab::SwitchingTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    vBoxLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);
    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addSpacing(10);

    // -----------------------------------------------------------------------------
    QCheckBox *resetSwitchingActivityBeforeReportPowerCheckBox = new QCheckBox("Reset switching activity before report power");
    QComboBox *switchingActivityForResetsComboBox = new QComboBox;
    switchingActivityForResetsComboBox->addItems(QStringList() << "None" << "Deassert" << "Do Not Deassert");

    mainLayout->addWidget(resetSwitchingActivityBeforeReportPowerCheckBox);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Switching Activity for Resets:", switchingActivityForResetsComboBox);
    mainLayout->addLayout(formLayout);

    // ---------------------------- Simulation Settings --------------------------------
    QLabel *simulationSettingsLabel = new QLabel("Simulation Settings");
    simulationSettingsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(simulationSettingsLabel);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(hBoxLayout);
    hBoxLayout->addWidget(new QLabel("Simulation activity file (.saif):"));
    QLineEdit *simulationActivityFileLineEdit = new QLineEdit;
    hBoxLayout->addWidget(simulationActivityFileLineEdit);
    QPushButton *simulationActivityFileButton = new QPushButton("...");
    hBoxLayout->addWidget(simulationActivityFileButton);

    // ------------------------ Default Activity Settings -----------------------------
    QLabel *defaultActivitySettingsLabel = new QLabel("Default Activity Settings");
    defaultActivitySettingsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(defaultActivitySettingsLabel);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout);

    QLineEdit *lineEdit1 = new QLineEdit;
    QLineEdit *lineEdit2 = new QLineEdit;
    QLineEdit *lineEdit3 = new QLineEdit;
    QLineEdit *lineEdit4 = new QLineEdit;
    QLineEdit *lineEdit5 = new QLineEdit;
    QLineEdit *lineEdit6 = new QLineEdit;

    gridLayout->addWidget(new QLabel("Static Probability"), 0, 1, 1, 2);
    gridLayout->addWidget(new QLabel("Toggle Rate"), 0, 3, 1, 2);

    gridLayout->addWidget(new QLabel("BRAM Port Enable:"), 1, 0);
    gridLayout->addWidget(lineEdit1, 1, 1, 1, 1);
    gridLayout->addWidget(new QLabel("[0.0 - 1.0]"), 1, 2, 1, 1);
    gridLayout->addWidget(lineEdit2, 1, 3, 1, 1);
    gridLayout->addWidget(new QLabel("[0 - 100]"), 1, 4, 1, 1);

    gridLayout->addWidget(new QLabel("BRAM Write Enable:"), 2, 0);
    gridLayout->addWidget(lineEdit3, 2, 1, 1, 1);
    gridLayout->addWidget(new QLabel("[0.0 - 1.0]"), 2, 2, 1, 1);
    gridLayout->addWidget(lineEdit4, 2, 3, 1, 1);
    gridLayout->addWidget(new QLabel("[0 - 100]"), 2, 4, 1, 1);

    gridLayout->addWidget(new QLabel("Bidi Output Port Enable:"), 3, 0);
    gridLayout->addWidget(lineEdit5, 3, 1, 1, 1);
    gridLayout->addWidget(new QLabel("[0.0 - 1.0]"), 3, 2, 1, 1);
    gridLayout->addWidget(lineEdit6, 3, 3, 1, 1);
    gridLayout->addWidget(new QLabel("[0 - 100]"), 3, 4, 1, 1);

    // ------------------------ Toggle Rate Settings --------------------
    QLabel *toggleRateSettingsLabel = new QLabel("Default Activity Settings");
    toggleRateSettingsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(toggleRateSettingsLabel);

    QLineEdit *lineEdit7 = new QLineEdit;
    QLineEdit *lineEdit8 = new QLineEdit;
    QLineEdit *lineEdit9 = new QLineEdit;
    QLineEdit *lineEdit10 = new QLineEdit;
    QLineEdit *lineEdit11 = new QLineEdit;
    QLineEdit *lineEdit12 = new QLineEdit;
    QLineEdit *lineEdit13 = new QLineEdit;
    QLineEdit *lineEdit14 = new QLineEdit;
    QLineEdit *lineEdit15 = new QLineEdit;
    QLineEdit *lineEdit16 = new QLineEdit;
    QLineEdit *lineEdit17 = new QLineEdit;
    QLineEdit *lineEdit18 = new QLineEdit;
    QLineEdit *lineEdit19 = new QLineEdit;
    QLineEdit *lineEdit20 = new QLineEdit;
    QLineEdit *lineEdit21 = new QLineEdit;
    QLineEdit *lineEdit22 = new QLineEdit;
    QLineEdit *lineEdit23 = new QLineEdit;
    QLineEdit *lineEdit24 = new QLineEdit;

    QGridLayout *gridLayout1 = new QGridLayout;
    gridLayout1->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout1);

    gridLayout1->addWidget(new QLabel("Static Probability"), 0, 1, 1, 2);
    gridLayout1->addWidget(new QLabel("Toggle Rate"), 0, 3, 1, 2);

    // gridLayout1->addItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), 2, 0);
    gridLayout1->addWidget(new QLabel("Logic"), 2, 0);
    gridLayout1->addWidget(new QLabel("  Registers:"), 3, 0);
    gridLayout1->addWidget(new QLabel("  Shift Registers:"), 4, 0);
    gridLayout1->addWidget(new QLabel("  Distributed RAMs:"), 5, 0);
    gridLayout1->addWidget(new QLabel("  LUTs:"), 6, 0);
    gridLayout1->addWidget(new QLabel("  DSPs:"), 7, 0);
    gridLayout1->addWidget(new QLabel("  BIock RAMs:"), 8, 0);
    gridLayout1->addWidget(new QLabel("GTs"), 9, 0);
    gridLayout1->addWidget(new QLabel("  RX Data:"), 10, 0);
    gridLayout1->addWidget(new QLabel("  TX Data:"), 11, 0);

    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 1, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 3, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 4, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 5, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 6, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 7, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 8, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 10, 2, 1, 1);
    gridLayout1->addWidget(new QLabel("[0.0 - 1.0]"), 11, 2, 1, 1);

    gridLayout1->addWidget(new QLabel("[0 - 100]"), 1, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 3, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 4, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 5, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 6, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 7, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 8, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 10, 4, 1, 1);
    gridLayout1->addWidget(new QLabel("[0 - 100]"), 11, 4, 1, 1);

    gridLayout1->addWidget(new QLabel("Primary Outputs:"), 1, 0);
    gridLayout1->addWidget(lineEdit7, 1, 1, 1, 1);
    gridLayout1->addWidget(lineEdit8, 1, 3, 1, 1);
    gridLayout1->addWidget(lineEdit9, 3, 1, 1, 1);
    gridLayout1->addWidget(lineEdit10, 3, 3, 1, 1);
    gridLayout1->addWidget(lineEdit11, 4, 1, 1, 1);
    gridLayout1->addWidget(lineEdit12, 4, 3, 1, 1);
    gridLayout1->addWidget(lineEdit13, 5, 1, 1, 1);
    gridLayout1->addWidget(lineEdit14, 5, 3, 1, 1);
    gridLayout1->addWidget(lineEdit15, 6, 1, 1, 1);
    gridLayout1->addWidget(lineEdit16, 6, 3, 1, 1);
    gridLayout1->addWidget(lineEdit17, 7, 1, 1, 1);
    gridLayout1->addWidget(lineEdit18, 7, 3, 1, 1);
    gridLayout1->addWidget(lineEdit19, 8, 1, 1, 1);
    gridLayout1->addWidget(lineEdit20, 8, 3, 1, 1);
    gridLayout1->addWidget(lineEdit21, 10, 1, 1, 1);
    gridLayout1->addWidget(lineEdit22, 10, 3, 1, 1);
    gridLayout1->addWidget(lineEdit23, 11, 1, 1, 1);
    gridLayout1->addWidget(lineEdit24, 11, 3, 1, 1);


}
