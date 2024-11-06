/**
  ******************************************************************************
  * @file           : CoeFileEditor.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/6
  ******************************************************************************
  */
#include "CoeFileEditor.h"

CoeFileEditor::CoeFileEditor(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(800, 300);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    tableView = new QTableView(this);
    tableView->verticalHeader()->setVisible(false); // 不显示行号
    model = new QStandardItemModel(0, 2, this);
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList headers = {"Key", "Value"};
    model->setHorizontalHeaderLabels(headers);
    model->setItem(0, 0, new QStandardItem("memory_initialization_radix"));
    model->item(0, 0)->setFlags(model->item(0, 0)->flags() & ~Qt::ItemIsEditable);
    model->setItem(1, 0, new QStandardItem("memory_initialization_vector"));
    model->item(1, 0)->setFlags(model->item(1, 0)->flags() & ~Qt::ItemIsEditable);
    mainLayout->addWidget(tableView);

    QPushButton *saveAsButton = new QPushButton("Save As...");
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save, Qt::Horizontal, this);
    buttonBox->addButton(saveAsButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(QDialogButtonBox::Close);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &CoeFileEditor::onSaveTriggered);

    connect(saveAsButton, &QPushButton::clicked, this, &CoeFileEditor::onSaveAsTriggered);

    connect(buttonBox, &QDialogButtonBox::rejected, [this](){
        this->close();
    });
    mainLayout->addWidget(buttonBox);
}

int CoeFileEditor::exec()
{
    QDialog::exec();
    return 0;
}

void CoeFileEditor::onSaveTriggered()
{
    qDebug() << "Save";
    if (model->item(0, 1) && model->item(1, 1)) {
        qDebug() << "memory_initialization_radix  :" << model->item(0, 1)->text();
        qDebug() << "memory_initialization_vector :" << model->item(1, 1)->text();
    }

    this->accept();
}

void CoeFileEditor::onSaveAsTriggered()
{
    qDebug() << "SaveAs...";
    if (model->item(0, 1) && model->item(1, 1)) {
        qDebug() << "memory_initialization_radix  :" << model->item(0, 1)->text();
        qDebug() << "memory_initialization_vector :" << model->item(1, 1)->text();
    }

    this->accept();
}
