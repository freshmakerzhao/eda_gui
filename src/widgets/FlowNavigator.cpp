/**
  ******************************************************************************
  * @file           : FlowNavigator.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/2
  ******************************************************************************
  */

#include "FlowNavigator.h"
#include "utils/TaskManager.h"

FlowNavigator::FlowNavigator(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(1);
    setHeaderHidden(true);
    // setIndentation(20);
    setStyleSheet("QTreeWidget::item { height: 45px; }");
    // ============ PROJECT MANAGER ============
    // QTreeWidgetItem *proMgrItem = new QTreeWidgetItem(this, QStringList() << "PROJECT MANAGER");
    // proMgrItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    // // Settings
    // QTreeWidgetItem *settingsItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Settings");
    // settingsItem->setIcon(0, QIcon(""));
    // // Add Sources
    // QTreeWidgetItem *addSourcesItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Add Sources");
    // addSourcesItem->setIcon(0, QIcon(""));
    // // IP Catalog
    // QTreeWidgetItem *ipCatalogItem = new QTreeWidgetItem(proMgrItem, QStringList() << "IP Catalog");
    // ipCatalogItem->setIcon(0, QIcon(""));
    // ================== 综合 ==================
    QTreeWidgetItem *synthItem = new QTreeWidgetItem(this, QStringList() << "SYNTHESIS");
    synthItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    QTreeWidgetItem *synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run");
    synthRunItem->setIcon(0, QIcon(""));
    // run synth
    QTreeWidgetItem *synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(""));
    synthReportItem->setDisabled(true);
    // ================== imp ==================
    QTreeWidgetItem *impItem = new QTreeWidgetItem(this, QStringList() << "IMPLEMENTATION");
    impItem->setIcon(0, QIcon(""));
    // pack_place_route
    QTreeWidgetItem *impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(""));
    // 仅pack
    QTreeWidgetItem *impPackItem = new QTreeWidgetItem(impItem, QStringList() << "Only pack");
    impPackItem->setIcon(0, QIcon(""));
    // 仅place
    QTreeWidgetItem *impPlaceItem = new QTreeWidgetItem(impItem, QStringList() << "Only place");
    impPlaceItem->setIcon(0, QIcon(""));
    // 仅route
    QTreeWidgetItem *impRouteItem = new QTreeWidgetItem(impItem, QStringList() << "Only route");
    impRouteItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *impPackReportItem = new QTreeWidgetItem(impItem, QStringList() << "Report");
    // 查看 Pack 日志
    impPackReportItem->setIcon(0, QIcon(""));
    impPackReportItem->setDisabled(true);
    // ================== 码流及可视化 ==================
    QTreeWidgetItem *proItem = new QTreeWidgetItem(this, QStringList() << "PROGRAM AND DEBUG");
    proItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proNetlistViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate NetlistView");
    proNetlistViewItem->setDisabled(true);
    proNetlistViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitItem = new QTreeWidgetItem(proItem, QStringList() << "Generate Bitstream");
    proBitItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate GridView");
    proBitViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proDownloadBitItem = new QTreeWidgetItem(proItem, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(""));
    expandAll();

    QObject::connect(this, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column) {
        TaskManager::instance().handleTreeItemActivation(item);
        clearSelection(); // 清除选中状态
    });

}
