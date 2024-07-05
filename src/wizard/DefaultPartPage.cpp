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

    QStandardItemModel *model = new QStandardItemModel(0, 8, this);
    tableView->setModel(model);
    QStringList headers = {"Part", "I/O Pin Count", "Available IOBs", "LUT Elements", "FlipFlops" , "Block RAMs" , "DSPs" , "PCIe"};
    model->setHorizontalHeaderLabels(headers);


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
            int row = 0;
            for (auto it = config.begin(); it != config.end(); ++it) {
                QString displayPart = QString::fromStdString(it->first.as<std::string>());

                QString io_pin_count = QString::fromStdString(it->second["io_pin_count"].as<std::string>());
                QString iob_count = QString::fromStdString(it->second["iob_count"].as<std::string>());
                QString lut_count = QString::fromStdString(it->second["lut_count"].as<std::string>());
                QString ff_count = QString::fromStdString(it->second["ff_count"].as<std::string>());
                QString bram_count = QString::fromStdString(it->second["bram_count"].as<std::string>());
                QString dsp_count = QString::fromStdString(it->second["dsp_count"].as<std::string>());
                QString pcie_count = QString::fromStdString(it->second["pcie_count"].as<std::string>());
                QString archName = QString::fromStdString(it->second["archName"].as<std::string>());
                QString arch = QString::fromStdString(it->second["arch"].as<std::string>());
                QString family_name = QString::fromStdString(it->second["family_name"].as<std::string>());
                QString part = QString::fromStdString(it->second["part"].as<std::string>());

                model->setItem(row, 0, new QStandardItem(displayPart));
                model->setItem(row, 1, new QStandardItem(io_pin_count));
                model->setItem(row, 2, new QStandardItem(iob_count));
                model->setItem(row, 3, new QStandardItem(lut_count));
                model->setItem(row, 4, new QStandardItem(ff_count));
                model->setItem(row, 5, new QStandardItem(bram_count));
                model->setItem(row, 6, new QStandardItem(dsp_count));
                model->setItem(row, 7, new QStandardItem(pcie_count));
                model->setItem(row, 8, new QStandardItem(archName));
                model->setItem(row, 9, new QStandardItem(arch));
                model->setItem(row, 10, new QStandardItem(family_name));
                model->setItem(row, 11, new QStandardItem(part));

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
    // 设置第一列的宽度为200
    tableView->setColumnWidth(0, 180);
    // 获取表头视图
    QHeaderView *header = tableView->horizontalHeader();
    // 第一列设置为固定宽度
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    // 其他列设置为自动分配剩余空间
    for (int i = 1; i < model->columnCount(); ++i) {
        header->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    // 设置宽度自适应
//    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 将同一行标记为选中状态
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    // 隐藏第四 第五列 archName/arch/family_name 仅供程序使用，用户无需获取
    tableView->setColumnHidden(8, true);
    tableView->setColumnHidden(9, true);
    tableView->setColumnHidden(10, true);
    tableView->setColumnHidden(11, true);
    QObject::connect(tableView, &QTableView::clicked, this, &DefaultPartPage::selectPart);

    // Filter
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    tableView->setModel(proxyModel);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setFixedSize(220, 30);
    connect(lineEdit, &QLineEdit::textChanged, [proxyModel](const QString &text){
        proxyModel->setFilterFixedString(text);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Search:", lineEdit);
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
        wizard->displayPart = idx.siblingAtColumn(0).data(Qt::DisplayRole).toString();     // 获取 displayPart
        wizard->archName = idx.siblingAtColumn(8).data(Qt::DisplayRole).toString(); // 获取archName
        wizard->arch = idx.siblingAtColumn(9).data(Qt::DisplayRole).toString();     // 获取arch
        wizard->familyName = idx.siblingAtColumn(10).data(Qt::DisplayRole).toString();     // 获取family_name
        wizard->part = idx.siblingAtColumn(11).data(Qt::DisplayRole).toString();     // 获取part

        qDebug() << "[DefaultPartPage] wizard param: "<< wizard->part << wizard->archName << wizard->arch << wizard->familyName << wizard->displayPart;
    }

    completeChanged();
}

