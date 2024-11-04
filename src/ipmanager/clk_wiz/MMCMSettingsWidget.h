#ifndef MMCMSETTINGSWIDGET_H
#define MMCMSETTINGSWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>
#include <QScrollArea>
#include "ipmanager/common/AdvancedTableView.h"

class MMCMSettingsWidget : public QWidget
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
