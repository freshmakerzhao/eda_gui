#ifndef CLKWIZSUMMARY_H
#define CLKWIZSUMMARY_H

#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include "ipmanager/common/BasePage.h"
#include "ipmanager/common/AdvancedTableView.h"

class ClkWizSummary : public BasePage
{
    Q_OBJECT
public:
    ClkWizSummary(QWidget *parent = nullptr);

private:
    AdvancedTableView *primaryInputClockAttributesTableView;

    QStandardItemModel *primaryInputClockAttributesModel;

    QLabel *primitiveInstantiatedLabel;

    QLabel *divideCounterLabel;

    QLabel *multCounterLabel;

    QLabel *clockPhaseShiftLabel;

    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;" // 不能删除，我也不知道为什么
                                  "}";

};

#endif // CLKWIZSUMMARY_H
