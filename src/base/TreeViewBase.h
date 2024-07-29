/**
  ******************************************************************************
  * @file           : TreeViewBase.h
  * @author         : ksy
  * @description    : 为QTreeView提供搜索/折叠等扩展
  * @attention      : None
  * @date           : 2024/7/26
  ******************************************************************************
  */
#ifndef TREEVIEWBASE_H
#define TREEVIEWBASE_H

#include <QTreeView>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSortFilterProxyModel>
#include <QDebug>

class TreeViewBase : public QWidget
{
    Q_OBJECT
public:
    TreeViewBase(QTreeView *treeView, QWidget *parent = nullptr);

private:
    QWidget *baseWidget;

    QToolBar *toolBar;

    QLineEdit *lineEdit;

    QLabel *matchesLabel;
};

// class CustomSortFilterProxyModel : public QSortFilterProxyModel {
// protected:
//     bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
//         QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
//         if (sourceModel()->data(index).toString().contains(filterRegExp())) {
//             return true;
//         }

//         // 使用栈来模拟递归
//         QList<QModelIndex> stack;
//         stack.push_back(index);

//         while (!stack.isEmpty()) {
//             QModelIndex current = stack.takeLast(); // Removes the last item in the list and returns it.
//             int childCount = sourceModel()->rowCount(current);
//             for (int row = 0; row < childCount; ++row) {
//                 QModelIndex childIndex = sourceModel()->index(row, 0, current);
//                 // qDebug() << sourceModel()->data(childIndex).toString();
//                 if (sourceModel()->data(childIndex).toString().contains(filterRegExp())) {
//                     return true;
//                 }
//                 stack.push_back(childIndex);
//             }
//         }

//         return false;
//     }
// };

#endif // TREEVIEWBASE_H
