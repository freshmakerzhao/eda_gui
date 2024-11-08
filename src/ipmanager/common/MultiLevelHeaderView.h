#pragma once

#include <QHeaderView>
#include <QModelIndex>

enum MyItemDataRole
{
    COLUMN_SPAN_ROLE = Qt::UserRole + 1,
    ROW_SPAN_ROLE,
};

class MultiLevelHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    MultiLevelHeaderView(Qt::Orientation orientation, int rows, int columns, QWidget* parent = 0);
    virtual ~MultiLevelHeaderView();

    void setRowHeight(int row, int rowHeight);
    void setColumnWidth(int col, int colWidth);
    void setCellData(int row, int column, int role, const QVariant& value);
    // the below methods is just shortcut for setCellData
    void setCellSpan(int row, int column, int rowSpanCount, int columnSpanCount);
    void setCellBackgroundColor(int row, int column, const QColor&);
    void setCellForegroundColor(int row, int column, const QColor&);
    void setCellText(int row, int column, const QString& text);

    QModelIndex columnSpanIndex(const QModelIndex& currentIndex) const;
    QModelIndex rowSpanIndex(const QModelIndex& currentIndex) const;
protected:
    // override
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual QModelIndex indexAt(const QPoint&);
    virtual void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    virtual QSize sectionSizeFromContents(int logicalIndex) const override;

    // inherent features
    int columnSpanSize(int row, int from, int spanCount) const;
    int rowSpanSize(int column, int from, int spanCount) const;
    bool getRootCell(int row, int column, int& rootCellRow, int& rootCellColumn) const;
    // (row, column) must be root cell of merged cells
    QRect getCellRect(int row, int column) const;
    int getSectionRange(QModelIndex& index, int* beginSection, int* endSection) const;

protected slots:
    void onSectionResized(int logicalIdx, int oldSize, int newSize);

signals:
    void sectionPressed(int from, int to);
};

