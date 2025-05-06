#ifndef POWERCHART_H
#define POWERCHART_H

#include <QWidget>
#include <QCustomPlot.h>
#include <QMouseEvent>

#include "utils/json.hpp"

enum class Legend
{
    Clocks,
    Signals,
    Logic,
    MMCM,
    IO
};

class PowerChart : public QWidget
{
    Q_OBJECT
public:
    PowerChart(QWidget *parent = nullptr);

    void loadData(nlohmann::basic_json<nlohmann::ordered_map> j);

signals:
    void dynamicPowerLegendClick(const Legend &legend);

private:

    void createPercentageBarChart(QCustomPlot *customPlot);

    void createDynamicPowerChart(QCustomPlot *customPlot);

    QCustomPlot *customPlot;
    QCustomPlot *customPlot2;

    const double minHeight = 0.06;
    const double offset = 0.015f;

    double total_on_chip_power;
    double static_power;
    double total_dynamic_power;

    double dynamic_power_io;
    double dynamic_power_mmcm;
    double dynamic_power_clocks;
    double dynamic_power_logic;
    double dynamic_power_signals;
};

#endif // POWERCHART_H
