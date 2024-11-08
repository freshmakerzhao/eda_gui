#include "ClkWizSummary.h"

ClkWizSummary::ClkWizSummary(QWidget *parent) :
    BasePage(parent)
{
    // --------------- Primary Input Clock Attributes ------------------
    QLabel *primaryInputClockAttributesLabel = new QLabel("Primary Input Clock Attributes", this);
    primaryInputClockAttributesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    primaryInputClockAttributesLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(primaryInputClockAttributesLabel);

    primaryInputClockAttributesTableView = new AdvancedTableView(this);
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

    primaryInputClockAttributesTableView->resizeTableView();
    mainLayout->addStretch(); // 添加一个弹簧以将控件聚集到顶部
}
