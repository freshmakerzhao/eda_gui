#ifndef PORTRENAMINGWIDGET_H
#define PORTRENAMINGWIDGET_H

#include "ipmanager/common/BasePage.h"
#include "ipmanager/common/AdvancedTableView.h"

class PortRenamingWidget : public BasePage
{
public:
    PortRenamingWidget(QWidget *parent = nullptr);

private:
    QLabel *vcoFrequencyValLabel;

    AdvancedTableView *tableView;

    QStandardItemModel *model;
};

#endif // PORTRENAMINGWIDGET_H
