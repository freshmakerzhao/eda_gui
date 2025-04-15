#include "PowerWidget.h"

PowerWidget::PowerWidget(QWidget *parent)
    : QWidget(parent),
    treeView(new QTreeView),
    stackedWidget(new QStackedWidget),
    _model(new QStandardItemModel(this))
{
    setStyleSheet(".QWidget, .QStackedWidget { background-color: white; }");
    // stackedWidget->setStyleSheet(".QStackedWidget { background-color: white; }");

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);

    treeView->setModel(_model);
    treeView->setHeaderHidden(true);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(treeView);
    splitter->addWidget(stackedWidget);

    vBoxLayout->addWidget(splitter);
    splitter->setHandleWidth(10);

    QList<int> list;
    list << 500 << 1100;
    splitter->setSizes(list);

    powerSettingsWidget = new PowerSettingsWidget;
    powerSummaryWidget = new PowerSummaryWidget;
    clockUtilizationWidget = new UtilizationWidget(Utilization::Clock);
    logicUtilizationWidget = new UtilizationWidget(Utilization::Logic);

    loadDataFromJson();
    setupTreeView();
    setupStackedWidget();


    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PowerWidget::onTreeSelectionChanged);

    treeView->expandAll();

    connect(powerSummaryWidget->getPowerChartPtr(), &PowerChart::dynamicPowerLegendClick, this, &PowerWidget::onDynamicPowerLegendClick);
}
