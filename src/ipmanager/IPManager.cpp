#include "IPManager.h"

IPManager *IPManager::instance()
{
    static IPManager *_instance = nullptr;
    if (!_instance) {
        _instance = new IPManager(nullptr);
    }
    return _instance;
}

IPManager::IPManager(QWidget* parent)
    : QWidget(parent)
{
    resize(800, 500);
    init();
}

void IPManager::init()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));
    QAction *expandAllAction = new QAction("ExpandAll", this);
    QAction *collapseAllAction = new QAction("CollapseAll", this);
    toolBar->addAction(expandAllAction);
    toolBar->addSeparator();
    toolBar->addAction(collapseAllAction);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);

    treeView = new QTreeView(this);
    model = new QStandardItemModel(treeView);
    QStringList headers = {"Name",
                           "AXI4",
                           "Status",
                           "License",
                           "VLNV"
    };
    model->setHorizontalHeaderLabels(headers);
    treeView->setModel(model);
    treeView->setColumnWidth(0, 360);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // treeView->setAlternatingRowColors(free); // 交替显示
    treeView->setStyleSheet("QTreeView { border: 1px solid #999; }"
                            "QTreeView::item { border-bottom: 1px solid #999; border-right: 1px solid #999;}"
                            "QTreeView::item:selected { background-color: #4f7cce; }");

    connect(expandAllAction, &QAction::triggered, [this]() {
        treeView->expandAll();
    });

    connect(collapseAllAction, &QAction::triggered, [this]() {
        treeView->collapseAll();
    });

    vlayout->addWidget(treeView);
    vlayout->setMargin(0);

    QWidget *detailWidget = new QWidget(this);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    QWidget *baseWidget = new QWidget(this);
    baseWidget->setLayout(vlayout);
    splitter->addWidget(baseWidget);
    splitter->addWidget(detailWidget);

    QHBoxLayout *vlayout2 = new QHBoxLayout(this);
    vlayout2->setMargin(0);
    vlayout2->addWidget(splitter);

    QStandardItem *item1 = new QStandardItem(QString("Vivado Repository"));
    model->setItem(0, 0, item1);
    QStandardItem *item2 = new QStandardItem(QString("Alliance Partners"));
    item1->appendRow(item2);
    QStandardItem *item3 = new QStandardItem(QString("Arm"));
    item2->appendRow(item3);
    QStandardItem *item4 = new QStandardItem(QString("Arm Cortex-M1 Processor"));
    item3->appendRow(item4);
    QStandardItem *item5 = new QStandardItem(QString("Arm Cortex-M3 Processor"));
    item3->appendRow(item5);

    for (int i = 0; i < 5; ++i) {
        QList<QStandardItem *> rowItems;
        for (int j = 0; j < headers.size(); ++j) {
            QString data = "Data " + QString::number(i + 1) + "-" + QString::number(j + 1);
            QStandardItem *item = new QStandardItem(data);
            rowItems.append(item);
        }
        model->appendRow(rowItems);
    }

    treeView->expandAll();
}
