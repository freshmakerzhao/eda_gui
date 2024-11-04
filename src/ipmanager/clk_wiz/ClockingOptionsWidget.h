#ifndef CLOCKINGOPTIONSWIDGET_H
#define CLOCKINGOPTIONSWIDGET_H

#include <QDebug>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QtWidgets>
#include "ipmanager/common/AdvancedTableView.h"


class ClockingOptionsWidget : public QWidget
{
public:
    ClockingOptionsWidget(AdvancedTableView *inputClockInformationTableView, QStandardItemModel *inputClockInformationModel, QWidget *parent = nullptr);

    // QString getInputClockInfor();

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

    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;" // 不能删除，我也不知道为什么
                                  "}";
};


#endif // CLOCKINGOPTIONSWIDGET_H
