#include "PowerChart.h"
#include <iostream>

PowerChart::PowerChart(QWidget *parent)
    : QWidget(parent),
    customPlot(new QCustomPlot),
    customPlot2(new QCustomPlot)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hBoxLayout->addWidget(customPlot);
    hBoxLayout->addWidget(customPlot2);

    customPlot->setFixedSize(100, 400);
    customPlot2->setFixedSize(250, 250);
}

void PowerChart::loadData(nlohmann::basic_json<nlohmann::ordered_map> j)
{
    try {
        auto summary = j["summary"];
        total_on_chip_power = summary["total_on_chip_power"].get<double>();
        auto on_chip_power = summary["on_chip_power"];
        static_power = on_chip_power["static_power"].get<double>();
        auto dynamic_power = on_chip_power["dynamic_power"];
        std::cout << "Dynamic Power - BRAM: " << dynamic_power["BRAM"] << std::endl;
        std::cout << "Dynamic Power - DSP: " << dynamic_power["DSP"] << std::endl;
        std::cout << "Dynamic Power - IO: " << dynamic_power["IO"] << std::endl;
        std::cout << "Dynamic Power - MMCM: " << dynamic_power["MMCM"] << std::endl;
        std::cout << "Dynamic Power - Clocks: " << dynamic_power["clocks"] << std::endl;
        std::cout << "Dynamic Power - Logic: " << dynamic_power["logic"] << std::endl;
        std::cout << "Dynamic Power - Signals: " << dynamic_power["signals"] << std::endl;

        dynamic_power["BRAM"].get<double>();
        dynamic_power["DSP"].get<double>();

        dynamic_power_io = dynamic_power["IO"].get<double>();
        dynamic_power_mmcm = dynamic_power["MMCM"].get<double>();
        dynamic_power_clocks = dynamic_power["clocks"].get<double>();
        dynamic_power_logic = dynamic_power["logic"].get<double>();
        dynamic_power_signals = dynamic_power["signals"].get<double>();
        createPercentageBarChart(customPlot);
        createDynamicPowerChart(customPlot2);
    } catch (const std::exception& e) {
        qDebug() << "JSON Parsing Error:" << e.what();
    }
}

