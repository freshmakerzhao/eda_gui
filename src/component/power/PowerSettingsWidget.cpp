#include "PowerSettingsWidget.h"

PowerSettingsWidget::PowerSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setMargin(0);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setAlignment(Qt::AlignTop);

    QLabel *deviceLabel = new QLabel("Device");
    deviceLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    vBoxLayout->addWidget(deviceLabel);
    vBoxLayout->addWidget(new QLabel("  Part:             xc7a100tfgg484-2"));
    vBoxLayout->addWidget(new QLabel("  Temp grade:       commercial"));
    vBoxLayout->addWidget(new QLabel("  Process:          typical"));
    vBoxLayout->addWidget(new QLabel("  Characterization: Production"));

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout;
    vBoxLayout1->setAlignment(Qt::AlignTop);

    QLabel *environmentLabel = new QLabel("Environment");
    environmentLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    vBoxLayout1->addWidget(environmentLabel);
    vBoxLayout1->addWidget(new QLabel("  Output Load:             0 pF"));
    vBoxLayout1->addWidget(new QLabel("  Ambient temperature:     25.0°C"));
    vBoxLayout1->addWidget(new QLabel("  Airflow:                 250 LFM"));
    vBoxLayout1->addWidget(new QLabel("  Heat sink:               medium (Medium Profile)"));
    vBoxLayout1->addWidget(new QLabel("  θSA:                     4.6 °C/W"));
    vBoxLayout1->addWidget(new QLabel("  Board selection:         medium (10\"x10\")"));
    vBoxLayout1->addWidget(new QLabel("  Number of board layers:  2to15 (12 to 15 Layers)"));
    vBoxLayout1->addWidget(new QLabel("  θJB:6.8                  °C/W"));
    vBoxLayout1->addWidget(new QLabel("  Board temperature:       25.0°C"));

    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addLayout(vBoxLayout1);
}

void PowerSettingsWidget::loadData(nlohmann::basic_json<nlohmann::ordered_map> j)
{

}
