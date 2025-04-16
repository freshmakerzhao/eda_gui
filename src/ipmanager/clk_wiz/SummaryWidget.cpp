#include "SummaryWidget.h"
#include "qscrollbar.h"

SummaryWidget::SummaryWidget(QWidget *parent) :
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

    // QFormLayout *fLayout = new QFormLayout;
    // fLayout->setContentsMargins(25, 0, 0, 0);
    // mainLayout->addLayout(fLayout);
    // --------------- Primary Input Clock Attributes ------------------
    QLabel *primaryInputClockAttributesLabel = new QLabel("Primary Input Clock Attributes", this);
    primaryInputClockAttributesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    primaryInputClockAttributesLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(primaryInputClockAttributesLabel);

    primaryInputClockAttributesTableView = new QTableView(this);
    mainLayout->addWidget(primaryInputClockAttributesTableView);
    primaryInputClockAttributesTableView->setEditTriggers(QTableView::NoEditTriggers);
    primaryInputClockAttributesTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    primaryInputClockAttributesTableView->setSelectionMode(QAbstractItemView::NoSelection);
    primaryInputClockAttributesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // primaryInputClockAttributesTableView->setFixedHeight(115);
    primaryInputClockAttributesTableView->verticalHeader()->setVisible(false);
    primaryInputClockAttributesTableView->horizontalHeader()->setVisible(false);
    primaryInputClockAttributesModel = new QStandardItemModel(3, 2);
    primaryInputClockAttributesModel->setData(primaryInputClockAttributesModel->index(0, 0), "Input Clock Frequency(MHz)");
    primaryInputClockAttributesModel->setData(primaryInputClockAttributesModel->index(1, 0), "Clock Source");
    primaryInputClockAttributesModel->setData(primaryInputClockAttributesModel->index(2, 0), "Jitter");
    primaryInputClockAttributesTableView->setModel(primaryInputClockAttributesModel);

    mainLayout->addSpacing(20);
    // --------------- Clocking Primitive Attributes -------------------
    QLabel *clockingPrimitiveAttributesLabel = new QLabel("Clocking Primitive Attributes", this);
    clockingPrimitiveAttributesLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(clockingPrimitiveAttributesLabel);

    primitiveInstantiatedLabel = new QLabel("Primitive Instantiated : N/A", this);
    mainLayout->addWidget(primitiveInstantiatedLabel);
    divideCounterLabel = new QLabel("Divide Counter : N/A", this);
    mainLayout->addWidget(divideCounterLabel);
    multCounterLabel = new QLabel("Mult Counter : N/A", this);
    mainLayout->addWidget(multCounterLabel);
    clockPhaseShiftLabel = new QLabel("Clock Phase Shift : N/A", this);
    mainLayout->addWidget(clockPhaseShiftLabel);

    // -------------------------------------- Resize ------------------------------------------
    // Auto resize columns to fit content initially
    primaryInputClockAttributesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Store the widths of the columns
    QVector<int> columnWidths;
    for (int col = 0; col < primaryInputClockAttributesModel->columnCount(); ++col) {
        columnWidths.append(primaryInputClockAttributesTableView->columnWidth(col));
    }

    // Set the mode to Interactive to allow manual resizing after automatic adjustment
    primaryInputClockAttributesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Set the widths back to the original sizes
    for (int col = 0; col < primaryInputClockAttributesModel->columnCount(); ++col) {
        primaryInputClockAttributesTableView->setColumnWidth(col, columnWidths.at(col));
    }

    primaryInputClockAttributesTableView->resizeColumnsToContents();
    primaryInputClockAttributesTableView->resizeRowsToContents();

    int width = primaryInputClockAttributesTableView->verticalHeader()->width();
    for (int i = 0; i < primaryInputClockAttributesTableView->model()->columnCount(); ++i) {
        width += primaryInputClockAttributesTableView->columnWidth(i);
    }

    int height = primaryInputClockAttributesTableView->horizontalHeader()->height();
    for (int i = 0; i < primaryInputClockAttributesTableView->model()->rowCount(); ++i) {
        height += primaryInputClockAttributesTableView->rowHeight(i);
    }

    // Add space for scrollbar if necessary
    if (primaryInputClockAttributesTableView->verticalScrollBar()->isVisible()) {
        width += primaryInputClockAttributesTableView->verticalScrollBar()->width();
    }
    if (primaryInputClockAttributesTableView->horizontalScrollBar()->isVisible()) {
        height += primaryInputClockAttributesTableView->horizontalScrollBar()->height();
    }

    primaryInputClockAttributesTableView->setMinimumSize(width + 2, height + 2);
    primaryInputClockAttributesTableView->setMaximumSize(width + 2, height + 2);

    primaryInputClockAttributesTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addStretch(); // 添加一个弹簧以将控件聚集到顶部


}
