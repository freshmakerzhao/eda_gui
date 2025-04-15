#include "EnvironmentTab.h"

EnvironmentTab::EnvironmentTab(QWidget *parent)
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

    // ---------------------------- Device Settings --------------------------------
    QLabel *deviceSettingsLabel = new QLabel("Device Settings");
    deviceSettingsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(deviceSettingsLabel);

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout;
    vBoxLayout1->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(vBoxLayout1);

    QFormLayout *formLayout = new QFormLayout;
    vBoxLayout1->addLayout(formLayout);

    QComboBox *tempGradeComboBox = new QComboBox;
    tempGradeComboBox->setFixedWidth(150);
    formLayout->addRow("Temp grade:", tempGradeComboBox);
    tempGradeComboBox->addItems(QStringList() << "commercial" << "industrial");

    QComboBox *processComboBox = new QComboBox;
    processComboBox->setFixedWidth(150);
    formLayout->addRow("Temp grade:", processComboBox);
    processComboBox->addItems(QStringList() << "typical" << "maximum");

    mainLayout->addSpacing(20);
    // ------------------------- Environment Settings ------------------------------
    QLabel *environmentSettingsLabel = new QLabel("Environment Settings");
    environmentSettingsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(environmentSettingsLabel);

    QSpinBox *outputLoadSpinBox = new QSpinBox;
    outputLoadSpinBox->setValue(0);
    QCheckBox *junctionTemperatureCheckBox = new QCheckBox("Junction temperature:");
    QSpinBox *junctionTemperatureSpinBox = new QSpinBox;
    junctionTemperatureSpinBox->setValue(25);
    QSpinBox *ambienttemperatureSpinBox = new QSpinBox;
    ambienttemperatureSpinBox->setValue(25);
    QCheckBox *effectiveThetaJACheckBox = new QCheckBox("Effective θJA:");
    QSpinBox *effectiveThetaJASpinBox = new QSpinBox;
    effectiveThetaJASpinBox->setValue(25);
    QComboBox *airflowComboBox = new QComboBox;
    airflowComboBox->addItems(QStringList() << "250" << "200");
    QComboBox *heatSinkComboBox = new QComboBox;
    heatSinkComboBox->addItems(QStringList() << "none" << "low (Low Profile)" << "medium (Medium Profile)" << "high (High Profile)" << "custom");
    QDoubleSpinBox *thetaSASpinBox = new QDoubleSpinBox;
    thetaSASpinBox->setValue(4.6);
    QComboBox *boardSelection = new QComboBox;
    boardSelection->addItems(QStringList() << "jedec (2s2P)" << "small (4\"x4\")" << "medium (10\"x10\")" << "large (20\"x20\")" << "Custom");
    QComboBox *numberOfBoardLayers = new QComboBox;
    numberOfBoardLayers->addItems(QStringList() << "4to7 (4 to 7 Layers)" << "8to11 (8 to 11 Layers)" << "12to15 (12 to 15 Layers)" << "16+(16 or more Layers)");
    QDoubleSpinBox *thetaJBSpinBox = new QDoubleSpinBox;
    thetaJBSpinBox->setValue(6.8);
    QSpinBox *boardTemperatureSpinBox = new QSpinBox;
    boardTemperatureSpinBox->setValue(25);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(gridLayout);

    gridLayout->addWidget(new QLabel("Output Load:"), 0, 0);
    gridLayout->addWidget(outputLoadSpinBox, 0, 1);
    gridLayout->addWidget(new QLabel("pF [0-10000]"), 0, 2);

    gridLayout->addWidget(junctionTemperatureCheckBox, 1, 0);
    gridLayout->addWidget(junctionTemperatureSpinBox, 1, 1);
    gridLayout->addWidget(new QLabel("°C"), 1, 2);

    gridLayout->addWidget(new QLabel("Ambient temperature:"), 2, 0);
    gridLayout->addWidget(ambienttemperatureSpinBox, 2, 1);
    gridLayout->addWidget(new QLabel("°C"), 2, 2);

    gridLayout->addWidget(effectiveThetaJACheckBox, 3, 0);
    gridLayout->addWidget(effectiveThetaJASpinBox, 3, 1);
    gridLayout->addWidget(new QLabel("°C/W [0 - 100]"), 3, 2);

    gridLayout->addWidget(new QLabel("Airflow:"), 4, 0);
    gridLayout->addWidget(airflowComboBox, 4, 1);
    gridLayout->addWidget(new QLabel("LFM"), 4, 2);

    gridLayout->addWidget(new QLabel("Heat Sink:"), 5, 0);
    gridLayout->addWidget(heatSinkComboBox, 5, 1);

    gridLayout->addWidget(new QLabel("θSA:"), 6, 0);
    gridLayout->addWidget(thetaSASpinBox, 6, 1);
    gridLayout->addWidget(new QLabel("°C/W [0 - 100]"), 6, 2);

    gridLayout->addWidget(new QLabel("Board selection:"), 7, 0);
    gridLayout->addWidget(boardSelection, 7, 1);

    gridLayout->addWidget(new QLabel("Number of board layers:"), 8, 0);
    gridLayout->addWidget(numberOfBoardLayers, 8, 1);

    gridLayout->addWidget(new QLabel("θJB:"), 9, 0);
    gridLayout->addWidget(thetaJBSpinBox, 9, 1);
    gridLayout->addWidget(new QLabel("°C/W [0 - 100]"), 9, 2);

    gridLayout->addWidget(new QLabel("Board temperature:"), 10, 0);
    gridLayout->addWidget(boardTemperatureSpinBox, 10, 1);
    gridLayout->addWidget(new QLabel("°C [-55-85]"), 10, 2);

    mainLayout->addSpacing(20);
}
