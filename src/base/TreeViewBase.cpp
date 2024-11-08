/**
  ******************************************************************************
  * @file           : TreeViewBase.h
  * @author         : ksy
  * @description    : 为QTreeView提供搜索/折叠等扩展
  * @attention      : None
  * @date           : 2024/7/26
  ******************************************************************************
  */
#include "TreeViewBase.h"

TreeViewBase::TreeViewBase(QTreeView *treeView,
                           const bool &enableSearch,
                           QWidget *parent)
    : QWidget(parent),
    toolBar(new QToolBar(this))
{
    toolBar = new QToolBar(this);
    // toolBar->setStyleSheet(
    //     ".QToolBar { "
    //     "background-color: white; "
    //     "border-top: 1px solid #878790; "
    //     "border-left: 1px solid #878790; "
    //     "border-right: 1px solid #878790; "
    //     // "background-color: lightblue; "
    //     // "border: 2px solid black; "  // 2像素宽的黑色实线边框
    //     "border-radius: 0px; "      // 可选：圆角半径
    //     "}"
    //     );
    QAction *searchAction = new QAction("Search", toolBar);
    searchAction->setCheckable(true); // 不能删除，否则按钮选中效果不生效
    QAction *expandAllAction = new QAction("ExpandAll", toolBar);
    QAction *collapseAllAction = new QAction("CollapseAll", toolBar);
    toolBar->addAction(searchAction);
    toolBar->addSeparator();
    toolBar->addAction(expandAllAction);
    toolBar->addSeparator();
    toolBar->addAction(collapseAllAction);
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);

    baseWidget = new QWidget(this);
    baseWidget->setVisible(false);
    vlayout->addWidget(baseWidget);

    lineEdit = new QLineEdit(baseWidget);
    lineEdit->setFixedWidth(170);
    lineEdit->setClearButtonEnabled(true);

    matchesLabel = new QLabel("", baseWidget);

    if (!enableSearch) {
        searchAction->setVisible(false);
        matchesLabel->hide();
    }

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(matchesLabel);

    QFormLayout *fLayout = new QFormLayout(baseWidget);
    fLayout->setMargin(3);
    fLayout->addRow("Search:", hLayout);

    vlayout->addWidget(treeView);

    connect(searchAction, &QAction::triggered, [this, searchAction](bool) {
        if (baseWidget->isVisible()) {
            lineEdit->clear();
            baseWidget->setVisible(false);
            searchAction->setChecked(false);
        } else {
            baseWidget->setVisible(true);
            searchAction->setChecked(true);
        }
    });

    connect(expandAllAction, &QAction::triggered, treeView, &QTreeView::expandAll);

    connect(collapseAllAction, &QAction::triggered, treeView, &QTreeView::collapseAll);

    // CustomSortFilterProxyModel *proxyModel = new CustomSortFilterProxyModel;
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setRecursiveFilteringEnabled(true); // 递归匹配满足的子节点，父类可见
    proxyModel->setSourceModel(treeView->model());
    treeView->setModel(proxyModel);
    treeView->expandAll();

    QObject::connect(lineEdit, &QLineEdit::textChanged, [treeView, proxyModel, this](const QString &text) {
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
        treeView->expandAll();

        QModelIndex rootIndex = QModelIndex();
        int secondLevelCount = 0;

        int rowCount = proxyModel->rowCount(rootIndex);
        for (int row = 0; row < rowCount; ++row) {
            QModelIndex childIndex = proxyModel->index(row, 0, rootIndex);
            secondLevelCount += proxyModel->rowCount(childIndex);
        }

        // // 初始化计数
        // QModelIndex rootIndex = QModelIndex();
        // int userRoleCount = 0;

        // // 遍历第一层节点
        // int rowCount = proxyModel->rowCount(rootIndex);
        // for (int row = 0; row < rowCount; ++row) {
        //     QModelIndex childIndex = proxyModel->index(row, 0, rootIndex);

        //     // 遍历每个子节点，检查是否有 UserRole 数据
        //     int childCount = proxyModel->rowCount(childIndex);
        //     for (int childRow = 0; childRow < childCount; ++childRow) {
        //         QModelIndex subChildIndex = proxyModel->index(childRow, 0, childIndex);
        //         QVariant userRoleData = proxyModel->data(subChildIndex, Qt::UserRole);
        //         if (!userRoleData.isNull()) {
        //             ++userRoleCount;
        //         }
        //     }
        // }

        if (lineEdit->text().isEmpty()) {
            matchesLabel->setText("");
        } else {
            matchesLabel->setText(QString("Matches: %1").arg(secondLevelCount));
            // matchesLabel->setText(QString("Matches: %1").arg(userRoleCount));
        }
        // qDebug() << "Second level node count:" << secondLevelCount;

    });
}
