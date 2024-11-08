#include "ClockingOptionsWidget.h"

ClockingOptionsWidget::ClockingOptionsWidget(AdvancedTableView *inputClockInformationTableView, QStandardItemModel *inputClockInformationModel, QWidget *parent) :
    BasePage(parent)
{
    // ----------------------- Clock Monitor ---------------------------
    QLabel *clockMonitorLabel = new QLabel("Clock Monitor", this);
    clockMonitorLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(clockMonitorLabel);
    enableClockMonitoringCheckBox = new QCheckBox("Enable Clock Monitoring", this);
    QHBoxLayout *hBoxLayout1 = new QHBoxLayout;
    hBoxLayout1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hBoxLayout1->addWidget(enableClockMonitoringCheckBox);
    mainLayout->addLayout(hBoxLayout1);
    mainLayout->addSpacing(20);
    // ------------------------- Primitive -----------------------------
    QLabel *primitiveLabel = new QLabel("Primitive", this);
    primitiveLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(primitiveLabel);
    mmcmRadioButton = new QRadioButton("MMCM", this), mmcmRadioButton->setObjectName("MMCM_Radio_Button");
    pllRadioButton = new QRadioButton("PLL", this), pllRadioButton->setObjectName("PLL_Radio_Button");
    buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(mmcmRadioButton);
    buttonGroup1->addButton(pllRadioButton);
    mmcmRadioButton->setChecked(true);
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout;
    hBoxLayout2->setMargin(0);
    hBoxLayout2->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hBoxLayout2->setAlignment(Qt::AlignLeft);
    hBoxLayout2->addWidget(mmcmRadioButton);
    hBoxLayout2->addWidget(pllRadioButton);
    mainLayout->addLayout(hBoxLayout2);
    mainLayout->addSpacing(20);
    // ---------------------- Init GridLayout --------------------------
    QHBoxLayout *hBoxLayout3 = new QHBoxLayout;
    mainLayout->addLayout(hBoxLayout3);
    mainLayout->addSpacing(20);
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignLeft);
    hBoxLayout3->addLayout(gridLayout);
    gridLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0);
    gridLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 3);
    // --------------------- Clocking Features -------------------------
    QLabel *clockingFeaturesLabel = new QLabel("Clocking Features", this);
    clockingFeaturesLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    frequencySynthesisCheckBox = new QCheckBox("Frequency Synthesis", this);
    minimizePowerCheckBox = new QCheckBox("Minimize Power", this);
    phaseAlignmentCheckBox = new QCheckBox("Phase Alignment", this), phaseAlignmentCheckBox->setObjectName("Phase_Alignment");
    spreadSpectrumCheckBox = new QCheckBox("Spread Spectrum", this);
    dynamicReconfigCheckBox = new QCheckBox("Dynamic Reconfig", this), dynamicReconfigCheckBox->setObjectName("Dynamic_Reconfig");
    dynamicPhaseShiftCheckBox = new QCheckBox("Dynamic Phase Shift", this), dynamicPhaseShiftCheckBox->setObjectName("Dynamic_Phase_Shift");
    safeClockStartupCheckBox = new QCheckBox("Safe Clock Startup", this);
    gridLayout->addWidget(clockingFeaturesLabel, 0, 0, 1, 3);
    gridLayout->addWidget(frequencySynthesisCheckBox, 1, 1);
    gridLayout->addWidget(minimizePowerCheckBox, 1, 2);
    gridLayout->addWidget(phaseAlignmentCheckBox, 2, 1);
    gridLayout->addWidget(spreadSpectrumCheckBox, 2, 2);
    gridLayout->addWidget(dynamicReconfigCheckBox, 3, 1);
    gridLayout->addWidget(dynamicPhaseShiftCheckBox, 3, 2);
    gridLayout->addWidget(safeClockStartupCheckBox, 4, 1);
    // --------------------- Jitter Optimization -----------------------
    QLabel *jitterOptimizationLabel = new QLabel("Jitter Optimization", this);
    jitterOptimizationLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    balancedRadioButton = new QRadioButton("Balanced", this);
    minimizeOutputJitterRadioButton = new QRadioButton("Minimize Output Jitter", this);
    maximizeInputJitterfilteringRadioButton = new QRadioButton("Maximize Input Jitter filtering", this);
    gridLayout->addWidget(jitterOptimizationLabel, 0, 3, 1, 3);
    gridLayout->addWidget(balancedRadioButton, 1, 4);
    gridLayout->addWidget(minimizeOutputJitterRadioButton, 2, 4);
    gridLayout->addWidget(maximizeInputJitterfilteringRadioButton, 3, 4);
    buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(balancedRadioButton);
    buttonGroup2->addButton(minimizeOutputJitterRadioButton);
    buttonGroup2->addButton(maximizeInputJitterfilteringRadioButton);
    balancedRadioButton->setChecked(true);
    // ------------- Dynamic Reconfig Interface Options ----------------
    QLabel *dynamicReconfigInterfaceOptionsLabel = new QLabel("Dynamic Reconfig Interface Options", this);
    dynamicReconfigInterfaceOptionsLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(dynamicReconfigInterfaceOptionsLabel);
    QHBoxLayout *hBoxLayout4 = new QHBoxLayout;
    hBoxLayout4->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hBoxLayout4->setAlignment(Qt::AlignLeft);
    mainLayout->addLayout(hBoxLayout4);
    axi4LiteRadioButton = new QRadioButton("AXI4Lite", this);
    drpRadioButton = new QRadioButton("DRP", this);
    phaseDutyCycleConfigCheckBox = new QCheckBox("Phase Duty Cycle Config", this);
    writeDRPregistersCheckBox = new QCheckBox("Write DRP registers", this);
    hBoxLayout4->addWidget(axi4LiteRadioButton);
    hBoxLayout4->addWidget(drpRadioButton);
    hBoxLayout4->addWidget(phaseDutyCycleConfigCheckBox);
    hBoxLayout4->addWidget(writeDRPregistersCheckBox);
    buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(axi4LiteRadioButton);
    buttonGroup3->addButton(drpRadioButton);
    mainLayout->addSpacing(20);
    // ------------------- Input Clock Information --------------------
    QLabel *inputClockInformationLabel = new QLabel("Input Clock Information", this);
    inputClockInformationLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(inputClockInformationLabel);
    // model = new QStandardItemModel;
    model = inputClockInformationModel;
    // model = qobject_cast<QStandardItemModel *>(tableView->model());
    QStringList headers;
    headers << "" << "Input Clock" << "Port Name" << "Input Frequency (MHz)" << ""
            << "Jitter Options" << "Input Jitter" << "Source";
    model->setHorizontalHeaderLabels(headers);

    // tableView = new QTableView(this);
    // tableView = new AdvancedTableView(this);
    tableView = inputClockInformationTableView;
    QHBoxLayout *inputClockInformationLayout = new QHBoxLayout;
    inputClockInformationLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    inputClockInformationLayout->addWidget(tableView);
    mainLayout->addLayout(inputClockInformationLayout);
    tableView->setModel(model);
    // model = qobject_cast<QStandardItemModel *>(tableView->model());

    model->insertRows(0, 2);
    // ----------------------- Primary --------------------------
    model->setData(model->index(0, 0), "");
    model->item(0, 0)->setFlags(model->item(0, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    model->setData(model->index(0, 1), "Primary");
    model->item(0, 1)->setFlags(model->item(0, 1)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    // 创建 QLineEdit 并嵌入到 "Port Name" 列
    QLineEdit *priPortNameEdit = new QLineEdit(tableView);
    priPortNameEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(0, 2), priPortNameEdit);
    priPortNameEdit->setText("clk_in1");
    // 创建 QLineEdit 并嵌入到 "Input Frequency (MHz)" 列
    QLineEdit *priFrequencyEdit = new QLineEdit(tableView);
    priFrequencyEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(0, 3), priFrequencyEdit);
    priFrequencyEdit->setText("100.000");
    // 创建 QComboBox 并嵌入到 "Jitter Options" 列
    QComboBox *priJitterOptionsComboBox = new QComboBox(tableView);
    priJitterOptionsComboBox->addItems({"UI", "PS"});
    model->setData(model->index(0, 4), "10.000-800.000");
    model->item(0, 4)->setFlags(model->item(0, 4)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    tableView->setIndexWidget(model->index(0, 5), priJitterOptionsComboBox);
    // 创建 QLineEdit 并嵌入到 "Input Jitter" 列
    QLineEdit *priJitterEdit = new QLineEdit(tableView);
    priJitterEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(0, 6), priJitterEdit);
    priJitterEdit->setText("0.010");
    // 创建 QComboBox 并嵌入到 "Source" 列
    QComboBox *priSourceBox = new QComboBox(tableView);
    priSourceBox->addItems({"Single ended clock capable pin", "Differential clock capable pin", "Global buffer", "No buffer"});
    tableView->setIndexWidget(model->index(0, 7), priSourceBox);
    // priSourceBox->setCurrentText("Source 1");
    // ----------------------- Seecondary ------------------------
    QWidget *widget = new QWidget(tableView);
    QCheckBox *secEnableCheckBox = new QCheckBox(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(secEnableCheckBox);
    hLayout->setMargin(0);
    hLayout->setAlignment(secEnableCheckBox, Qt::AlignCenter);
    widget->setLayout(hLayout);
    tableView->setIndexWidget(model->index(1, 0), widget);
    model->setData(model->index(1, 1), "Secondary");
    model->item(1, 1)->setFlags(model->item(1, 1)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *secPortNameEdit = new QLineEdit(tableView);
    secPortNameEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(1, 2), secPortNameEdit);
    secPortNameEdit->setText("clk_in2");
    // 创建 QLineEdit 并嵌入到 "Input Frequency (MHz)" 列
    QLineEdit *secFrequencyEdit = new QLineEdit(tableView);
    secFrequencyEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(1, 3), secFrequencyEdit);
    secFrequencyEdit->setText("60.001");
    model->setData(model->index(1, 4), "60.000-144.000");
    model->item(1, 4)->setFlags(model->item(1, 4)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    // 填充空列
    model->setData(model->index(1, 5), "");
    model->item(1, 5)->setFlags(model->item(1, 5)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    // 创建 QLineEdit 并嵌入到 "Input Jitter" 列
    QLineEdit *secJitterEdit = new QLineEdit(tableView);
    secJitterEdit->setClearButtonEnabled(true);
    tableView->setIndexWidget(model->index(1, 6), secJitterEdit);
    secJitterEdit->setText("0.010");
    // 创建 QComboBox 并嵌入到 "Source" 列
    QComboBox *secSourceBox = new QComboBox(tableView);
    secSourceBox->addItems({"Single ended clock capable pin", "Differential clock capable pin", "Global buffer", "No buffer"});
    tableView->setIndexWidget(model->index(1, 7), secSourceBox);
    tableView->resizeTableView();

    mainLayout->addStretch(); // 添加一个弹簧以将控件聚集到顶部
}
