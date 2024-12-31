/**
  ******************************************************************************
  * @file           : DefaultPartPage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#include <QRadioButton>
#include "DefaultPartPage.h"
#include "utils/DeviceInfoUtils.h"
#include "utils/ProjectManager.h"

DefaultPartPage::DefaultPartPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Default Part");
    setSubTitle("Choose a default part for your project.");

    tableView = new QTableView(this);
    tableView->verticalHeader()->setVisible(false); // 不显示行号

    // 启用水平滚动条
    tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    
    DeviceInfoUtils util;
    QStandardItemModel *model = util.getDeviceModel();
    tableView->setModel(model);

    // 设置整个表格为只读
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置第一列的宽度为190
    // tableView->setColumnWidth(0, 190);

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

    lineEdit = new QLineEdit(this);
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setFixedSize(220, 30);

    matchesLabel = new QLabel(this);

    connect(lineEdit, &QLineEdit::textChanged, [proxyModel, this](const QString &text){
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
        QModelIndex rootIndex = QModelIndex();
        int rowCount = proxyModel->rowCount(rootIndex);
        if (lineEdit->text().isEmpty()) {
            matchesLabel->setText("");
        } else {
            matchesLabel->setText(QString("Matches: %1").arg(rowCount));
        }
    });

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setMargin(0);
    hBoxLayout->addWidget(lineEdit);
    hBoxLayout->addWidget(matchesLabel);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Search:", hBoxLayout);
    layout->addLayout(formLayout);

    layout->addWidget(tableView);

    // Auto resize columns to fit content initially
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Store the widths of the columns
    QVector<int> columnWidths;
    for (int col = 0; col < model->columnCount(); ++col) {
        columnWidths.append(tableView->columnWidth(col) + 20);
    }

    // 设置成交互模式，并支持手动调节大小
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // 设置初始宽度
    for (int col = 0; col < model->columnCount(); ++col) {
        tableView->setColumnWidth(col, columnWidths.at(col));
    }

    // ---------------- 兼容模式 单选按钮 -----------------
    // 创建单选按钮
    compatibility_mode_button = new QRadioButton("Enable compatibility mode", this);
    compatibility_mode_button->setChecked(true); // 默认启用

    // 提示信息，创建项目后无法更改
    compatibility_mode_label = new QLabel("This option cannot be changed after creating the project.", this);
    compatibility_mode_label->setStyleSheet("color: gray; font-style: italic;");

    // 创建布局并添加单选按钮和提示信息
    QVBoxLayout *compatibilityModeLayout = new QVBoxLayout;
    compatibilityModeLayout->addWidget(compatibility_mode_button);
    compatibilityModeLayout->addWidget(compatibility_mode_label);
    // ---------------- 兼容模式 单选按钮 -----------------


    // 在页面中添加布局
    layout->addLayout(compatibilityModeLayout);
    setLayout(layout);
}

bool DefaultPartPage::isComplete() const
{
    // Part为必选项
    if (tableView->selectionModel()->selectedIndexes().isEmpty()) {
        return false; // 不满足条件，禁止进入下一步
    }
    ProjectManager::instance().updateCompressOption(compatibility_mode_button->isChecked() ? "enable" : "disable");
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
    emit completeChanged();
}

