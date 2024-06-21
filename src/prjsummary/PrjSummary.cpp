/**
  ******************************************************************************
  * @file           : PrjSummary.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 19/6/2024
  ******************************************************************************
  */
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QTableWidget>
#include "PrjSummary.h"
#include <QHeaderView>

PrjSummary *PrjSummary::instance()
{
    static PrjSummary *_instance = nullptr;
    if (!_instance) {
        _instance = new PrjSummary(nullptr);
    }
    return _instance;
}

PrjSummary::PrjSummary(QWidget* parent)
        : QWidget(parent)
{
    resize(800, 500);
    init();
}


void PrjSummary::init()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // ==================================  顶部的 Widget 开始 ==================================
    QWidget *topWidget = new QWidget();
    QVBoxLayout *settingsGroupLayout = new QVBoxLayout(topWidget);
    topWidget->setFixedHeight(250);
    // ================================== 顶部标题部分 开始 ==================================
    QLabel *titleLabel = new QLabel("Setting");
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setAlignment(Qt::AlignVCenter);
    titleLabel->setFixedHeight(25);
    titleLabel->setStyleSheet(
                              "font-weight: bold; font-size: 12px; "
                              "background-color: rgb(237, 237, 237);"
    );
    // ================================== 顶部标题部分 结束 ==================================

    // ================================== Setting 内容 开始 ==================================
    QWidget *settingsWidget = new QWidget();
    settingsWidget->setStyleSheet("background-color: rgb(247, 247, 247);");
    QFormLayout *settingsLayout = new QFormLayout(settingsWidget);

    // Project name
    QLabel *settingsPrjNameLabel = new QLabel("Project name:");
    QLabel *settingsPrjNameValueLabel = new QLabel(settingsPrjName);
    // Project location
    QLabel *settingsPrjLocationLabel = new QLabel("Project location:");
    QLabel *settingsPrjLocationValueLabel = new QLabel(settingsPrjLocation);
    // Product family
    QLabel *settingsPrdFamilyLabel = new QLabel("Product family:");
    QLabel *settingsPrdFamilyValueLabel = new QLabel(settingsPrjFamily);

    settingsPrjNameLabel->setFixedHeight(rowHeight);
    settingsPrjNameLabel->setFixedWidth(leftLabelWidth);
    settingsPrjNameValueLabel->setFixedHeight(rowHeight);
    settingsPrjNameValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    settingsPrjLocationLabel->setFixedHeight(rowHeight);
    settingsPrjLocationLabel->setFixedWidth(leftLabelWidth);
    settingsPrjLocationValueLabel->setFixedHeight(rowHeight);
    settingsPrjLocationValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    settingsPrdFamilyLabel->setFixedHeight(rowHeight);
    settingsPrdFamilyLabel->setFixedWidth(leftLabelWidth);
    settingsPrdFamilyValueLabel->setFixedHeight(rowHeight);
    settingsPrdFamilyValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    settingsLayout->addRow(settingsPrjNameLabel, settingsPrjNameValueLabel); // Project name
    settingsLayout->addRow(settingsPrjLocationLabel, settingsPrjLocationValueLabel); // Project location
    settingsLayout->addRow(settingsPrdFamilyLabel, settingsPrdFamilyValueLabel); // Product family

    QString buttonStyle =
            "QPushButton { "
                "border: none; "
                "color: rgb(22, 97, 247); "
                "background-color: transparent; "
                "text-align: left; "
            "} "
            "QPushButton:hover { text-decoration: underline; }"
    ;

    // Project part
    QLabel *settingsPrjPartLabel = new QLabel("Project part:");
    QPushButton *settingsPrjPartValueButton = new QPushButton(settingsPrjPart);
    // Top module name
    QLabel *settingsPrjTopModuleLabel = new QLabel("Top module name:");
    QPushButton *settingsPrjTopModuleValueButton = new QPushButton(settingsPrjTopModuleName);

    settingsPrjPartLabel->setFixedHeight(rowHeight);
    settingsPrjPartLabel->setFixedWidth(leftLabelWidth);
    settingsPrjPartValueButton->setFixedHeight(rowHeight);
    settingsPrjPartValueButton->setStyleSheet(buttonStyle);
    settingsPrjPartValueButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    settingsPrjTopModuleLabel->setFixedHeight(rowHeight);
    settingsPrjTopModuleLabel->setFixedWidth(leftLabelWidth);
    settingsPrjTopModuleValueButton->setFixedHeight(rowHeight);
    settingsPrjTopModuleValueButton->setStyleSheet(buttonStyle);
    settingsPrjTopModuleValueButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    settingsLayout->addRow(settingsPrjPartLabel, settingsPrjPartValueButton); // Project part
    settingsLayout->addRow(settingsPrjTopModuleLabel, settingsPrjTopModuleValueButton); // Top module name

    // 将标题和settingsWidget添加到topWidget的布局中
    settingsGroupLayout->addWidget(titleLabel);
    settingsGroupLayout->addWidget(settingsWidget);

    // 将topWidget添加到主布局中
    mainLayout->addWidget(topWidget, 0, Qt::AlignTop);
    // ================================== Setting 内容 结束 ==================================

    // ==================================  顶部的 Widget 结束 ==================================

    // ================================== 下方布局部分 开始 ==================================

    QWidget *bottomWidget = new QWidget();
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    // 去除 bottomLayout 布局的边距,使其能够与上方对齐
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    // ================================== 下方 左侧 布局 开始 ==================================

    // ================================== 下方左侧 synthesis 开始 ==================================

    // ================================== 下方左侧 synthesis title 开始 ==================================

    QLabel *synthesisTitleLabel = new QLabel("Synthesis");
    synthesisTitleLabel->setAlignment(Qt::AlignLeft);
    synthesisTitleLabel->setAlignment(Qt::AlignVCenter);
    synthesisTitleLabel->setFixedHeight(25);
    synthesisTitleLabel->setStyleSheet("font-weight: bold; font-size: 12px; background-color: rgb(237, 237, 237);");

    // ================================== 下方左侧 synthesis title 结束 ==================================

    // ================================== 下方左侧 synthesis 内容 开始 ==================================

    QVBoxLayout *leftLayout = new QVBoxLayout;
    QWidget *synthesisWidget = new QWidget;

    synthesisWidget->setStyleSheet("background-color: rgb(247, 247, 247);");
    synthesisWidget->setFixedHeight(95);
    QFormLayout *synthesisLayout = new QFormLayout(synthesisWidget);

    // Synthesis Status
    QLabel *synthesisStatusLabel = new QLabel("Status:");
    QLabel *synthesisStatusValueLabel = new QLabel(synthesisStatus);
    // Synthesis Part
    QLabel *synthesisPartLabel = new QLabel("Part");
    QLabel *synthesisPartValueLabel = new QLabel(synthesisPart);

    synthesisStatusLabel->setFixedHeight(rowHeight);
    synthesisStatusLabel->setFixedWidth(leftLabelWidth);
    synthesisStatusValueLabel->setFixedHeight(rowHeight);
    synthesisStatusValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    synthesisPartLabel->setFixedHeight(rowHeight);
    synthesisPartLabel->setFixedWidth(leftLabelWidth);
    synthesisPartValueLabel->setFixedHeight(rowHeight);
    synthesisPartValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    synthesisLayout->addRow(synthesisStatusLabel, synthesisStatusValueLabel); // Synthesis Status
    synthesisLayout->addRow(synthesisPartLabel, synthesisPartValueLabel); // Synthesis Part

    // ================================== 下方左侧 synthesis 内容 结束 ==================================

    // ================================== 下方左侧 Utilization title 开始 ==================================

    QLabel *utilizationTitleLabel = new QLabel("Utilization");
    utilizationTitleLabel->setAlignment(Qt::AlignLeft);
    utilizationTitleLabel->setAlignment(Qt::AlignVCenter);
    utilizationTitleLabel->setFixedHeight(25);
    utilizationTitleLabel->setStyleSheet("font-weight: bold; font-size: 12px; background-color: rgb(237, 237, 237);");

    // ================================== 下方左侧 Utilization title 结束 ==================================

    // ================================== 下方左侧 Utilization 内容 开始 ==================================

    QWidget *utilizationWidget = new QWidget;
    utilizationWidget->setStyleSheet("background-color: rgb(247, 247, 247);");
    QHBoxLayout *utilizationTableLayout = new QHBoxLayout(utilizationWidget);

    // 创建表格
    QTableWidget *tableWidget = new QTableWidget(6, 4);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Resource" << "Estimation" << "Available" << "Utilization %");

    // 使每列宽度平均分配整个表格
    QHeaderView *header = tableWidget->horizontalHeader();

    // 创建一个调色板并设置选中颜色
    QPalette palette = tableWidget->palette();
    palette.setColor(QPalette::Highlight, QColor(204, 222, 253));  // 设置选中背景色为黄色
    tableWidget->setPalette(palette);

    header->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false); // 隐藏行索引
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
    tableWidget->resizeColumnsToContents();  // 调整列宽以适应内容


    // 将表格添加到主布局
    utilizationTableLayout->addWidget(tableWidget);

    leftLayout->addWidget(synthesisTitleLabel);
    leftLayout->addWidget(synthesisWidget);
    leftLayout->addWidget(utilizationTitleLabel);
    leftLayout->addWidget(utilizationWidget);

    // 将左侧布局添加到主布局
    QWidget *leftContainer = new QWidget;
    leftContainer->setLayout(leftLayout);

    leftContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomLayout->addWidget(leftContainer);

    // ================================== 下方左侧 Utilization 内容 结束 ==================================

    // ================================== 下方 左侧 布局 结束 ==================================

    // ================================== 下方右侧 implementation title 开始 ==================================

    QLabel *implementationTitleLabel = new QLabel("Implementation");
    implementationTitleLabel->setAlignment(Qt::AlignLeft);
    implementationTitleLabel->setAlignment(Qt::AlignVCenter);
    implementationTitleLabel->setFixedHeight(25);
    implementationTitleLabel->setStyleSheet("font-weight: bold; font-size: 12px; background-color: rgb(237, 237, 237);");

    // ================================== 下方右侧 implementation title 结束 ==================================

    // ================================== 下方右侧 implementation 内容 开始 ==================================

    QVBoxLayout *rightLayout = new QVBoxLayout;
    QWidget *implementationWidget = new QWidget;
    implementationWidget->setStyleSheet("background-color: rgb(247, 247, 247);");
    implementationWidget->setFixedHeight(95);

    QFormLayout *implementationLayout = new QFormLayout(implementationWidget);

    // implementation Status
    QLabel *implementationStatusLabel = new QLabel("Status:");
    QLabel *implementationStatusValueLabel = new QLabel(implementationStatus);
    // implementation Part
    QLabel *implementationPartLabel = new QLabel("Part");
    QLabel *implementationPartValueLabel = new QLabel(implementationPart);

    implementationStatusLabel->setFixedHeight(rowHeight);
    implementationStatusLabel->setFixedWidth(rightlabelWidth);
    implementationStatusValueLabel->setFixedHeight(rowHeight);
    implementationStatusValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    implementationPartLabel->setFixedHeight(rowHeight);
    implementationPartLabel->setFixedWidth(rightlabelWidth);
    implementationPartValueLabel->setFixedHeight(rowHeight);
    implementationPartValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    implementationLayout->addRow(implementationStatusLabel, implementationStatusValueLabel); // implementation Status
    implementationLayout->addRow(implementationPartLabel, implementationPartValueLabel); // implementation Part

    // ================================== 下方右侧 implementation 内容 结束 ==================================

    // ================================== 下方右侧 Timing title 开始 ==================================

    QLabel *timingTitleLabel = new QLabel("Timing");
    timingTitleLabel->setAlignment(Qt::AlignLeft);
    timingTitleLabel->setAlignment(Qt::AlignVCenter);
    timingTitleLabel->setFixedHeight(25);
    timingTitleLabel->setStyleSheet("font-weight: bold; font-size: 12px; background-color: rgb(237, 237, 237);");

    // ================================== 下方右侧 Timing title 结束 ==================================

    // ================================== 下方右侧 timing 内容 开始 ==================================

    QWidget *timingWidget = new QWidget;
    timingWidget->setStyleSheet("background-color: rgb(247, 247, 247);");
    QFormLayout *timingLayout = new QFormLayout(timingWidget);

    // 最差的负的时差
    QLabel *timingWorstNegativeSlackLabel = new QLabel("Worst Negative Slack(WNS):");
    QLabel *timingWorstNegativeSlackValueLabel = new QLabel(timingWorstNegativeSlack);

    // 负的时差总数
    QLabel *timingTotalNegativeSlackLabel = new QLabel("Total Negative Slack(TNS):");
    QLabel *timingTotalNegativeSlackValueLabel = new QLabel(timingTotalNegativeSlack);

    // 失败端点的数量
    QLabel *timingNumberOfFailingEndpointsLabel = new QLabel("Number Of Failing Endpoints:");
    QLabel *timingNumberOfFailingEndpointsValueLabel = new QLabel(timingNumberOfFailingEndpoints);

    // 节点总数
    QLabel *timingTotalNumberOfEndpointsLabel = new QLabel("Total Number Of Endpoints:");
    QLabel *timingTotalNumberOfEndpointsValueLabel = new QLabel(timingTotalNumberOfEndpoints);

    timingWorstNegativeSlackLabel->setFixedHeight(rowHeight);
    timingWorstNegativeSlackLabel->setFixedWidth(rightlabelWidth);
    timingWorstNegativeSlackValueLabel->setFixedHeight(rowHeight);
    timingWorstNegativeSlackValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    timingTotalNegativeSlackLabel->setFixedHeight(rowHeight);
    timingTotalNegativeSlackLabel->setFixedWidth(rightlabelWidth);
    timingTotalNegativeSlackValueLabel->setFixedHeight(rowHeight);
    timingTotalNegativeSlackValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    timingNumberOfFailingEndpointsLabel->setFixedHeight(rowHeight);
    timingNumberOfFailingEndpointsLabel->setFixedWidth(rightlabelWidth);
    timingNumberOfFailingEndpointsValueLabel->setFixedHeight(rowHeight);
    timingNumberOfFailingEndpointsValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    timingTotalNumberOfEndpointsLabel->setFixedHeight(rowHeight);
    timingTotalNumberOfEndpointsLabel->setFixedWidth(rightlabelWidth);
    timingTotalNumberOfEndpointsValueLabel->setFixedHeight(rowHeight);
    timingTotalNumberOfEndpointsValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    timingLayout->addRow(timingWorstNegativeSlackLabel, timingWorstNegativeSlackValueLabel); // Synthesis Status
    timingLayout->addRow(timingTotalNegativeSlackLabel, timingTotalNegativeSlackValueLabel); // Synthesis Part
    timingLayout->addRow(timingNumberOfFailingEndpointsLabel, timingNumberOfFailingEndpointsValueLabel); // Synthesis Part
    timingLayout->addRow(timingTotalNumberOfEndpointsLabel, timingTotalNumberOfEndpointsValueLabel); // Synthesis Part

    // ================================== 下方右侧 timing 内容 结束 ==================================

    rightLayout->addWidget(implementationTitleLabel);
    rightLayout->addWidget(implementationWidget);
    rightLayout->addWidget(timingTitleLabel);
    rightLayout->addWidget(timingWidget);

    // 将右侧布局添加到主布局
    QWidget *rightContainer = new QWidget;
    rightContainer->setLayout(rightLayout);
    rightContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomLayout->addWidget(rightContainer);

    // ================================== 下方布局部分 结束 ==================================
    mainLayout->addWidget(bottomWidget, 0, Qt::AlignTop);
    // 添加一个弹簧，将它放在布局的底部
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(verticalSpacer);
    // 将子窗口添加到主布局中，不允许扩展
    mainLayout->setAlignment(topWidget, Qt::AlignTop); // 设置顶部对齐
    setLayout(mainLayout);
    setStyleSheet("background-color: rgb(247, 247, 247);");
}
