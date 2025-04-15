#ifndef POWERWIDGET_H
#define POWERWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QFile>
#include <iostream>

#include "utils/json.hpp"
#include "PowerSettingsWidget.h"
#include "PowerSummaryWidget.h"
#include "UtilizationWidget.h"

class PowerWidget : public QWidget
{
    Q_OBJECT
public:
    PowerWidget(QWidget *parent = nullptr);

private:
    void loadDataFromJson(const QString &json_file = "D:/ReportPower/resource/gui_power_data_100t(1).json") {
        QFile file(json_file);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open file";
            return;
        }

        QTextStream in(&file);
        QString fileContent = in.readAll();

        try {
            nlohmann::basic_json<nlohmann::ordered_map> j = nlohmann::basic_json<nlohmann::ordered_map>::parse(fileContent.toStdString());

            // 访问和打印 JSON 数据
            // 读取 summary 部分
            // auto summary = j["summary"];
            // std::cout << "Junction Temperature: " << summary["junction_temperature"] << std::endl;
            // std::cout << "Total On-Chip Power: " << summary["total_on_chip_power"] << std::endl;

            powerSettingsWidget->loadData(j);
            powerSummaryWidget->loadData(j);

            // auto on_chip_power = summary["on_chip_power"];
            // std::cout << "GTP: " << on_chip_power["GTP"] << std::endl;
            // std::cout << "Static Power: " << on_chip_power["static_power"] << std::endl;

            // auto dynamic_power = on_chip_power["dynamic_power"];
            // std::cout << "Dynamic Power - BRAM: " << dynamic_power["BRAM"] << std::endl;
            // std::cout << "Dynamic Power - DSP: " << dynamic_power["DSP"] << std::endl;
            // std::cout << "Dynamic Power - IO: " << dynamic_power["IO"] << std::endl;
            // std::cout << "Dynamic Power - MMCM: " << dynamic_power["MMCM"] << std::endl;
            // std::cout << "Dynamic Power - Clocks: " << dynamic_power["clocks"] << std::endl;
            // std::cout << "Dynamic Power - Logic: " << dynamic_power["logic"] << std::endl;
            // std::cout << "Dynamic Power - Signals: " << dynamic_power["signals"] << std::endl;

            // 读取 temperature_power_slopes 部分
            auto temperature_power_slopes = j["temperature_power_slopes"];
            for (const auto& slope : temperature_power_slopes) {
                std::cout << "Range: [" << slope["range"][0] << ", " << slope["range"][1] << "], Slope: " << slope["slope"] << std::endl;
            }

            // 读取 utilization_details 部分
            // auto utilization_details = j["utilization_details"];
            // for (const auto& clock : utilization_details["clocks"]) {
            //     std::cout << "Clock Name: " << clock["name"] << ", Frequency: " << clock["frequency"] << std::endl;
            // }

            clockUtilizationWidget->loadData(j);
            logicUtilizationWidget->loadData(j);

            // for (const auto& logic : utilization_details["logic"]) {
            //     std::cout << "Logic Name: " << logic["name"] << ", Type: " << logic["type"] << ", Utilization: " << logic["utilization"] << std::endl;
            // }

        } catch (const std::exception& e) {
            qDebug() << "JSON Parsing Error:" << e.what();
        }
    }

    void setupTreeView()
    {
        QStandardItem *root = _model->invisibleRootItem();

        QStandardItem *page1 = new QStandardItem("Settings");
        page1->setData(0, Qt::UserRole);  // 存储堆叠窗口索引
        root->appendRow(page1);

        QStandardItem *page2 = new QStandardItem("Summary");
        page2->setData(1, Qt::UserRole);
        root->appendRow(page2);

        QStandardItem *page3 = new QStandardItem("Power Supply");
        page3->setData(2, Qt::UserRole);
        root->appendRow(page3);

        QStandardItem *page4 = new QStandardItem("Utilization Details");
        // page4->setData(3, Qt::UserRole);
        root->appendRow(page4);

        QStandardItem *page5 = new QStandardItem("Clocks");
        page5->setData(3, Qt::UserRole);
        page4->appendRow(page5);

        QStandardItem *page6 = new QStandardItem("Logic");
        page6->setData(4, Qt::UserRole);
        page4->appendRow(page6);
    }

    void setupStackedWidget()
    {
        stackedWidget->addWidget(powerSettingsWidget);
        stackedWidget->addWidget(powerSummaryWidget);
        stackedWidget->addWidget(new QLabel("Power Supply", stackedWidget));
        stackedWidget->addWidget(clockUtilizationWidget);
        stackedWidget->addWidget(logicUtilizationWidget);
    }

private slots:
    void onTreeSelectionChanged(const QModelIndex &current)
    {
        // 获取存储的堆叠索引
        QVariant indexData = current.data(Qt::UserRole);
        if (indexData.isValid()) {
            bool ok;
            int stackIndex = indexData.toInt(&ok);

            if (ok && stackIndex < stackedWidget->count()) {
                stackedWidget->setCurrentIndex(stackIndex);
            }
        }
    }

    void onDynamicPowerLegendClick(const Legend &legend)
    {
        switch (legend) {
        case Legend::Clocks:
            stackedWidget->setCurrentWidget(clockUtilizationWidget);
            break;
        case Legend::Signals:

            break;
        case Legend::Logic:
            stackedWidget->setCurrentWidget(logicUtilizationWidget);
            break;
        case Legend::MMCM:

            break;
        case Legend::IO:

            break;
        default:
            break;
        }
    }

private:
    QTreeView *treeView;

    QStackedWidget *stackedWidget;

    QStandardItemModel* _model;

    PowerSettingsWidget *powerSettingsWidget;

    PowerSummaryWidget *powerSummaryWidget;

    UtilizationWidget *clockUtilizationWidget;

    UtilizationWidget *logicUtilizationWidget;
};

#endif // POWERWIDGET_H
