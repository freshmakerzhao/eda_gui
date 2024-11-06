#ifndef OUTPUTCLOCKSWIDGET_H
#define OUTPUTCLOCKSWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QScrollBar>
#include <QScrollArea>
#include "ipmanager/common/BasePage.h"
#include "ipmanager/common/AdvancedTableView.h"

class OutputClocksWidget : public BasePage
{
public:
    OutputClocksWidget(AdvancedTableView *outputClockInforTableView,
                       QStandardItemModel *outputClockInforModel,
                       QWidget *parent = nullptr);

private:
    AdvancedTableView *tableView;

    QStandardItemModel *model;

    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;" // 不能删除，我也不知道为什么
                                  "}";
};

#endif // OUTPUTCLOCKSWIDGET_H