void PowerChart::createPercentageBarChart(QCustomPlot *customPlot) {
    // customPlot->legend->setVisible(true);
    // customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom | Qt::AlignRight);
    // 设置数据
    double dynamic_power = total_on_chip_power - static_power;
    // double total = total_dynamic_power + static_power;
    QVector<double> values = {dynamic_power / total_on_chip_power * 100, static_power / total_on_chip_power * 100};
    QVector<QColor> colors = {QColor(255, 255, 153), QColor(153, 204, 255), Qt::green};
    QVector<QString> labels = {QString::number(values[0]) + "%", QString::number(values[1]) + "%"};

    // // 创建矩形
    // double start = 0.0;
    // for (int i = 0; i < values.size(); ++i) {
    //     QCPGraph *graph = new QCPGraph(customPlot->yAxis, customPlot->xAxis);
    //     graph->setData(QVector<double>{start, start + values[i] / 100.0}, QVector<double>{1, 1});
    //     graph->setBrush(colors[i]);
    //     // graph->setPen(Qt::NoPen);
    //     start += values[i] / 100.0;
    // }

    // 创建矩形
    double start = 0.0;
    for (int i = 0; i < values.size(); ++i) {
        QCPItemRect *rect = new QCPItemRect(customPlot);
        rect->topLeft->setCoords(1.0f - 0.015f, start);
        rect->bottomRight->setCoords(0, start + values[i] / 100.0);
        rect->setBrush(colors[i]);
        QPen pen(QColor(128, 128, 128), 2.0f);
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        rect->setPen(pen); // 设置边框颜色和宽度
        start += values[i] / 100.0;
    }

    // 添加文本标签
    start = 0.0;
    for (int i = 0; i < values.size(); ++i) {
        QCPItemText *textLabel = new QCPItemText(customPlot);
        textLabel->setText(labels[i]);
        textLabel->position->setCoords(0.5, start + values[i] / 200.0);
        textLabel->setFont(QFont("Arial", 6));
        textLabel->setColor(Qt::black);
        start += values[i] / 100.0;
    }

    // 设置坐标轴
    customPlot->xAxis->setRange(0, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->xAxis->setVisible(false);
    customPlot->yAxis->setVisible(false);

    // 绘制图表
    customPlot->replot();


}

void PowerChart::createDynamicPowerChart(QCustomPlot *customPlot) {
    // customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->setInteraction(QCP::iSelectPlottables, true);
    customPlot->legend->setVisible(true);
    customPlot->legend->setBorderPen(Qt::NoPen);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom | Qt::AlignRight);

    total_dynamic_power = (dynamic_power_clocks + dynamic_power_signals + dynamic_power_logic + dynamic_power_mmcm + dynamic_power_io) / 100;
    if (total_dynamic_power == 0) {
        return;
    }

    QVector<double> values = {dynamic_power_clocks / total_dynamic_power, dynamic_power_signals / total_dynamic_power, dynamic_power_logic / total_dynamic_power, dynamic_power_mmcm / total_dynamic_power, dynamic_power_io / total_dynamic_power};
    QVector<QColor> colors = {QColor(255, 153, 255), QColor(153, 255, 255), QColor(138, 240, 107), QColor(240, 51, 189), QColor(204, 255, 153)};
    QVector<QString> labels = {"Clock", "Signals", "Logic", "MMCM", "IO"};
    QVector<QString> names = {"Clock", "Signals", "Logic", "MMCM", "IO"};
    QVector<QCPBars*> bars;

    QVector<QCPItemRect*> rectItems;

    QMap<QString, Legend> legend_map = {
        {"Clock", Legend::Clocks},
        {"Signals", Legend::Signals},
        {"Logic", Legend::Logic},
        {"MMCM", Legend::MMCM},
        {"IO", Legend::IO}
    };

    // // 创建矩形
    // double start = 0.0;
    // for (int i = 0; i < values.size(); ++i) {
    //     QCPGraph *graph = new QCPGraph(customPlot->yAxis, customPlot->xAxis);
    //     graph->setData(QVector<double>{start, start + values[i] / 100.0}, QVector<double>{1, 1});
    //     graph->setBrush(colors[i]);
    //     // graph->setPen(Qt::NoPen);
    //     start += values[i] / 100.0;
    // }

    // 创建矩形
    double start = 0.01f;
    for (int i = 0; i < values.size(); ++i) {
        QCPItemRect *rect = new QCPItemRect(customPlot);
        rect->setProperty("label", labels[i]);

        double width = qMax(values[i] / 100.0, minHeight);
        rect->topLeft->setCoords(0.005f + offset, start);
        rect->bottomRight->setCoords(1.0f - offset, start + width - 0.005);
        // rect->topLeft->setCoords(offset, start + width);
        // rect->bottomRight->setCoords(1.0f - offset, start);

        rect->setBrush(colors[i]);
        QPen pen(QColor(128, 128, 128), 2.0f);
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        rect->setPen(pen); // 设置边框颜色和宽度
        // start += values[i] / 100.0;
        start += width;
        start += 0.005f;

        // 创建图例条目
        QCPBars *bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        bars.append(bar);
        bar->setName(names[i]); // 设置图例名称
        bar->setBrush(colors[i]); // 设置填充颜色
        bar->setData(QVector<double>({0}), QVector<double>({0})); // 数据
        rectItems.append(rect);
    }

    // 添加文本标签
    // start = 0.0;
    // for (int i = 0; i < values.size(); ++i) {
    //     QCPItemText *textLabel = new QCPItemText(customPlot);
    //     textLabel->setText(QString::number(values[i] * 100));
    //     textLabel->position->setCoords(0.5, start + values[i] / 200.0);
    //     textLabel->setFont(QFont("Arial", 6));
    //     textLabel->setColor(Qt::black);
    //     start += values[i] / 100.0;
    // }

    // 设置坐标轴
    customPlot->xAxis->setRange(0, 3);
    // customPlot->yAxis->setRange(0, 1);
    customPlot->yAxis->setRange(0, start);
    customPlot->xAxis->setVisible(false);
    customPlot->yAxis->setVisible(false);

    // 绘制图表
    customPlot->replot();

    QObject::connect(
        customPlot,
        &QCustomPlot::legendClick,
        [=](QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event) {
            QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
            if (plItem) {
                QString name = plItem->plottable()->name();
                if (legend_map.contains(name))
                    emit dynamicPowerLegendClick(legend_map[name]);
            }   
        }
    );

    customPlot->setMouseTracking(true); // 启用鼠标跟踪
    customPlot->setFocusPolicy(Qt::StrongFocus); // 设置聚焦策略

    QObject::connect(customPlot, &QCustomPlot::mousePress, [=](QMouseEvent *event) {
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        for (auto *rect : rectItems) {
            double x1 = rect->topLeft->coords().x();
            double y1 = rect->topLeft->coords().y();
            double x2 = rect->bottomRight->coords().x();
            double y2 = rect->bottomRight->coords().y();
            if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                QString label = rect->property("label").toString();
                if (legend_map.contains(label)) {
                    emit dynamicPowerLegendClick(legend_map[label]);
                    break;
                }
            }
        }
    });

    QObject::connect(customPlot, &QCustomPlot::mouseMove, [=](QMouseEvent *event) {
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        bool needReplot = false;
        for (QCPAbstractItem *item : rectItems) {
            if (QCPItemRect *rect = qobject_cast<QCPItemRect*>(item)) {
                double x1 = rect->topLeft->coords().x();
                double y1 = rect->topLeft->coords().y();
                double x2 = rect->bottomRight->coords().x();
                double y2 = rect->bottomRight->coords().y();
                // qDebug() << x << " " << y;
                // qDebug() << x1 << "," << y1 << " " << x2 << "," << y2;
                QPen pen = rect->pen();
                if (x >= x1 && x <= x2 && y <= y2 && y >= y1) {
                    if (pen.color() != Qt::blue) {
                        pen.setColor(Qt::blue);
                        pen.setWidthF(3.0f);
                        rect->setPen(pen);
                        needReplot = true;
                    }
                    QToolTip::showText(event->globalPos(), QString(rect->property("label").toString()));
                } else {
                    if (pen.color() != Qt::gray) {
                        pen.setColor(Qt::gray);
                        pen.setWidthF(2.0f);
                        rect->setPen(pen);
                        needReplot = true;
                    }
                }
                if (needReplot) customPlot->replot();
            }
        }
    });

    // QObject::connect(customPlot, &QCustomPlot::plottableClick, [=](QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
    //     qDebug() << "Clicked on plottable:" << plottable->name() << "at data index:" << dataIndex;
    // });

    // QObject::connect(customPlot, &QCustomPlot::itemClick, [=](QCPAbstractItem *item, QMouseEvent *event) {
    //     qDebug() << item->parentPlot()->plottable()->name();
    // });
}
