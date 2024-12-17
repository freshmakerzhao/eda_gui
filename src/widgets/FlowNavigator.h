/**
  ******************************************************************************
  * @file           : FlowNavigator.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/2
  ******************************************************************************
  */

#ifndef FLOWNAVIGATOR_H
#define FLOWNAVIGATOR_H

#include <QTreeWidget>

class FlowNavigator : public QTreeWidget
{
    Q_OBJECT
public:
    static FlowNavigator *instance();

    // enum Task {
    //     Settings,
    //     AddSources,
    //     IPCatalog,
    //     ProjectSummary,
    // };
    void downloadBitMode(const bool &f = false);

private:
    FlowNavigator(QWidget *parent = nullptr);

    QTreeWidgetItem *proMgrItem;
    QTreeWidgetItem *simItem;
    QTreeWidgetItem *synthItem;
    QTreeWidgetItem *impItem;
    QTreeWidgetItem *proItem;

    // ============ PROJECT MANAGER ============
    //! Settings
    QTreeWidgetItem *settingsItem;
    //! Add Sources
    QTreeWidgetItem *addSourcesItem;
    //! IP Catalog
    QTreeWidgetItem *ipCatalogItem;
    //! Project Summary
    QTreeWidgetItem *prjSummaryItem;

    // ============== SIMULATION ===============
    //! run simulation
    QTreeWidgetItem *simRunItem;

    // ============== SYNTHESIS ================
    //! run synth
    QTreeWidgetItem *synthRunItem;
    //! synth report
    QTreeWidgetItem *synthReportItem;

    // =========== Implementation ==============
    //! pack_place_route
    QTreeWidgetItem *impAllItem;
    //! impl report
    QTreeWidgetItem *impPackReportItem;
    // ========== PROGRAM AND DEBUG ============
    //! netlist
    QTreeWidgetItem *proNetlistViewItem;
    //! bitstream
    QTreeWidgetItem *proBitItem;
    //! gridview
    QTreeWidgetItem *proBitViewItem;
    // ========== PROGRAM AND DEBUG HARDWARE MANAGER ============
    //! HARDWARE MANAGER
    QTreeWidgetItem *groupHardwareManager;
    //! auto connect
    QTreeWidgetItem *proAutoConnect;
    //! download bit
    QTreeWidgetItem *proDownloadBitItem;

    // ========== PROGRAM AND DEBUG HARDWARE MANAGER ============

    // QTreeWidgetItem *downFlashItem;

    // QTreeWidgetItem *readBackRegItem;

    // QTreeWidgetItem *readBackMemoryItem;

signals:
};

#endif // FLOWNAVIGATOR_H
