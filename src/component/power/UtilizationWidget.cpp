#include "UtilizationWidget.h"

UtilizationWidget::UtilizationWidget(const Utilization &u,
                                     QWidget *parent)
    : utilization(u),
    QWidget(parent),
    tableView(new QTableView),
    _model(new QStandardItemModel(this))
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    vBoxLayout->addWidget(tableView);

    tableView->setModel(_model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void UtilizationWidget::loadData(nlohmann::basic_json<nlohmann::ordered_map> j)
{
    auto utilization_details = j["utilization_details"];

    if (utilization == Utilization::Clock) {
        _model->setHorizontalHeaderLabels({"Name", "Frequency(MHZ)"});
        int row = 0;
        for (const auto& clock : utilization_details["clocks"]) {
            _model->setItem(row, 0, new QStandardItem(QString::fromStdString(clock["name"])));
            _model->setItem(row, 1, new QStandardItem(QString::number(clock["frequency"].get<double>(), 'f', 2)));
            row++;
        }
    }

    if (utilization == Utilization::Logic) {
        _model->setHorizontalHeaderLabels({"Name", "Type", "Utilization"});
        int row = 0;
        for (const auto& logic : utilization_details["logic"]) {
            _model->setItem(row, 0, new QStandardItem(QString::fromStdString(logic["name"])));
            _model->setItem(row, 1, new QStandardItem(QString::fromStdString(logic["type"])));
            _model->setItem(row, 2, new QStandardItem(QString::number(logic["utilization"].get<double>(), 'f', 2)));
            row++;
        }
    }
}

