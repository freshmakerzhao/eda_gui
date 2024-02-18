#include "taskview.h"
#include "ui_taskview.h"

#include <QTreeWidget>

TaskView::TaskView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskView)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderHidden(true);
    QTreeWidgetItem* synthesisItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << "Synthesis");
    synthesisItem->setIcon(0, QIcon("")); // 在QIcon("")放置图标地址:/QIcon.ico

    QTreeWidgetItem* synthesisChild1 = new QTreeWidgetItem(synthesisItem, QStringList() << "view report");
    synthesisChild1->setIcon(0, QIcon(""));

    QTreeWidgetItem* synthesisChild2 = new QTreeWidgetItem(synthesisItem, QStringList() << "export blif");
    synthesisChild2->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << "Implementation");
    implementationItem->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild1 = new QTreeWidgetItem(implementationItem, QStringList() << "view report");
    implementationChild1->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild2 = new QTreeWidgetItem(implementationItem, QStringList() << "only place");
    implementationChild2->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild3 = new QTreeWidgetItem(implementationItem, QStringList() << "only route");
    implementationChild3->setIcon(0, QIcon(""));

    QTreeWidgetItem* timingAnalysisItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << "Timing Analysis");
    timingAnalysisItem->setIcon(0, QIcon(""));

    QTreeWidgetItem* editSettingsItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << "Edit Settings");
    editSettingsItem->setIcon(0, QIcon(""));

    ui->treeWidget->expandAll();

}

TaskView::~TaskView()
{
    delete ui;
}
