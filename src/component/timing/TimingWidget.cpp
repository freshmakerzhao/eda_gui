#include "TimingWidget.h"
// #include "TimingViewer.h"

TimingWidget::TimingWidget(QWidget *parent)
    : QWidget(parent),
    treeView(new QTreeView),
    stackedWidget(new QStackedWidget),    
    _model(new QStandardItemModel(this))
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);

    treeView->setModel(_model);
    treeView->setHeaderHidden(true);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(treeView);
    splitter->addWidget(stackedWidget);

    vBoxLayout->addWidget(splitter);
    splitter->setHandleWidth(10);

    QList<int> list;
    list << 500 << 1100;
    splitter->setSizes(list);

    setupTreeView();
    setupStackedWidget();

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &TimingWidget::onTreeSelectionChanged);

    treeView->expandAll();
}

