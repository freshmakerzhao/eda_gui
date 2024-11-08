#include "PortRenamingWidget.h"

PortRenamingWidget::PortRenamingWidget(QWidget *parent) :
    BasePage(parent)
{
    // ----------------------- VCO Frequency ---------------------------
    QLabel *vcoFrequencyLabel = new QLabel("VCO Frequency", this);
    vcoFrequencyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vcoFrequencyLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(vcoFrequencyLabel);
    mainLayout->addSpacing(10);
    vcoFrequencyValLabel = new QLabel("VCO Freg = 1000.00000 MHZ", this);
    mainLayout->addWidget(vcoFrequencyValLabel);
    mainLayout->addSpacing(20);
    // -------------------- Optional Port Names ------------------------
    QLabel *optionalPortNamesLabel = new QLabel("Optional Port Names", this);
    optionalPortNamesLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(optionalPortNamesLabel);

    tableView = new AdvancedTableView(this);
    // tableView->setFixedSize(255, 70);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addWidget(tableView);

    QStringList headers;
    headers << "Other Pins" << "Port Name";
    model = new QStandardItemModel(1, 2);
    model->setHorizontalHeaderLabels(headers);
    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(false);
    model->setData(model->index(0, 0), "locked");
    model->item(0, 0)->setFlags(model->item(0, 0)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    QLineEdit *portNameEdit = new QLineEdit(tableView);
    portNameEdit->setClearButtonEnabled(true);
    portNameEdit->setText("locked");
    tableView->setIndexWidget(model->index(0, 1), portNameEdit);

    tableView->resizeTableView();
    mainLayout->addStretch(); // 添加一个弹簧以将控件聚集到顶部
}
