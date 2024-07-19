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

FlowNavigator *FlowNavigator::instance()
{
    static FlowNavigator *_instance = nullptr;
    if (!_instance) {
        _instance = new FlowNavigator(nullptr);
    }
    return _instance;
}

void FlowNavigator::downloadBitMode(const bool &f)
{
    proMgrItem->setDisabled(f);
    synthItem->setDisabled(f);
    impItem->setDisabled(f);

    settingsItem->setDisabled(f);
    addSourcesItem->setDisabled(f);
    ipCatalogItem->setDisabled(f);
    prjSummaryItem->setDisabled(f);

    synthRunItem->setDisabled(f);
    synthReportItem->setDisabled(f);
    impAllItem->setDisabled(f);
    impPackReportItem->setDisabled(f);

    proNetlistViewItem->setDisabled(f);
    proBitItem->setDisabled(f);
    proBitViewItem->setDisabled(f);
    proDownloadBitItem->setDisabled(f);
}

FlowNavigator::FlowNavigator(QWidget *parent)
    : QTreeWidget(parent),
    proMgrItem(new QTreeWidgetItem(this, QStringList() << "PROJECT MANAGER")),
    synthItem(new QTreeWidgetItem(this, QStringList() << "SYNTHESIS")),
    impItem(new QTreeWidgetItem(this, QStringList() << "IMPLEMENTATION")),
    proItem(new QTreeWidgetItem(this, QStringList() << "PROGRAM AND DEBUG"))
{
    // proMgrItem->setDisabled(true);
    setColumnCount(1);
    setHeaderHidden(true);
    // setIndentation(20);
    setStyleSheet("QTreeWidget::item { height: 45px; }");
    // ============ PROJECT MANAGER ============
    // Settings
    settingsItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Settings");
    settingsItem->setIcon(0, QIcon(":/icons/resource/icons/4-icon_setting.png"));
    // Add Sources
    addSourcesItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Add Sources");
    addSourcesItem->setIcon(0, QIcon(":/icons/resource/icons/12-1icon_add.png"));
    // IP Catalog
    ipCatalogItem = new QTreeWidgetItem(proMgrItem, QStringList() << "IP Catalog");
    ipCatalogItem->setIcon(0, QIcon(":/icons/resource/icons/5-icon_ip_catalog.png"));
    // Project Summary
    prjSummaryItem = new QTreeWidgetItem(proMgrItem, QStringList() << "Project Summary");
    prjSummaryItem->setIcon(0, QIcon(":/icons/resource/icons/20-icon_summary_2.png"));
    // ================== 综合 ==================
    synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run Synthesis");
    synthRunItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    // run synth
    QTreeWidgetItem *synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(":/icons/resource/icons/6-icon_report.png"));
    synthReportItem->setDisabled(true);
    // ================== imp ==================
    // pack_place_route
    impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    impPackReportItem = new QTreeWidgetItem(impItem, QStringList() << "Report");
    // 查看 Pack 日志
    impPackReportItem->setIcon(0, QIcon(":/icons/resource/icons/6-icon_report.png"));
    impPackReportItem->setDisabled(true);
    // ================== 码流及可视化 ==================
    proNetlistViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate NetlistView");
    proNetlistViewItem->setDisabled(true);
    proNetlistViewItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    proBitItem = new QTreeWidgetItem(proItem, QStringList() << "Generate Bitstream");
    proBitItem->setIcon(0, QIcon(":/icons/resource/icons/3-icon_generate_bitstream.png"));
    proBitViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate GridView");
    proBitViewItem->setIcon(0, QIcon(":/icons/resource/icons/28-icon_grid_view.png"));
    proDownloadBitItem = new QTreeWidgetItem(proItem, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    expandAll();

    QObject::connect(this, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column) {
        if (item == synthRunItem) {
            TaskManager::instance().handleTreeItemActivation(0);
        } else if (item == synthReportItem){
            TaskManager::instance().handleTreeItemActivation(1);
        } else if (item == impAllItem) {
            TaskManager::instance().handleTreeItemActivation(2);
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
        } else if (item == prjSummaryItem) {
            TaskManager::instance().handleTreeItemActivation(14);
        }
        clearSelection(); // 清除选中状态
    });

}
