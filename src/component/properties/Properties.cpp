/**
  ******************************************************************************
  * @file           : Properties.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2025/1/15
  ******************************************************************************
  */
#include "Properties.h"

Properties *Properties::instance(QWidget *parent)
{
    static Properties *_instance = nullptr;
    if (!_instance) {
        _instance = new Properties(parent);
    }
    return _instance;
}

void Properties::updateHardwareProperties(QJsonObject dataObj)
{
    _model->removeRows(0, _model->rowCount());

    for (auto it = dataObj.begin(); it != dataObj.end(); ++it) {
        QJsonObject obj = it.value().toObject();
        QString name = obj.value("register_name").toString();
        QString hexValue = obj.value("hex_value").toString();

        // 创建行数据
        QStandardItem *nameItem = new QStandardItem(name);
        QStandardItem *hexItem = new QStandardItem(hexValue);

        // 设置第 0 列（Name）为不可修改
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);

        // 将行添加到模型
        _model->appendRow({nameItem, hexItem});
    }
}

Properties::Properties(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[Properties] Constructing...";
    setStyleSheet(".QTreeView {"
                  " border: 0.5px solid #8c8c8c;"
                  "}");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);

    treeView = new QTreeView;
    treeView->setHeaderHidden(true);
    mainLayout->addWidget(treeView);

    _model = new QStandardItemModel(treeView);
    treeView->setModel(_model);
    connect(_model, &QStandardItemModel::dataChanged,
            this, &Properties::onDataChanged);

    QString jsonString = R"({
        "data": {
            "01100": {
                "register_name": "CONFIG_STATUS",
                "register_address": "00111",
                "hex_value": "0x13631093",
                "bin_value": "0b00010011011000110001000010010011"
            }
        }
    })";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON";
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonObject dataObj = rootObj.value("data").toObject();

    updateHardwareProperties(dataObj);
}

Properties::~Properties()
{
    qDebug() << "[Properties] Distructing...";
}

void Properties::onDataChanged(const QModelIndex &topLeft,
                               const QModelIndex &bottomRight) {
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        QString newValue = _model->item(row, 1)->text();
        qDebug() << "Hex Value updated for row" << row << ": " << newValue;
    }
}

