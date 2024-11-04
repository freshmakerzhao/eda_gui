#include "OutputClocksWidget.h"
#include "ipmanager/common/MultiLevelHeaderView.h"

OutputClocksWidget::OutputClocksWidget(AdvancedTableView *outputClockInforTableView,
                                       QStandardItemModel *outputClockInforModel,
                                       QWidget *parent) :
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
    // -----------------------------------------------------------
    QLabel *label = new QLabel("The phase is calculated relative to the active input clock.", this);
    mainLayout->addWidget(label);
    // tableView = new AdvancedTableView(this);
    tableView = outputClockInforTableView;
    tableView->verticalHeader()->setVisible(false);

    mainLayout->addWidget(tableView);

    const int row = 8;
    const int col = 11;
    // model = new QStandardItemModel(row, col);
    model = outputClockInforModel;
    tableView->setModel(model);

    auto pHeader = new MultiLevelHeaderView(Qt::Horizontal, 2, col, tableView);
    tableView->setHorizontalHeader(pHeader);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    pHeader->setCellSpan(0, 0, 2, 1);
    pHeader->setCellSpan(0, 1, 2, 1);
    pHeader->setCellSpan(0, 2, 1, 2);
    pHeader->setCellSpan(0, 4, 1, 2);
    pHeader->setCellSpan(0, 6, 1, 2);
    pHeader->setCellSpan(0, 8, 2, 1);
    pHeader->setCellSpan(0, 9, 2, 1);
    pHeader->setCellSpan(0, 10, 2, 1);

    pHeader->setCellSpan(1, 2, 1, 1);
    pHeader->setCellSpan(1, 3, 1, 1);
    pHeader->setCellSpan(1, 4, 1, 1);
    pHeader->setCellSpan(1, 5, 1, 1);
    pHeader->setCellSpan(1, 6, 1, 1);
    pHeader->setCellSpan(1, 7, 1, 1);
    //一级
    pHeader->setCellText(0, 0, "Output Clock");
    pHeader->setCellText(0, 1, "Port Name");
    pHeader->setCellText(0, 2, "Output Freq (MHz)");
    pHeader->setCellText(0, 4, "Phase (degrees)");
    pHeader->setCellText(0, 6, "Duty Cycle (%)");
    pHeader->setCellText(0, 8, "Drives");
    pHeader->setCellText(0, 9, "Use\nFine PS");
    pHeader->setCellText(0, 10, "Max Freq.\nof buffer");
    //二级
    pHeader->setCellText(1, 2, "Requested");
    pHeader->setCellText(1, 3, "Actual");
    pHeader->setCellText(1, 4, "Requested");
    pHeader->setCellText(1, 5, "Actual");
    pHeader->setCellText(1, 6, "Requested");
    pHeader->setCellText(1, 7, "Actual");

    // Output Clock
    for (int i = 0; i < model->rowCount(); i++) {
        QWidget *widget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setText(QString("clk_out%1").arg(i));
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(checkBox);
        hLayout->setMargin(0);
        hLayout->setAlignment(checkBox, Qt::AlignCenter);
        widget->setLayout(hLayout);
        tableView->setIndexWidget(model->index(i, 0), widget);
        if (i == 0) {
            checkBox->setChecked(true);
        }
    }
    // Port Name
    for (int i = 0; i < model->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText(QString("clk_out%1").arg(i));
        lineEdit->setClearButtonEnabled(true);
        tableView->setIndexWidget(model->index(i, 1), lineEdit);
    }
    // Output Freq(MHz) Requested
    for (int i = 0; i < model->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("100.000");
        lineEdit->setClearButtonEnabled(true);
        tableView->setIndexWidget(model->index(i, 2), lineEdit);
    }
    // Output Freq(MHz) Actual
    for (int i = 0; i < model->rowCount(); i++) {
        model->setData(model->index(i, 3), "100.00000");
        model->item(i, 3)->setFlags(model->item(i, 3)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    // Phase (degrees) Requested
    for (int i = 0; i < model->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("0.000");
        lineEdit->setClearButtonEnabled(true);
        tableView->setIndexWidget(model->index(i, 4), lineEdit);
    }
    // Phase (degrees) Actual
    for (int i = 0; i < model->rowCount(); i++) {
        model->setData(model->index(i, 5), "N/A");
        model->item(i, 5)->setFlags(model->item(i, 5)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    // Duty Cycle (%) Requested
    for (int i = 0; i < model->rowCount(); i++) {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setText("50.000");
        lineEdit->setClearButtonEnabled(true);
        tableView->setIndexWidget(model->index(i, 6), lineEdit);
    }
    // Duty Cycle (%) Actual
    for (int i = 0; i < model->rowCount(); i++) {
        model->setData(model->index(i, 7), "N/A");
        model->item(i, 7)->setFlags(model->item(i, 7)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }
    // Drives
    for (int i = 0; i < model->rowCount(); i++) {
        QComboBox *comboBox = new QComboBox;
        comboBox->addItems({"BUFG", "BUFH", "BUFGCE", "BUFHCE", "No buffer"});
        tableView->setIndexWidget(model->index(i, 8), comboBox);
    }
    // UseFine PS
    for (int i = 0; i < model->rowCount(); i++) {
        QWidget *widget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(checkBox);
        hLayout->setMargin(0);
        hLayout->setAlignment(checkBox, Qt::AlignCenter);
        widget->setLayout(hLayout);
        tableView->setIndexWidget(model->index(i, 9), widget);
    }
    // Max Freq.of buffer
    for (int i = 0; i < model->rowCount(); i++) {
        model->setData(model->index(i, 10), "N/A");
        model->item(i, 10)->setFlags(model->item(i, 10)->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    }

    tableView->horizontalScrollBar()->setVisible(false);
    tableView->verticalScrollBar()->setVisible(false);
    // -------------------------------------- Resize ------------------------------------------
    // Auto resize columns to fit content initially
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Store the widths of the columns
    QVector<int> columnWidths;
    for (int col = 0; col < model->columnCount(); ++col) {
        columnWidths.append(tableView->columnWidth(col));
    }

    // Set the mode to Interactive to allow manual resizing after automatic adjustment
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Set the widths back to the original sizes
    for (int col = 0; col < model->columnCount(); ++col) {
        tableView->setColumnWidth(col, columnWidths.at(col));
    }

    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();

    int width = tableView->verticalHeader()->width();
    for (int i = 0; i < tableView->model()->columnCount(); ++i) {
        width += tableView->columnWidth(i);
    }
    // qDebug() << "tableView->horizontalHeader()->model()->rowCount() : " << tableView->horizontalHeader()->model()->rowCount();
    int height = tableView->horizontalHeader()->height() * 2;
    qDebug() << "horizontalHeader()->height() : " << height;
    for (int i = 0; i < tableView->model()->rowCount(); ++i) {
        height += tableView->rowHeight(i);
    }

    // Add space for scrollbar if necessary
    if (tableView->verticalScrollBar()->isVisible()) {
        width += tableView->verticalScrollBar()->width();
    }
    if (tableView->horizontalScrollBar()->isVisible()) {
        height += tableView->horizontalScrollBar()->height();
    }

    tableView->setMinimumSize(width + 2, height + 2);
    tableView->setMaximumSize(width + 2, height + 2);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addStretch();
}

