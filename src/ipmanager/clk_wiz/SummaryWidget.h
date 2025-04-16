#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>

class SummaryWidget : public QWidget
{
    Q_OBJECT
public:
    SummaryWidget(QWidget *parent = nullptr);

private:
    QTableView *primaryInputClockAttributesTableView;

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

#endif // SUMMARYWIDGET_H
