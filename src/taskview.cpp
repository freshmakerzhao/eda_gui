#include "taskview.h"

#include <QTreeWidget>

TaskView::TaskView(QWidget *parent)
    : QWidget(parent)
{
    QTreeWidget *treeWidget = new QTreeWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(treeWidget);
    layout->setMargin(0);
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderHidden(true);
    QTreeWidgetItem* synthesisItem = new QTreeWidgetItem(treeWidget, QStringList() << "Synthesis");
    synthesisItem->setIcon(0, QIcon("")); // 在QIcon("")放置图标地址:/QIcon.ico

    QTreeWidgetItem* synthesisChild1 = new QTreeWidgetItem(synthesisItem, QStringList() << "view report");
    synthesisChild1->setIcon(0, QIcon(""));

    QTreeWidgetItem* synthesisChild2 = new QTreeWidgetItem(synthesisItem, QStringList() << "export blif");
    synthesisChild2->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationItem = new QTreeWidgetItem(treeWidget, QStringList() << "Implementation");
    implementationItem->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild1 = new QTreeWidgetItem(implementationItem, QStringList() << "view report");
    implementationChild1->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild2 = new QTreeWidgetItem(implementationItem, QStringList() << "only place");
    implementationChild2->setIcon(0, QIcon(""));

    QTreeWidgetItem* implementationChild3 = new QTreeWidgetItem(implementationItem, QStringList() << "only route");
    implementationChild3->setIcon(0, QIcon(""));

    QTreeWidgetItem* timingAnalysisItem = new QTreeWidgetItem(treeWidget, QStringList() << "Timing Analysis");
    timingAnalysisItem->setIcon(0, QIcon(""));

    QTreeWidgetItem* editSettingsItem = new QTreeWidgetItem(treeWidget, QStringList() << "Edit Settings");
    editSettingsItem->setIcon(0, QIcon(""));

    treeWidget->expandAll();

}

TaskView::~TaskView()
{

}
