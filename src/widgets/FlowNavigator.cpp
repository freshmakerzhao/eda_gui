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
    QTreeWidgetItem *proMgrItem = new QTreeWidgetItem(this, QStringList() << "PROJECT MANAGER");
    proMgrItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    // Settings
    QTreeWidgetItem *settingsItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Settings");
    settingsItem->setIcon(0, QIcon(":/icons/resource/icons/4-icon_setting.png"));
    // Add Sources
    QTreeWidgetItem *addSourcesItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Add Sources");
    addSourcesItem->setIcon(0, QIcon(":/icons/resource/icons/12-1icon_add.png"));
    // IP Catalog
    QTreeWidgetItem *ipCatalogItem = new QTreeWidgetItem(proMgrItem, QStringList() << "IP Catalog");
    ipCatalogItem->setIcon(0, QIcon(":/icons/resource/icons/5-icon_ip_catalog.png"));
    // ================== 综合 ==================
    QTreeWidgetItem *synthItem = new QTreeWidgetItem(this, QStringList() << "SYNTHESIS");
    synthItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    QTreeWidgetItem *synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run Synthesis");
    synthRunItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    // run synth
    QTreeWidgetItem *synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(":/icons/resource/icons/6-icon_report.png"));
    synthReportItem->setDisabled(true);
    // ================== imp ==================
    QTreeWidgetItem *impItem = new QTreeWidgetItem(this, QStringList() << "IMPLEMENTATION");
    impItem->setIcon(0, QIcon(""));
    // pack_place_route
    QTreeWidgetItem *impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    // 仅pack
//    QTreeWidgetItem *impPackItem = new QTreeWidgetItem(impItem, QStringList() << "Only pack");
//    impPackItem->setIcon(0, QIcon(""));
    // 仅place
//    QTreeWidgetItem *impPlaceItem = new QTreeWidgetItem(impItem, QStringList() << "Only place");
//    impPlaceItem->setIcon(0, QIcon(""));
    // 仅route
//    QTreeWidgetItem *impRouteItem = new QTreeWidgetItem(impItem, QStringList() << "Only route");
//    impRouteItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *impPackReportItem = new QTreeWidgetItem(impItem, QStringList() << "Report");
    // 查看 Pack 日志
    impPackReportItem->setIcon(0, QIcon(":/icons/resource/icons/6-icon_report.png"));
    impPackReportItem->setDisabled(true);
    // ================== 码流及可视化 ==================
    QTreeWidgetItem *proItem = new QTreeWidgetItem(this, QStringList() << "PROGRAM AND DEBUG");
    proItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proNetlistViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate NetlistView");
    proNetlistViewItem->setDisabled(true);
    proNetlistViewItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    QTreeWidgetItem *proBitItem = new QTreeWidgetItem(proItem, QStringList() << "Generate Bitstream");
    proBitItem->setIcon(0, QIcon(":/icons/resource/icons/3-icon_generate_bitstream.png"));
    QTreeWidgetItem *proBitViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate GridView");
    proBitViewItem->setIcon(0, QIcon(":/icons/resource/icons/28-icon_grid_view.png"));
    QTreeWidgetItem *proDownloadBitItem = new QTreeWidgetItem(proItem, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    expandAll();

    QObject::connect(this, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column) {
        if (item == synthRunItem) {
            TaskManager::instance().handleTreeItemActivation(0);
        } else if (item == synthReportItem){
            TaskManager::instance().handleTreeItemActivation(1);
        } else if (item == impAllItem) {
            TaskManager::instance().handleTreeItemActivation(2);
//        } else if (item == impPackItem) {
//            TaskManager::instance().handleTreeItemActivation(3);
//        } else if (item == impPlaceItem) {
//            TaskManager::instance().handleTreeItemActivation(4);
//        } else if (item == impRouteItem) {
//            TaskManager::instance().handleTreeItemActivation(5);
        } else if (item == impPackReportItem) {
            TaskManager::instance().handleTreeItemActivation(6);
        } else if (item == proNetlistViewItem) {
            TaskManager::instance().handleTreeItemActivation(7);
        } else if (item == proBitItem) {
            TaskManager::instance().handleTreeItemActivation(8);
        } else if (item == proBitViewItem) {
            TaskManager::instance().handleTreeItemActivation(9);
        } else if (item == proDownloadBitItem) {
            TaskManager::instance().handleTreeItemActivation(10);
        } else if (item == addSourcesItem) {
            TaskManager::instance().handleTreeItemActivation(11);
        } else if (item == ipCatalogItem) {
            TaskManager::instance().handleTreeItemActivation(12);
        } else if (item == settingsItem) {
            TaskManager::instance().handleTreeItemActivation(13);
        }
        clearSelection(); // 清除选中状态
    });

}
