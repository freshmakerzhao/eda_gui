#ifndef CLOCKINGOPTIONSWIDGET_H
#define CLOCKINGOPTIONSWIDGET_H

#include "ipmanager/common/BasePage.h"
#include "ipmanager/common/AdvancedTableView.h"

class ClockingOptionsWidget : public BasePage
{
public:
    ClockingOptionsWidget(AdvancedTableView *inputClockInformationTableView, QStandardItemModel *inputClockInformationModel, QWidget *parent = nullptr);

private:
    // ----------------------- Clock Monitor ---------------------------
    QCheckBox *enableClockMonitoringCheckBox;
    // ------------------------- Primitive -----------------------------
    QRadioButton *mmcmRadioButton;
    QRadioButton *pllRadioButton;
    QButtonGroup *buttonGroup1;
    // --------------------- Clocking Features -------------------------
    QCheckBox *frequencySynthesisCheckBox;
    QCheckBox *minimizePowerCheckBox;
    QCheckBox *phaseAlignmentCheckBox;
    QCheckBox *spreadSpectrumCheckBox;
    QCheckBox *dynamicReconfigCheckBox;
    QCheckBox *dynamicPhaseShiftCheckBox;
    QCheckBox *safeClockStartupCheckBox;
    // --------------------- Jitter Optimization -----------------------
    QRadioButton *balancedRadioButton;
    QRadioButton *minimizeOutputJitterRadioButton;
    QRadioButton *maximizeInputJitterfilteringRadioButton;
    QButtonGroup *buttonGroup2;
    // ------------- Dynamic Reconfig Interface Options ----------------
    QRadioButton *axi4LiteRadioButton;
    QRadioButton *drpRadioButton;
    QCheckBox *phaseDutyCycleConfigCheckBox;
    QCheckBox *writeDRPregistersCheckBox;
    QButtonGroup *buttonGroup3;
    // ------------- Dynamic Reconfig Interface Options ----------------

    QStandardItemModel *model;
    // QTableView *tableView;
    AdvancedTableView *tableView;
};


#endif // CLOCKINGOPTIONSWIDGET_H
