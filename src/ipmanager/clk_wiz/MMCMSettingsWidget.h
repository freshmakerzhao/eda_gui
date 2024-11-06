#ifndef MMCMSETTINGSWIDGET_H
#define MMCMSETTINGSWIDGET_H

#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>
#include <QScrollArea>
#include "ipmanager/common/BasePage.h"
#include "ipmanager/common/AdvancedTableView.h"

class MMCMSettingsWidget : public BasePage
{
    Q_OBJECT
public:
    MMCMSettingsWidget(QWidget *parent = nullptr);

private:
    QCheckBox *allowOverrideModeCheckBox;

    QStandardItemModel *attributeModel;

    AdvancedTableView *attributeTableView;

    QStandardItemModel *portModel;

    AdvancedTableView *portTableView;
};

#endif // MMCMSETTINGSWIDGET_H
