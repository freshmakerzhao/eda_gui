#include "IPManager.h"
#include "BlockMemoryGenerator/BlockMemoryGenerator.h"

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
    toolBar->setIconSize(QSize(28, 28));
    QAction *expandAllAction = new QAction(QIcon(":/resource/icons/ExpandAll.png"),"ExpandAll", this);
    QAction *collapseAllAction = new QAction(QIcon(":/resource/icons/CollapseAll.png"),"CollapseAll", this);
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

    QStandardItem *rootItem = new QStandardItem(QString("Vivado Repository"));
    model->setItem(0, 0, rootItem);
    QStandardItem *basicelsmentsitem = new QStandardItem(QString("Basic Elements"));
    rootItem->appendRow(basicelsmentsitem);
    QStandardItem *memoryelementsitem = new QStandardItem(QString("Memory Elements"));
    basicelsmentsitem->appendRow(memoryelementsitem);
    blockmemorygeneratoritem = new QStandardItem(QString("Block Memory Generator"));
    QList<QStandardItem *> rowItems;
    rowItems.append(blockmemorygeneratoritem);
    rowItems.append(new QStandardItem(QString("AXI4")));
    rowItems.append(new QStandardItem(QString("Production")));
    rowItems.append(new QStandardItem(QString("Included")));
    rowItems.append(new QStandardItem(QString("xilinx.com:ip:blk_mem_gen:8.4")));
    // memoryelementsitem->appendRow(blockmemorygeneratoritem);
    memoryelementsitem->appendRow(rowItems);
    blockmemorygeneratoritem->setData("blockmemorygenerator", Qt::UserRole);


    connect(treeView, &QTreeView::doubleClicked, this, &IPManager::clickedIP);

    treeView->expandAll();
}

void IPManager::clickedIP(const QModelIndex &index)
{
    //取选中的这行的第一个元素的index
    const QModelIndex &idx = index.sibling(index.row(),0);
    if (!idx.isValid()) {
        return;
    }
    const QString ipName = idx.data(Qt::UserRole).toString();
    // qDebug() << idx;
    if (ipName == "blockmemorygenerator") {
        BlockMemoryGenerator blockMemoryGenerator(this);
        blockMemoryGenerator.exec();
    }

}
