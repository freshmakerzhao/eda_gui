#ifndef CLOCKINGWIZARD_H
#define CLOCKINGWIZARD_H


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
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QJsonObject>
#include "ipmanager/clk_wiz/ClockingOptionsWidget.h"
#include "ipmanager/clk_wiz/OutputClocksWidget.h"
#include "ipmanager/clk_wiz/PortRenamingWidget.h"
#include "ipmanager/clk_wiz/MMCMSettingsWidget.h"
#include "ipmanager/clk_wiz/SummaryWidget.h"
#include "base/Globals.h"


class ClockingWizard : public QDialog
{
    Q_OBJECT
public:
    ClockingWizard(QWidget *parent = nullptr);
protected:
    void accept() override;
private:
    QTabWidget *tabWidget;

    ClockingOptionsWidget *clockingOptionsWidget;

    OutputClocksWidget *outputClocksWidget;

    PortRenamingWidget *portRenamingWidget;

    MMCMSettingsWidget *mmcmSettingsWidget;

    SummaryWidget *summaryWidget;

    // QGraphicsView *viewModule;

    QAction *docAction;
    QAction *ipLocAction;
    QAction *switchToDefaultAction;
    
    QLineEdit *componentNameLineEdit;

    // int out_clk_nums = 0;

    AdvancedTableView *inputClockInformationTableView = new AdvancedTableView;

    QStandardItemModel *inputClockInformationModel = new QStandardItemModel;

    AdvancedTableView *outputClockInforTableView = new AdvancedTableView;

    QStandardItemModel *outputClockInforModel = new QStandardItemModel(8, 11);



    void setup_core_generation_info(QTableView *tableView, QStandardItemModel *model);

    void setup_input_clock_infor(QTableView *tableView, QStandardItemModel *model);

    void setup_output_clock_infor(QTableView *tableView, QStandardItemModel *model);

    void updateCoreGenerationInfoJson(QTableView *tableView, QStandardItemModel *model);

    void updateInputClockInforJson(QTableView *tableView, QStandardItemModel *model);

    void updateOutputClockInforJson(QTableView *tableView, QStandardItemModel *model);

    QString core_generation_info;

    QString input_clock_infor;

    QString output_clock_infor;
};

#endif // CLOCKINGWIZARD_H
