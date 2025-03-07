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
#include "service/HardWareManager.h"

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
    simItem(new QTreeWidgetItem(this, QStringList() << "SIMULATION")),
    synthItem(new QTreeWidgetItem(this, QStringList() << "SYNTHESIS")),
    impItem(new QTreeWidgetItem(this, QStringList() << "IMPLEMENTATION")),
    proItem(new QTreeWidgetItem(this, QStringList() << "PROGRAM AND DEBUG"))
{
    // proMgrItem->setDisabled(true);
    setColumnCount(1);
    setHeaderHidden(true);
    // setIndentation(20);
    setStyleSheet("QTreeWidget::item { height: 1.65em; }");
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
    // ================== 仿真 ==================
    simRunItem = new QTreeWidgetItem(simItem, QStringList() << "Run Simulation");
    simRunItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    // ================== 综合 ==================
    synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run Synthesis");
    synthRunItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    // run synth
    synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(":/icons/resource/icons/6-icon_report.png"));
    synthReportItem->setDisabled(true);
    // ================== imp ==================
    // pack_place_route
    impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(":/icons/resource/icons/1-icon_start_process.png"));
    reportTiming = new QTreeWidgetItem(impItem, QStringList() << "Report Timing");
    reportTiming->setIcon(0, QIcon(":/icons/resource/icons/8-icon_timing.png"));
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

    // ================================ Hardware Manager 分组 ==============================
    // 创建 Hardware Manager 分组
    groupHardwareManager = new QTreeWidgetItem(proItem, QStringList() << "Hardware Manager");
//    groupHardwareManager->setIcon(0, QIcon(":/icons/resource/icons/right2.png"));

    proAutoConnect = new QTreeWidgetItem(groupHardwareManager, QStringList() << "Auto Connect");
    proAutoConnect->setIcon(0, QIcon(":/icons/resource/0-icon_transparent.png"));
    proDownloadBitItem = new QTreeWidgetItem(groupHardwareManager, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(":/icons/resource/0-icon_transparent.png"));
    // 若需要显示子项指示器（下拉箭头），可显式设置
    groupHardwareManager->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//    groupHardwareManager->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
//    qDebug() << "Children count of groupHardwareManager:" << groupHardwareManager->childCount();
    // 默认状态为收起
//    connect(this, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem *item, int column) {
//        if (item == groupHardwareManager) {
//            bool isExpanded = item->isExpanded();
//            // 双击后切换展开状态
//            item->setExpanded(!isExpanded);
//
//            // 根据状态切换图标，例如：
//            // - 收起状态（!isExpanded为true时，表示原本展开状态现在要收起）
//            // - 展开状态
//            if (!isExpanded) {
//                // 原本是收起状态，双击后展开
//                // 使用一个展开状态的图标，例如open-folder.png
//                item->setIcon(0, QIcon(":/icons/resource/icons/down2.png"));
//            } else {
//                // 原本是展开状态，双击后收起
//                // 切换回收起状态图标，比如down2.png
//                item->setIcon(0, QIcon(":/icons/resource/icons/right2.png"));
//            }
//        }
//    });

    // ================================ Hardware Manager 分组 ==============================

    // downFlashItem = new QTreeWidgetItem(proItem, QStringList() << "Download Flash");
    // downFlashItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    // readBackRegItem = new QTreeWidgetItem(proItem, QStringList() << "Read Back Reg");
    // readBackRegItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    // readBackMemoryItem =  new QTreeWidgetItem(proItem, QStringList() << "Read Back Memory");
    // readBackMemoryItem->setIcon(0, QIcon(":/icons/resource/icons/0-icon_transparent.png"));
    expandAll();
    groupHardwareManager->setExpanded(false);
//
//    // 连接 itemExpanded 信号
//    QObject::connect(this, &QTreeWidget::itemExpanded, [&](QTreeWidgetItem* item) {
//        qDebug() << "Hardware Manager expanded";
//        if (item == groupHardwareManager) {
//            qDebug() << "123";
//        }
//    });
//
//    // 连接 itemCollapsed 信号
////    QObject::connect(this, &QTreeWidget::itemCollapsed, [&](QTreeWidgetItem* item) {
////        qDebug() << "Hardware Manager collapsed";
////        if (item == groupHardwareManager) {
////            qDebug() << "546";
////            // 在此处添加你需要执行的操作
////        }
////    });

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
        } else if (item == proAutoConnect) {
            // 自动连接，连接后记录连接状态
            HardWareManager::instance().autoConnect();
        }
        /* else if (item == downFlashItem) {
            TaskManager::instance().handleTreeItemActivation(15);
        } else if (item == readBackRegItem) {
            TaskManager::instance().handleTreeItemActivation(16);
        } else if (item == readBackMemoryItem) {
            TaskManager::instance().handleTreeItemActivation(17);
        }*/
          else if (item == simRunItem) {
            TaskManager::instance().handleTreeItemActivation(18);
        } else if (item == reportTiming) {
            TaskManager::instance().handleTreeItemActivation(19);
        }
        clearSelection(); // 清除选中状态
    });

}
