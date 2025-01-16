/**
  ******************************************************************************
  * @file           : ProjectSummary.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/19
  ******************************************************************************
  */
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QTableWidget>
#include "ProjectSummary.h"
#include <QHeaderView>

ProjectSummary *ProjectSummary::instance()
{
    static ProjectSummary *_instance = nullptr;
    if (!_instance) {
        _instance = new ProjectSummary(nullptr);
    }
    return _instance;
}

ProjectSummary::ProjectSummary(QWidget* parent)
        : QWidget(parent)
{
    init();
}


void ProjectSummary::init()
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    vBoxLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    // ==================================  顶部的 Widget 开始 ==================================
    QWidget *topWidget = new QWidget();
    QVBoxLayout *settingsGroupLayout = new QVBoxLayout(topWidget);
    // ================================== 顶部标题部分 开始 ==================================
    QLabel *titleLabel = new QLabel("Setting");
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setAlignment(Qt::AlignVCenter);
    titleLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    // ================================== 顶部标题部分 结束 ==================================

    // ================================== Setting 内容 开始 ==================================
    QWidget *settingsWidget = new QWidget();
    settingsWidget->setStyleSheet(CONTENT_WIDGET_STYLESHEET);
    QFormLayout *settingsLayout = new QFormLayout(settingsWidget);

    // Project name
    QLabel *settingsPrjNameLabel = new QLabel("Project name:");
    QLabel *settingsPrjNameValueLabel = settingsPrjName;
    // Project location
    QLabel *settingsPrjLocationLabel = new QLabel("Project location:");
    QLabel *settingsPrjLocationValueLabel = settingsPrjLocation;
    // Product family
    QLabel *settingsPrdFamilyLabel = new QLabel("Product family:");
    QLabel *settingsPrdFamilyValueLabel = settingsPrjFamily;

    settingsPrjNameValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    settingsPrjLocationValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
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
    QPushButton *settingsPrjPartValueButton = settingsPrjPart;
    // Top module name
    QLabel *settingsPrjTopModuleLabel = new QLabel("Top module name:");
    QPushButton *settingsPrjTopModuleValueButton = settingsPrjTopModuleName;

    settingsPrjPartValueButton->setStyleSheet(buttonStyle);
    settingsPrjPartValueButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

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
    // 分割左右部分
    splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(10);
    // ================================== 下方 左侧 布局 开始 ==================================

    // ================================== 下方左侧 synthesis 开始 ==================================

    // ================================== 下方左侧 synthesis title 开始 ==================================

    QLabel *synthesisTitleLabel = new QLabel("Synthesis");
    synthesisTitleLabel->setAlignment(Qt::AlignLeft);
    synthesisTitleLabel->setAlignment(Qt::AlignVCenter);
    synthesisTitleLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);

    // ================================== 下方左侧 synthesis title 结束 ==================================

    // ================================== 下方左侧 synthesis 内容 开始 ==================================

    QVBoxLayout *leftLayout = new QVBoxLayout;
    QWidget *synthesisWidget = new QWidget;

    synthesisWidget->setStyleSheet(CONTENT_WIDGET_STYLESHEET);
    QFormLayout *synthesisLayout = new QFormLayout(synthesisWidget);

    // Synthesis Status
    QLabel *synthesisStatusLabel = new QLabel("Status:");
    QLabel *synthesisStatusValueLabel = synthesisStatus;
    // Synthesis Part
    QLabel *synthesisPartLabel = new QLabel("Part");
    QLabel *synthesisPartValueLabel = synthesisPart;

    synthesisStatusValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    synthesisPartValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    synthesisLayout->addRow(synthesisStatusLabel, synthesisStatusValueLabel); // Synthesis Status
    synthesisLayout->addRow(synthesisPartLabel, synthesisPartValueLabel); // Synthesis Part

    // ================================== 下方左侧 synthesis 内容 结束 ==================================

    // ================================== 下方左侧 Utilization title 开始 ==================================

    QLabel *utilizationTitleLabel = new QLabel("Utilization");
    utilizationTitleLabel->setAlignment(Qt::AlignLeft);
    utilizationTitleLabel->setAlignment(Qt::AlignVCenter);
    utilizationTitleLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);

    // ================================== 下方左侧 Utilization title 结束 ==================================

    // ================================== 下方左侧 Utilization 内容 开始 ==================================

    QWidget *utilizationWidget = new QWidget;
    utilizationWidget->setStyleSheet(CONTENT_WIDGET_STYLESHEET);
    QHBoxLayout *utilizationTableLayout = new QHBoxLayout(utilizationWidget);

    // 创建表格
    QTableWidget *tableWidget = new QTableWidget(6, 4);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Resource" << "Estimation" << "Available" << "Utilization %");

    // 使每列宽度平均分配整个表格
    QHeaderView *header = tableWidget->horizontalHeader();

    // 创建一个调色板并设置选中颜色
    QPalette palette = tableWidget->palette();
    palette.setColor(QPalette::Highlight, QColor(204, 222, 253));  // 设置选中背景色
    tableWidget->setPalette(palette);

    // header->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false); // 隐藏行索引
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
    tableWidget->resizeColumnsToContents();  // 调整列宽以适应内容


    // 将表格添加到主布局
    utilizationTableLayout->addWidget(tableWidget);

    leftLayout->addWidget(synthesisTitleLabel);
    leftLayout->addWidget(synthesisWidget);
    leftLayout->addWidget(utilizationTitleLabel);
    leftLayout->addWidget(utilizationWidget);
    leftLayout->addStretch();

    // 将左侧布局添加到主布局
    QWidget *leftContainer = new QWidget;
    leftContainer->setLayout(leftLayout);

    leftContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftContainer->setMinimumWidth(500);
    splitter->addWidget(leftContainer);

    // ================================== 下方左侧 Utilization 内容 结束 ==================================

    // ================================== 下方 左侧 布局 结束 ==================================

    // ================================== 下方右侧 implementation title 开始 ==================================

    QLabel *implementationTitleLabel = new QLabel("Implementation");
    implementationTitleLabel->setAlignment(Qt::AlignLeft);
    implementationTitleLabel->setAlignment(Qt::AlignVCenter);
    implementationTitleLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);

    // ================================== 下方右侧 implementation title 结束 ==================================

    // ================================== 下方右侧 implementation 内容 开始 ==================================

    QVBoxLayout *rightLayout = new QVBoxLayout;
    QWidget *implementationWidget = new QWidget;
    implementationWidget->setStyleSheet(CONTENT_WIDGET_STYLESHEET);

    QFormLayout *implementationLayout = new QFormLayout(implementationWidget);

    // implementation Status
    QLabel *implementationStatusLabel = new QLabel("Status:");
    QLabel *implementationStatusValueLabel = implementationStatus;
    // implementation Part
    QLabel *implementationPartLabel = new QLabel("Part");
    QLabel *implementationPartValueLabel = implementationPart;

    implementationStatusValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    implementationPartValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    implementationLayout->addRow(implementationStatusLabel, implementationStatusValueLabel); // implementation Status
    implementationLayout->addRow(implementationPartLabel, implementationPartValueLabel); // implementation Part

    // ================================== 下方右侧 implementation 内容 结束 ==================================

    // ================================== 下方右侧 Timing title 开始 ==================================

    QLabel *timingTitleLabel = new QLabel("Timing");
    timingTitleLabel->setAlignment(Qt::AlignLeft);
    timingTitleLabel->setAlignment(Qt::AlignVCenter);
    timingTitleLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);

    // ================================== 下方右侧 Timing title 结束 ==================================

    // ================================== 下方右侧 timing 内容 开始 ==================================

    QWidget *timingWidget = new QWidget;
    timingWidget->setStyleSheet(CONTENT_WIDGET_STYLESHEET);
    QFormLayout *timingLayout = new QFormLayout(timingWidget);

    // 最差的负的时差
    QLabel *timingWorstNegativeSlackLabel = new QLabel("Worst Negative Slack(WNS):");
    QLabel *timingWorstNegativeSlackValueLabel = timingWorstNegativeSlack;

    // 负的时差总数
    QLabel *timingTotalNegativeSlackLabel = new QLabel("Total Negative Slack(TNS):");
    QLabel *timingTotalNegativeSlackValueLabel = timingTotalNegativeSlack;

    // 失败端点的数量
    QLabel *timingNumberOfFailingEndpointsLabel = new QLabel("Number Of Failing Endpoints:");
    QLabel *timingNumberOfFailingEndpointsValueLabel = timingNumberOfFailingEndpoints;

    // 节点总数
    QLabel *timingTotalNumberOfEndpointsLabel = new QLabel("Total Number Of Endpoints:");
    QLabel *timingTotalNumberOfEndpointsValueLabel = timingTotalNumberOfEndpoints;

    timingWorstNegativeSlackValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    timingTotalNegativeSlackValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    timingNumberOfFailingEndpointsValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
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
    rightLayout->addStretch();

    // 将右侧布局添加到主布局
    QWidget *rightContainer = new QWidget;
    rightContainer->setLayout(rightLayout);
    rightContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(rightContainer);
    // ================================== 下方布局部分 结束 ==================================
    mainLayout->addWidget(splitter, 0, Qt::AlignTop);
    // 添加一个弹簧，将它放在布局的底部
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(verticalSpacer);
    // 将子窗口添加到主布局中，不允许扩展
    mainLayout->setAlignment(topWidget, Qt::AlignTop); // 设置顶部对齐
    setStyleSheet(CONTENT_WIDGET_STYLESHEET);
}

void ProjectSummary::setParams(const QMap<Project::ParamKey, QString> &params) {

    settingsPrjName->setText(params[Project::Name]);
    settingsPrjLocation->setText(params[Project::Path]);
    settingsPrjFamily->setText(params[Project::FamilyName]);
    settingsPrjPart->setText(params[Project::DisplayPart]);
    settingsPrjTopModuleName->setText(params[Project::TopModule]);

//    QString synthesisStatus = "Complete";
//    QString synthesisPart = "xc7a35tfgg484-2";

//    QString implementationStatus = "Complete";
//    QString implementationPart = "xc7a35tfgg484-2";

    qDebug() << "[ProjectSummary] ProjectName: " << params[Project::Name];
    qDebug() << "[ProjectSummary] ProjectLocation: " << params[Project::Path];
    qDebug() << "[ProjectSummary] ProjectFamily: " << params[Project::FamilyName];
    qDebug() << "[ProjectSummary] ProjectPart: " << params[Project::DisplayPart];
    qDebug() << "[ProjectSummary] ProjectTopModuleName: " << params[Project::TopModule];
}
