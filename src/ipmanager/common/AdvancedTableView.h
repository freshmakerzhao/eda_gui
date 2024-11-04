#ifndef ADVANCEDTABLEVIEW_H
#define ADVANCEDTABLEVIEW_H

#include <QApplication>
#include <QTableView>
#include <QLineEdit>
#include <QScrollBar>
#include <QComboBox>
#include <QHeaderView>
#include <QStandardItemModel>

class AdvancedTableView : public QTableView {
    Q_OBJECT

public:
    AdvancedTableView(QWidget *parent = nullptr) : QTableView(parent) {
        // 设置列的调整模式为 Interactive
        horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

        // 监听列宽变化的信号
        connect(horizontalHeader(), &QHeaderView::sectionResized, this, &AdvancedTableView::updateEmbeddedWidgets);
    }

    void resizeTableView() {
        if (!this->model()) {
            return;
        }

        // Auto resize columns to fit content initially
        this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        // Store the widths of the columns
        QVector<int> columnWidths;
        for (int col = 0; col < this->model()->columnCount(); ++col) {
            columnWidths.append(this->columnWidth(col));
        }

        // Set the mode to Interactive to allow manual resizing after automatic adjustment
        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

        // Set the widths back to the original sizes
        for (int col = 0; col < this->model()->columnCount(); ++col) {
            this->setColumnWidth(col, columnWidths.at(col));
        }

        this->resizeColumnsToContents();
        this->resizeRowsToContents();

        int width = this->verticalHeader()->width();
        for (int i = 0; i < this->model()->columnCount(); ++i) {
            width += this->columnWidth(i);
        }

        int height = this->horizontalHeader()->height();
        for (int i = 0; i < this->model()->rowCount(); ++i) {
            height += this->rowHeight(i);
        }

        // Add space for scrollbar if necessary
        if (this->verticalScrollBar()->isVisible()) {
            width += this->verticalScrollBar()->width();
        }
        if (this->horizontalScrollBar()->isVisible()) {
            height += this->horizontalScrollBar()->height();
        }

        this->setMinimumSize(width + 2, height + 2);
        this->setMaximumSize(width + 2, height + 2);

        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

private slots:
    void updateEmbeddedWidgets(int logicalIndex, int, int newSize) {
        Q_UNUSED(logicalIndex);
        Q_UNUSED(newSize);

        for (int row = 0; row < model()->rowCount(); ++row) {
            for (int col = 0; col < model()->columnCount(); ++col) {
                QModelIndex index = model()->index(row, col);
                QWidget *widget = indexWidget(index);
                if (widget) {
                    QRect rect = visualRect(index);
                    widget->setGeometry(rect);
                }
            }
        }
    }

private:

};

#endif // ADVANCEDTABLEVIEW_H
