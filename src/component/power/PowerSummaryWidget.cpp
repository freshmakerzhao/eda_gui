#include "PowerSummaryWidget.h"

PowerSummaryWidget::PowerSummaryWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("* { background-color: white; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    mainLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(mainWidget);
    hBoxLayout->setMargin(0);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setAlignment(Qt::AlignTop);
    QLabel *label = new QLabel("Power analysis from lmplemented netlist. "
                               "Activityderived from constraints files, "
                               "simulation files orvectorless analysis");
    label->setWordWrap(true);
    vBoxLayout->addWidget(label);

    QLabel* totalOnChipPowerLabel = new QLabel(QString("Total On-Chip Power:    %1W").arg(totalOnChipPower, 0, 'f', 3));
    QLabel* JunctionTemperatureLabel = new QLabel(QString("Junction Temperature:   %1°C").arg(JunctionTemperature, 0, 'f', 1));
    totalOnChipPowerLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    JunctionTemperatureLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    vBoxLayout->addWidget(totalOnChipPowerLabel);
    vBoxLayout->addWidget(JunctionTemperatureLabel);

    hBoxLayout->addLayout(vBoxLayout);

    powerChart = new PowerChart;
    hBoxLayout->addWidget(powerChart);
}

void PowerSummaryWidget::loadData(nlohmann::basic_json<nlohmann::ordered_map> j)
{
    try {
        auto summary = j["summary"];
        totalOnChipPower = summary["junction_temperature"].get<double>();
        JunctionTemperature = summary["total_on_chip_power"].get<double>();
    } catch (const std::exception& e) {
        qDebug() << "JSON Parsing Error:" << e.what();
    }

    powerChart->loadData(j);
}
