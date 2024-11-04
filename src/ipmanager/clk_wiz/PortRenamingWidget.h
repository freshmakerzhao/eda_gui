#ifndef PORTRENAMINGWIDGET_H
#define PORTRENAMINGWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QScrollArea>

#include <QtWidgets>
// #include "common/AdvancedTableView.h"

class PortRenamingWidget : public QWidget
{
public:
    PortRenamingWidget(QWidget *parent = nullptr);

private:
    QLabel *vcoFrequencyValLabel;

    QTableView *tableView;

    QStandardItemModel *model;

    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;" // 不能删除，我也不知道为什么
                                  "}";
};

#endif // PORTRENAMINGWIDGET_H
