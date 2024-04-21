/**
  ******************************************************************************
  * @file           : DefaultPartPage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#include "DefaultPartPage.h"
#include <yaml-cpp/yaml.h>

DefaultPartPage::DefaultPartPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Default Part");
    setSubTitle("Choose a default part for your project.");

    tableView = new QTableView(this);
    tableView->verticalHeader()->setVisible(false); // 不显示行号
    QStandardItemModel *model = new QStandardItemModel(0, 4, this);
    tableView->setModel(model);
    QStringList headers = {"part", "device", "package", "speedgrade", "archName" , "arch"};
    model->setHorizontalHeaderLabels(headers);

    QFile file(":/resource/parts.yaml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 从资源读取YAML文件内容
        QTextStream in(&file);
        QString yamlData = in.readAll();

        // 使用yaml-cpp库解析YAML数据
        try {
            YAML::Node config = YAML::Load(yamlData.toStdString());
            // 处理YAML数据
            qDebug() << "YAML loaded successfully";
            int row = 0;
            for (auto it = config.begin(); it != config.end(); ++it) {
                QString part = QString::fromStdString(it->first.as<std::string>());
                QString device = QString::fromStdString(it->second["device"].as<std::string>());
                QString package = QString::fromStdString(it->second["package"].as<std::string>());
                QString speedgrade = QString::fromStdString(it->second["speedgrade"].as<std::string>());
                QString archName = QString::fromStdString(it->second["archName"].as<std::string>());
                QString arch = QString::fromStdString(it->second["arch"].as<std::string>());
                model->setItem(row, 0, new QStandardItem(part));
                model->setItem(row, 1, new QStandardItem(device));
                model->setItem(row, 2, new QStandardItem(package));
                model->setItem(row, 3, new QStandardItem(speedgrade));
                model->setItem(row, 4, new QStandardItem(archName));
                model->setItem(row, 5, new QStandardItem(arch));
                row++;
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

    // 设置整个表格为只读
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置宽度自适应
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 将同一行标记为选中状态
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    // 隐藏第四 第五列 archName/arch 仅供程序使用，用户无需获取
    tableView->setColumnHidden(4, true);
    tableView->setColumnHidden(5, true);
    QObject::connect(tableView, &QTableView::clicked, this, &DefaultPartPage::selectPart);

    // Filter
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    tableView->setModel(proxyModel);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setFixedSize(220, 30);
    connect(lineEdit, &QLineEdit::textChanged, [proxyModel](const QString &text){
        proxyModel->setFilterFixedString(text);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Filter:", lineEdit);
    layout->addLayout(formLayout);

    layout->addWidget(tableView);
}

bool DefaultPartPage::isComplete() const
{
    // Part为必选项
    if (tableView->selectionModel()->selectedIndexes().isEmpty()) {
        return false; // 不满足条件，禁止进入下一步
    }
    return true;
}

void DefaultPartPage::selectPart(const QModelIndex &index) {
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(tableView->model());
    if (proxyModel) {
        QModelIndex idx = proxyModel->mapToSource(index);

        Wizard *wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->part = idx.siblingAtColumn(0).data(Qt::DisplayRole).toString();     // 获取part
        wizard->archName = idx.siblingAtColumn(4).data(Qt::DisplayRole).toString(); // 获取archName
        wizard->arch = idx.siblingAtColumn(5).data(Qt::DisplayRole).toString();     // 获取arch

        qDebug() << wizard->part << wizard->archName << wizard->arch;
    }

    completeChanged();
}

