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
};

#endif // OUTPUTCLOCKSWIDGET_H
