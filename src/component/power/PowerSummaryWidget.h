#ifndef POWERSUMMARYWIDGET_H
#define POWERSUMMARYWIDGET_H

#include <QWidget>

#include "utils/json.hpp"
#include <iostream>
#include "PowerChart.h"

class PowerSummaryWidget : public QWidget
{
    Q_OBJECT
public:
    PowerSummaryWidget(QWidget *parent = nullptr);

    void loadData(nlohmann::basic_json<nlohmann::ordered_map> j);

    PowerChart *getPowerChartPtr() {
        return powerChart;
    }

private:
    double totalOnChipPower;

    double JunctionTemperature;

    const QString TITLE_LABEL_STYLESHEET = "QLabel{"
                                           "font-size: 20px;"
                                           "font-weight: bold;"
                                           "border-radius: 0px;"
                                           "}";

    PowerChart *powerChart;
};

#endif // POWERSUMMARYWIDGET_H
