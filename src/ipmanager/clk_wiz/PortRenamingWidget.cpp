#include "PortRenamingWidget.h"

PortRenamingWidget::PortRenamingWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    vBoxLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addSpacing(10);
    // ----------------------- VCO Frequency ---------------------------
    QLabel *vcoFrequencyLabel = new QLabel("VCO Frequency", this);
    vcoFrequencyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vcoFrequencyLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(vcoFrequencyLabel);
    mainLayout->addSpacing(10);
    vcoFrequencyValLabel = new QLabel("VCO Freg = 1000.00000 MHZ", this);
    mainLayout->addWidget(vcoFrequencyValLabel);
    mainLayout->addSpacing(20);
    // -------------------- Optional Port Names ------------------------
    QLabel *optionalPortNamesLabel = new QLabel("Optional Port Names", this);
    optionalPortNamesLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(optionalPortNamesLabel);

    tableView = new QTableView(this);
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

    // Auto resize columns to fit content initially
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int width = 0;
    for (int i = 0; i < tableView->model()->columnCount(); ++i) {
        width += tableView->columnWidth(i);
    }

    int height = tableView->horizontalHeader()->height();
    for (int i = 0; i < tableView->model()->rowCount(); ++i) {
        height += tableView->rowHeight(i);
    }

    tableView->setMinimumSize(width + 2, height + 2);
    tableView->setMaximumSize(width + 2, height + 2);

        tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addStretch(); // 添加一个弹簧以将控件聚集到顶部
}
