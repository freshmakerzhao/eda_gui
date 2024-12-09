/**
  ******************************************************************************
  * @file           : BlockMemoryGenerator.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */
#ifndef BLOCKMEMORYGENERATOR_H
#define BLOCKMEMORYGENERATOR_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QGraphicsView>
#include <QToolBar>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "ipmanager/blk_mem_gen/BasicWidget.h"
#include "ipmanager/blk_mem_gen/PortAOptionsWidget.h"
#include "ipmanager/blk_mem_gen/PortBOptionsWidget.h"
#include "ipmanager/blk_mem_gen/OtherOptionsWidget.h"
#include "ipmanager/blk_mem_gen/BlkMemGenSummary.h"
#include "ipmanager/blk_mem_gen/CoeFileEditor.h"
#include "PortAOptionsWidget.h"
#include "PortBOptionsWidget.h"
#include "OtherOptionsWidget.h"
#include "ipmanager/blk_mem_gen/BlkMemGenSummary.h"
// #include "IPlocationDialog.h"
#include "ipmanager/common/BaseDialog.h"

class BlockMemoryGenerator : public BaseDialog
{
    Q_OBJECT
public:
    BlockMemoryGenerator(QWidget *parent = nullptr);

    //! 响应MemoryType选择
    //! 更新Summary页面
    //! 更新Tab
    void updateMemoryType(const QString &option);
protected:
    void accept() override;
private:
    // QTabWidget *tabWidget;

    // QLineEdit *cmptNameLineEdit;
    void updateCoreGenerationInfoJson(QTableView *tableView);
    void setup_core_generation_info(QTableView *tableView);

    BasicWidget *basicWidget;

    PortAOptionsWidget *portAOptionsWidget;

    PortBOptionsWidget *portBOptionsWidget;

    OtherOptionsWidget *otherOptionsWidget;

    BlkMemGenSummary *summaryWidget;

    QJsonObject inputJsonRoot;

    QString	core_generation_info;

    // QGraphicsView *viewModule;

    // QAction *docAction;
    // QAction *ipLocAction;
    // QAction *swToDftAction;

private slots:
    void onipLocActionTrigger();
};

#endif // BLOCKMEMORYGENERATOR_H
