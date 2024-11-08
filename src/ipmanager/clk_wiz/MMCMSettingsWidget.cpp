#include "MMCMSettingsWidget.h"

MMCMSettingsWidget::MMCMSettingsWidget(QWidget *parent) :
    BasePage(parent)
{
    QLabel *label = new QLabel("These are the settings based on inputs from previous pages. Any update on this page \n"
                               "will override the optimal settings calculated by the wizard", this);
    mainLayout->addWidget(label);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addLayout(hBoxLayout);

    allowOverrideModeCheckBox = new QCheckBox("Allow Override Mode", this);
    hBoxLayout->addWidget(allowOverrideModeCheckBox);

    // -------------------------------------- Attribute ------------------------------------------
    attributeModel = new QStandardItemModel;

    attributeTableView = new AdvancedTableView(this);
    attributeTableView->verticalHeader()->setVisible(false);
    mainLayout->addWidget(attributeTableView);

    QStringList attributeHeaders;
    attributeHeaders << "Attribute" << "Value";
    attributeModel->setHorizontalHeaderLabels(attributeHeaders);

    attributeTableView->setModel(attributeModel);
    attributeModel->insertRows(0, 12);

    attributeModel->setData(attributeModel->index(0, 0), "BANDWIDTH");
    attributeModel->item(0, 0)->setFlags(attributeModel->item(0, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QComboBox *bandWidthComboBox = new QComboBox(attributeTableView);
    bandWidthComboBox->addItems({"OPTIMIZED", "LOW", "HIGH"});
    attributeTableView->setIndexWidget(attributeModel->index(0, 1), bandWidthComboBox);

    attributeModel->setData(attributeModel->index(1, 0), "CLKFBOUT_MULT_F");
    attributeModel->item(1, 0)->setFlags(attributeModel->item(1, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *clkfboutMultFLineEdit = new QLineEdit("10.000", attributeTableView);
    clkfboutMultFLineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(1, 1), clkfboutMultFLineEdit);

    attributeModel->setData(attributeModel->index(2, 0), "CLKFBOUT_PHASE");
    attributeModel->item(2, 0)->setFlags(attributeModel->item(2, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *clkfboutPhaseLineEdit = new QLineEdit("10.000", attributeTableView);
    clkfboutPhaseLineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(2, 1), clkfboutPhaseLineEdit);

    attributeModel->setData(attributeModel->index(3, 0), "CLKIN1_PERIOD");
    attributeModel->item(3, 0)->setFlags(attributeModel->item(3, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *clkin1PeriodLineEdit = new QLineEdit("10.000", attributeTableView);
    clkin1PeriodLineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(3, 1), clkin1PeriodLineEdit);

    attributeModel->setData(attributeModel->index(4, 0), "CLKIN2_PERIOD");
    attributeModel->item(4, 0)->setFlags(attributeModel->item(4, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *clkin2PeriodLineEdit = new QLineEdit("10.000", attributeTableView);
    // clkin2PeriodLineEdit->setClearButtonEnabled(true);
    clkin2PeriodLineEdit->setEnabled(false);
    attributeTableView->setIndexWidget(attributeModel->index(4, 1), clkin2PeriodLineEdit);

    attributeModel->setData(attributeModel->index(5, 0), "COMPENSATION");
    attributeModel->item(5, 0)->setFlags(attributeModel->item(5, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QComboBox *compensationComboBox = new QComboBox(attributeTableView);
    compensationComboBox->addItems({"ZHOLD", "EXTERNAL", "INTERNAL", "BUF IN"});
    attributeTableView->setIndexWidget(attributeModel->index(5, 1), compensationComboBox);

    attributeModel->setData(attributeModel->index(6, 0), "DICLK_DIVIDE");
    attributeModel->item(6, 0)->setFlags(attributeModel->item(6, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *diclkDivideLineEdit = new QLineEdit("1", attributeTableView);
    diclkDivideLineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(6, 1), diclkDivideLineEdit);

    attributeModel->setData(attributeModel->index(7, 0), "REF_JITTER1");
    attributeModel->item(7, 0)->setFlags(attributeModel->item(7, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *refJitter1LineEdit = new QLineEdit("0.010", attributeTableView);
    refJitter1LineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(7, 1), refJitter1LineEdit);

    attributeModel->setData(attributeModel->index(8, 0), "REF_JITTER2");
    attributeModel->item(8, 0)->setFlags(attributeModel->item(8, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *refJitter2LineEdit = new QLineEdit("0.010", attributeTableView);
    refJitter2LineEdit->setClearButtonEnabled(true);
    attributeTableView->setIndexWidget(attributeModel->index(8, 1), refJitter2LineEdit);

    attributeModel->setData(attributeModel->index(9, 0), "STARTUP_WAIT");
    attributeModel->item(9, 0)->setFlags(attributeModel->item(9, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QWidget *startupWaitWidget = new QWidget(attributeTableView);
    QCheckBox *startupWaitCheckBox = new QCheckBox(this);
    QHBoxLayout *startupWaitLayout = new QHBoxLayout();
    startupWaitLayout->addWidget(startupWaitCheckBox);
    startupWaitLayout->setMargin(0);
    startupWaitLayout->setAlignment(startupWaitCheckBox, Qt::AlignCenter);
    startupWaitWidget->setLayout(startupWaitLayout);
    attributeTableView->setIndexWidget(attributeModel->index(9, 1), startupWaitWidget);

    attributeModel->setData(attributeModel->index(10, 0), "CLKFBOUT_USE_FINE_PS");
    attributeModel->item(10, 0)->setFlags(attributeModel->item(10, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QWidget *clkfbootUseFinePsWidget = new QWidget(attributeTableView);
    QCheckBox *clkfbootUseFinePsCheckBox = new QCheckBox(this);
    QHBoxLayout *clkfbootUseFinePsLayout = new QHBoxLayout();
    clkfbootUseFinePsLayout->addWidget(clkfbootUseFinePsCheckBox);
    clkfbootUseFinePsLayout->setMargin(0);
    clkfbootUseFinePsLayout->setAlignment(clkfbootUseFinePsCheckBox, Qt::AlignCenter);
    clkfbootUseFinePsWidget->setLayout(clkfbootUseFinePsLayout);
    attributeTableView->setIndexWidget(attributeModel->index(10, 1), clkfbootUseFinePsWidget);

    attributeModel->setData(attributeModel->index(11, 0), "CLKOUT4_CASCADE");
    attributeModel->item(11, 0)->setFlags(attributeModel->item(11, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QWidget *clkout4CascadeWidget = new QWidget(attributeTableView);
    QCheckBox *clkout4CascadeCheckBox = new QCheckBox(this);
    QHBoxLayout *clkout4CascadeLayout = new QHBoxLayout();
    clkout4CascadeLayout->addWidget(clkout4CascadeCheckBox);
    clkout4CascadeLayout->setMargin(0);
    clkout4CascadeLayout->setAlignment(clkout4CascadeCheckBox, Qt::AlignCenter);
    clkout4CascadeWidget->setLayout(clkout4CascadeLayout);
    attributeTableView->setIndexWidget(attributeModel->index(11, 1), clkout4CascadeWidget);
    attributeTableView->resizeTableView();
    // -------------------------------------- Port ------------------------------------------
    portModel = new QStandardItemModel(8, 7);

    portTableView = new AdvancedTableView(this);
    portTableView->verticalHeader()->setVisible(false);
    mainLayout->addWidget(portTableView);

    QStringList portHeaders;
    portHeaders << "Clk Wizard Port" << "Renamed Port" << "MMCM/PLL Port" << "Divide" << "Duty Cycle" << "Phase" << "Use Fine Ps";
    portModel->setHorizontalHeaderLabels(portHeaders);

    portTableView->setModel(portModel);

    // Clk Wizard Port
    for (int i = 0; i < portModel->rowCount(); i++) {
        portModel->setData(portModel->index(i, 0), QString("clk_out%1").arg(i));
        portModel->item(i, 0)->setFlags(portModel->item(i, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    // Renamed Port
    for (int i = 0; i < portModel->rowCount(); i++) {
        portModel->setData(portModel->index(i, 1), QString("clk_out%1").arg(i));
        portModel->item(i, 1)->setFlags(portModel->item(i, 1)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    // MMCM/PLL Port
    for (int i = 0; i < portModel->rowCount(); i++) {
        portModel->setData(portModel->index(i, 2), QString("CLKOUT%1").arg(i));
        portModel->item(i, 2)->setFlags(portModel->item(i, 2)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    // Divide
    for (int i = 0; i < portModel->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("10.000");
        lineEdit->setClearButtonEnabled(true);
        portTableView->setIndexWidget(portModel->index(i, 3), lineEdit);
    }

    // Duty Cycle
    for (int i = 0; i < portModel->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("0.500");
        lineEdit->setClearButtonEnabled(true);
        portTableView->setIndexWidget(portModel->index(i, 4), lineEdit);
    }

    // Phase
    for (int i = 0; i < portModel->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("0.000");
        lineEdit->setClearButtonEnabled(true);
        portTableView->setIndexWidget(portModel->index(i, 5), lineEdit);
    }

    // Use Fine Ps
    for (int i = 0; i < portModel->rowCount(); i++) {
        QWidget *widget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(checkBox);
        hLayout->setMargin(0);
        hLayout->setAlignment(checkBox, Qt::AlignCenter);
        widget->setLayout(hLayout);
        portTableView->setIndexWidget(portModel->index(i, 6), widget);
    }

    portTableView->resizeTableView();

    mainLayout->addStretch();
}

