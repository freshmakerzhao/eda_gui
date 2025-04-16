#ifndef UTILIZATIONWIDGET_H
#define UTILIZATIONWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include "utils/json.hpp"

enum class Utilization
{
    Clock,
    Logic
};

class UtilizationWidget : public QWidget
{
    Q_OBJECT
public:
    UtilizationWidget(const Utilization &u,
                      QWidget *parent = nullptr);

    void loadData(nlohmann::basic_json<nlohmann::ordered_map> j);

private:
    QTableView *tableView;

    QStandardItemModel* _model;

    Utilization utilization;
};

#endif // UTILIZATIONWIDGET_H
