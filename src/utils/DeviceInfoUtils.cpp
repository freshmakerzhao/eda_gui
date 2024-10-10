/**
  ******************************************************************************
  * @file           : DeviceInfoUtils.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/10
  ******************************************************************************
  */
#include "DeviceInfoUtils.h"

DeviceInfoUtils::DeviceInfoUtils() {
    QFile file(partFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 从资源读取YAML文件内容
        QTextStream in(&file);
        QString yamlData = in.readAll();

        // 使用yaml-cpp库解析YAML数据
        try {
            YAML::Node config = YAML::Load(yamlData.toStdString());
            // 处理YAML数据
            qDebug() << "YAML loaded successfully";
            for (auto it = config.begin(); it != config.end(); ++it) {
                DeviceInfo info;

                QString displayPart = QString::fromStdString(it->first.as<std::string>());

                info.io_pin_count = QString::fromStdString(it->second["io_pin_count"].as<std::string>());
                info.iob_count = QString::fromStdString(it->second["iob_count"].as<std::string>());
                info.lut_count = QString::fromStdString(it->second["lut_count"].as<std::string>());
                info.ff_count = QString::fromStdString(it->second["ff_count"].as<std::string>());
                info.bram_count = QString::fromStdString(it->second["bram_count"].as<std::string>());
                info.dsp_count = QString::fromStdString(it->second["dsp_count"].as<std::string>());
                info.pcie_count = QString::fromStdString(it->second["pcie_count"].as<std::string>());
                info.archName = QString::fromStdString(it->second["archName"].as<std::string>());
                info.arch = QString::fromStdString(it->second["arch"].as<std::string>());
                info.family_name = QString::fromStdString(it->second["family_name"].as<std::string>());
                info.part = QString::fromStdString(it->second["part"].as<std::string>());

                deviceMap[displayPart] = info;
            }
        } catch (const YAML::ParserException& e) {
            qDebug() << "Error parsing YAML: " << e.what();
        } catch (const YAML::Exception& e) {
            qDebug() << "YAML error: " << e.what();
        }
        file.close();
    } else {
        qDebug() << "Error opening file";
    }
}

DeviceInfo DeviceInfoUtils::getDeviceInfo(const QString &displayPart) const {
    if (deviceMap.contains(displayPart)) {
        return deviceMap[displayPart];
    } else {
        qDebug() << "Device not found:" << displayPart;
        return DeviceInfo();  // 返回空的 DeviceInfo
    }
}

QStandardItemModel *DeviceInfoUtils::getDeviceModel() const {
    QStandardItemModel *model = new QStandardItemModel();
    QStringList headers = {"Part", "I/O Pin Count", "Available IOBs", "LUT Elements", "FlipFlops" , "Block RAMs" , "DSPs" , "PCIe"};
    model->setHorizontalHeaderLabels(headers);
    for (auto it = deviceMap.constBegin(); it != deviceMap.constEnd(); ++it) {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(it.key()));

        items.append(new QStandardItem(it.value().io_pin_count));
        items.append(new QStandardItem(it.value().iob_count));
        items.append(new QStandardItem(it.value().lut_count));
        items.append(new QStandardItem(it.value().ff_count));
        items.append(new QStandardItem(it.value().bram_count));
        items.append(new QStandardItem(it.value().dsp_count));
        items.append(new QStandardItem(it.value().pcie_count));
        items.append(new QStandardItem(it.value().archName));
        items.append(new QStandardItem(it.value().arch));
        items.append(new QStandardItem(it.value().family_name));
        items.append(new QStandardItem(it.value().part));

        model->appendRow(items);
    }

    return model;
}
