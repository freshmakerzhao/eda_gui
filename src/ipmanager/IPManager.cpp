#include "IPManager.h"
#include "clk_wiz/ClockingWizard.h"
#include "utils/ProjectManager.h"
#include "widgets/FileManager.h"

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

    connect(expandAllAction, &QAction::triggered, [this]() {
        treeView->expandAll();
    });

    connect(collapseAllAction, &QAction::triggered, [this]() {
        treeView->collapseAll();
    });

    vlayout->addWidget(treeView);
    vlayout->setMargin(0);

    // QWidget *detailWidget = new QWidget(this);
    detailLabel = new QLabel("Select an lP to see details");

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    QWidget *baseWidget = new QWidget(this);
    baseWidget->setLayout(vlayout);
    splitter->addWidget(baseWidget);
    splitter->addWidget(detailLabel);

    QHBoxLayout *vlayout2 = new QHBoxLayout(this);
    vlayout2->setMargin(0);
    vlayout2->addWidget(splitter);

    QStandardItem *rootItem = new QStandardItem(QString("HybrdChip Repository"));
    model->setItem(0, 0, rootItem);
    // QStandardItem *basicelsmentsitem = new QStandardItem(QString("Basic Elements"));
    // rootItem->appendRow(basicelsmentsitem);
    // QStandardItem *memoryelementsitem = new QStandardItem(QString("Memory Elements"));
    // basicelsmentsitem->appendRow(memoryelementsitem);
    // blockmemorygeneratoritem = new QStandardItem(QString("Block Memory Generator"));
    // QList<QStandardItem *> rowItems;
    // rowItems.append(blockmemorygeneratoritem);
    // rowItems.append(new QStandardItem(QString("AXI4")));
    // rowItems.append(new QStandardItem(QString("Production")));
    // rowItems.append(new QStandardItem(QString("Included")));
    // // rowItems.append(new QStandardItem(QString("xilinx.com:ip:blk_mem_gen:8.4")));
    // memoryelementsitem->appendRow(blockmemorygeneratoritem);
    // memoryelementsitem->appendRow(rowItems);
    // blockmemorygeneratoritem->setData("blockmemorygenerator", Qt::UserRole);

    QStandardItem *fpgaFeaturesAndDesignItem = new QStandardItem(QString("FPGA Features and Design"));
    rootItem->appendRow(fpgaFeaturesAndDesignItem);
    QStandardItem *clockingItem = new QStandardItem(QString("Clocking"));
    fpgaFeaturesAndDesignItem->appendRow(clockingItem);
    // QStandardItem *clockingWizardItem = new QStandardItem(QString("Clocking Wizard"));
    // clockingItem->appendRow(clockingItem);
    QStandardItem *clk_wiz = new QStandardItem(QString("Clocking Wizard"));
    QList<QStandardItem *> rowItems;
    rowItems.append(clk_wiz);
    rowItems.append(new QStandardItem(QString("AXI4")));
    rowItems.append(new QStandardItem(QString("Production")));
    rowItems.append(new QStandardItem(QString("Included")));
    rowItems.append(new QStandardItem(QString("hybrdchip.com:ip:clk_wiz:6.0")));
    clk_wiz->setData("clk_wiz", Qt::UserRole);
    clockingItem->appendRow(rowItems);

    connect(treeView, &QTreeView::doubleClicked, this, &IPManager::doubleClickedIP);

    connect(treeView, &QTreeView::clicked, this, &IPManager::clickedIP);

    treeView->expandAll();
}

void IPManager::doubleClickedIP(const QModelIndex &index)
{
    //取选中的这行的第一个元素的index
    const QModelIndex &idx = index.sibling(index.row(),0);
    if (!idx.isValid()) {
        return;
    }
    const QString ipName = idx.data(Qt::UserRole).toString();
    // qDebug() << idx;
    if (ipName == "blockmemorygenerator") {
        // BlockMemoryGenerator blockMemoryGenerator(this);
        // blockMemoryGenerator.exec();
    } else if (ipName == "clk_wiz"){
        ClockingWizard clk_wiz;
        if (clk_wiz.exec() == QDialog::Accepted) {
            // QString targetDir = QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("ip");
            // qDebug() << targetDir;

            FileManager::instance()->updateIPList();

        }
    }

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
        // BlockMemoryGenerator blockMemoryGenerator(this);
        // blockMemoryGenerator.exec();
    } else if (ipName == "clk_wiz"){
        detailLabel->setText("Name:         Clocking Wizard\n"
                             "Version:      6.0 (Rev. 3)\n"
                             "Interfaces:   AXI4\n"
                             "Status:       Production\n"
                             "License:      included");
    }
}
