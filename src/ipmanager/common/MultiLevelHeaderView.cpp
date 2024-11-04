#include <set>

#include <QAbstractTableModel>
#include <QMap>
#include <QPainter>
#include <QMouseEvent>
#include <QVariant>
#include <QBrush>
#include <qdrawutil.h>

#include "MultiLevelHeaderView.h"

struct Cell
{
    int row = 0;
    int column = 0;
    bool operator<(const Cell& oth) const
    {
        if (row < oth.row)
            return true;
        if (row == oth.row)
            return column < oth.column;
        return false;
    }
};

class MultiLevelHeaderModel : public QAbstractTableModel
{
public:
    MultiLevelHeaderModel(Qt::Orientation orientation, int rows, int cols, QObject* parent = 0);
    virtual ~MultiLevelHeaderModel();

public:
    // override
    virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setRowHeight(int row, int size);
    int getRowHeight(int row) const;
    void setColumnWidth(int col, int size);
    int getColumnWidth(int col) const;
    void setRootCell(int row, int column, int rootRow, int rootColumn);
    bool getRootCell(int row, int column, int & rootCellRow, int & rootCellColumn) const;
private:
    Qt::Orientation m_orientation;
    int m_rowCount;
    int m_columnCount;
    std::vector<int> m_rowSizes;
    std::vector<int> m_columnSizes;
    QMap<Cell, QMap<int, QVariant>> m_data;
    QMap<Cell, Cell> m_rootCellMap;
};

MultiLevelHeaderModel::MultiLevelHeaderModel(Qt::Orientation orientation, int rows, int cols, QObject* parent) :
    QAbstractTableModel(parent), m_orientation(orientation), m_rowCount(rows), m_columnCount(cols)
{
    m_rowSizes.resize(rows, 0);
    m_columnSizes.resize(cols, 0);
}

MultiLevelHeaderModel::~MultiLevelHeaderModel()
{
}

QModelIndex MultiLevelHeaderModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, nullptr);
}

int MultiLevelHeaderModel::rowCount(const QModelIndex &parent) const
{
    return m_rowCount;
}
int MultiLevelHeaderModel::columnCount(const QModelIndex &parent) const
{
    return m_columnCount;
}

QVariant MultiLevelHeaderModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_rowCount || index.row() < 0 || index.column() >= m_columnCount || index.column() < 0)
        return QVariant();

    auto it = m_data.find({ index.row(), index.column() });
    if (it != m_data.end())
    {
        auto it2 = it.value().find(role);
        if (it2 != it.value().end())
            return it2.value();
    }

    // default value
    if (role == Qt::BackgroundRole)
        // return QColor(0xcfcfcf);
        return QColor(Qt::white);

    if (role == Qt::SizeHintRole)
    {
        return QSize(m_columnSizes[index.column()], m_rowSizes[index.row()]);
    }
    return QVariant();
}

bool MultiLevelHeaderModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid())
    {
        QVariant value2;
        if (role == COLUMN_SPAN_ROLE)
        {
            int col = index.column();
            int span = value.toInt();
            if (span > 0) // span size should be more than 1, else nothing to do
            {
                if (col + span - 1 >= m_columnCount) // span size should be less than whole columns,
                    span = m_columnCount - col;
                value2 = span;
            }
        }
        else if (role == ROW_SPAN_ROLE)
        {
            int row = index.row();
            int span = value.toInt();
            if (span > 0) // span size should be more than 1, else nothing to do
            {
                if (row + span - 1 >= m_rowCount)
                    span = m_rowCount - row;
                value2 = span;
            }
        }
        else
        {
            value2 = value;
        }
        if (value.isValid())
        {
            if (role == Qt::SizeHintRole)
            {
                m_columnSizes[index.column()] = value.toSize().width();
                m_rowSizes[index.row()] = value.toSize().height();
            }
            else
            {
                auto& map = m_data[{index.row(), index.column()}];
                map.insert(role, value2);
            }
        }

        return true;
    }
    return false;
}

Qt::ItemFlags MultiLevelHeaderModel::flags(const QModelIndex &index) const
{
    return  Qt::NoItemFlags | QAbstractTableModel::flags(index);
}

void MultiLevelHeaderModel::setRowHeight(int row, int size)
{
    if (row >= 0 && row < m_rowCount)
    {
        m_rowSizes[row] = size;
        emit dataChanged(index(row, 0), index(row, m_columnCount - 1), QVector<int>() << Qt::SizeHintRole);
    }
}

int MultiLevelHeaderModel::getRowHeight(int row) const
{
    if (row >= 0 && row < m_rowCount)
        return m_rowSizes[row];
    return 0;
}

void MultiLevelHeaderModel::setColumnWidth(int col, int size)
{
    if (col >= 0 && col < m_columnCount)
    {
        m_columnSizes[col] = size;
        emit dataChanged(index(0, col), index(m_rowCount - 1, col),  QVector<int>() << Qt::SizeHintRole);
    }
}

int MultiLevelHeaderModel::getColumnWidth(int col) const
{
    if (col >= 0 && col < m_columnCount)
        return m_columnSizes[col];
    return 0;
}

void MultiLevelHeaderModel::setRootCell(int row, int column, int rootRow, int rootColumn)
{
    Cell c{ row, column };
    auto it = m_rootCellMap.find(c);
    assert(it == m_rootCellMap.end());
    Cell rc{ rootRow, rootColumn };
    m_rootCellMap.insert(c, rc);
}

bool MultiLevelHeaderModel::getRootCell(int row, int column, int & rootCellRow, int & rootCellColumn) const
{
    Cell c{ row, column };
    auto it = m_rootCellMap.find(c);
    if(it == m_rootCellMap.end())
        return false;
    rootCellRow = it.value().row;
    rootCellColumn = it.value().column;
    return true;
}


MultiLevelHeaderView::MultiLevelHeaderView(Qt::Orientation orientation, int rows, int columns, QWidget* parent) :
    QHeaderView(orientation, parent)
{
    // create header model
    MultiLevelHeaderModel* m = new MultiLevelHeaderModel(orientation, rows, columns);

    // set default size of item
    if (orientation == Qt::Horizontal)
    {
        // QFontMetrics _fontMetrics(this->font());
        // const int height = _fontMetrics.height() + 2;
        for (int row = 0; row < rows; ++row)
            m->setRowHeight(row, 30);
        for (int col = 0; col < columns; ++col)
            m->setColumnWidth(col, defaultSectionSize());
    }
    else
    {
        for (int row = 0; row < rows; ++row)
            m->setRowHeight(row, defaultSectionSize());
        for (int col = 0; col < columns; ++col)
            m->setColumnWidth(col, defaultSectionSize());
    }

    setModel(m);

    connect(this, SIGNAL(sectionResized(int, int, int)), this, SLOT(onSectionResized(int, int, int)));
}

MultiLevelHeaderView::~MultiLevelHeaderView()
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    if (m)
        delete m;
    setModel(nullptr);
}

void MultiLevelHeaderView::setRowHeight(int row, int rowHeight)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());

    m->setRowHeight(row, rowHeight);
    if (orientation() == Qt::Vertical)
        resizeSection(row, rowHeight);
}

void MultiLevelHeaderView::setColumnWidth(int col, int colWidth)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    m->setColumnWidth(col, colWidth);
    if (orientation() == Qt::Horizontal)
        resizeSection(col, colWidth);
}

void MultiLevelHeaderView::setCellData(int row, int column, int role, const QVariant & value)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    m->setData(m->index(row, column), value, role);
}

void MultiLevelHeaderView::setCellSpan(int row, int column, int rowSpanCount, int columnSpanCount)
{
    assert(rowSpanCount > 0);
    assert(columnSpanCount > 0);
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    QModelIndex idx = m->index(row, column);
    m->setData(idx, rowSpanCount, ROW_SPAN_ROLE);
    m->setData(idx, columnSpanCount, COLUMN_SPAN_ROLE);
    int lastRow = row + rowSpanCount;
    int lastCol = column + columnSpanCount;
    for (int i = row; i < lastRow; ++i)
    {
        for (int j = column; j < lastCol; ++j)
        {
            m->setRootCell(i, j, row, column);
        }
    }
}

void MultiLevelHeaderView::setCellBackgroundColor(int row, int column, const QColor& color)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    m->setData(m->index(row, column), color, Qt::BackgroundRole);
}

void MultiLevelHeaderView::setCellForegroundColor(int row, int column, const QColor& color)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    m->setData(m->index(row, column), color, Qt::ForegroundRole);
}

void MultiLevelHeaderView::setCellText(int row, int column, const QString & text)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    m->setData(m->index(row, column), text, Qt::DisplayRole);
}


void MultiLevelHeaderView::mousePressEvent(QMouseEvent* event)
{
    QHeaderView::mousePressEvent(event);
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    const int orient = orientation();
    if (index.isValid())
    {
        int beginSection = -1;
        int endSection = -1;
        int numbers = 0;
        numbers = getSectionRange(index, &beginSection, &endSection);
        if (numbers > 0)
        {
            emit sectionPressed(beginSection, endSection);
            return;
        }
        else
        {
            const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
            const int levelCount = (orient == Qt::Horizontal) ? m->rowCount() : m->columnCount();
            int logicalIdx = (orient == Qt::Horizontal) ? index.column() : index.row();
            int curLevel = (orient == Qt::Horizontal) ? index.row() : index.column();
            for (int i = 0; i < levelCount; ++i)
            {
                QModelIndex cellIndex = (orient == Qt::Horizontal) ? m->index(i, logicalIdx) : m->index(logicalIdx, i);
                numbers = getSectionRange(cellIndex, &beginSection, &endSection);
                if (numbers > 0)
                {
                    if (beginSection <= logicalIdx && logicalIdx <= endSection)
                    {
                        int beginLevel = (orient == Qt::Horizontal) ? cellIndex.row() : cellIndex.column();
                        QVariant levelSpanCnt = cellIndex.data((orient == Qt::Horizontal) ? ROW_SPAN_ROLE : COLUMN_SPAN_ROLE);
                        if (!levelSpanCnt.isValid())
                            continue;
                        int endLevel = beginLevel + levelSpanCnt.toInt() - 1;
                        if (beginLevel <= curLevel && curLevel <= endLevel)
                        {
                            emit sectionPressed(beginSection, endSection);
                            break;
                        }
                    }
                }
            }
        }
    }
}

QModelIndex MultiLevelHeaderView::indexAt(const QPoint& pos)
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
    const int orient = orientation();
    const int ROWS = m->rowCount();
    const int COLS = m->columnCount();
    int logicalIdx = logicalIndexAt(pos);

    if (orient == Qt::Horizontal)
    {
        int dY = 0;
        for (int row = 0; row < ROWS; ++row)
        {
            dY += m->getRowHeight(row);
            if (pos.y() <= dY)
            {
                QModelIndex cellIndex = m->index(row, logicalIdx);
                return cellIndex;
            }
        }
    }
    else
    {
        int dX = 0;
        for (int col = 0; col < COLS; ++col)
        {
            dX += m->getColumnWidth(col);
            if (pos.x() <= dX)
            {
                QModelIndex cellIndex = m->index(logicalIdx, col);
                return cellIndex;
            }
        }
    }

    return QModelIndex();
}

std::set<Cell> getCellsToBeDrawn(const MultiLevelHeaderView* view, const MultiLevelHeaderModel* m, int orient, int levelCount, int logicalIdx)
{
    std::set<Cell> cellsToBeDrawn;
    for (int i = 0; i < levelCount; ++i)
    {
        int row = i, column = logicalIdx;
        if (orient == Qt::Vertical)
            std::swap(row, column);
        int rootRow, rootCol;
        if (m->getRootCell(row, column, rootRow, rootCol))
            cellsToBeDrawn.insert({ rootRow, rootCol });
    }
    return cellsToBeDrawn;
}

void MultiLevelHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIdx) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
    const int orient = orientation();
    const int levelCount = (orient == Qt::Horizontal) ? m->rowCount() : m->columnCount();
    std::set<Cell> cellsToBeDrawn = getCellsToBeDrawn(this, m, orient, levelCount, logicalIdx);

    for (const auto& cell: cellsToBeDrawn)
    {
        QRect sectionRect = getCellRect(cell.row, cell.column);

        // draw section with style
        QStyleOptionHeader sectionStyle;
        initStyleOption(&sectionStyle);
        sectionStyle.textAlignment = Qt::AlignCenter;
        sectionStyle.iconAlignment = Qt::AlignVCenter;
        //sectionStyle.section = logicalIdx;
        QModelIndex cellIndex = m->index(cell.row, cell.column);
        sectionStyle.text = cellIndex.data(Qt::DisplayRole).toString();
        sectionStyle.rect = sectionRect;

        // file background or foreground color of the cell
        QVariant bg = cellIndex.data(Qt::BackgroundRole);
        QVariant fg = cellIndex.data(Qt::ForegroundRole);

        if (bg.canConvert(QVariant::Brush))
        {
            sectionStyle.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(bg));
            sectionStyle.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(bg));
        }
        if (fg.canConvert(QVariant::Brush))
        {
            sectionStyle.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(fg));
        }

        painter->save();
        QFont font;
        font.setBold(true);
        font.setPointSize(9);
        painter->setFont(font);
        qDrawShadePanel(painter, sectionStyle.rect, sectionStyle.palette, false, 1, &sectionStyle.palette.brush(QPalette::Button));
        style()->drawControl(QStyle::CE_HeaderLabel, &sectionStyle, painter);
        painter->restore();
    }
}

QSize MultiLevelHeaderView::sectionSizeFromContents(int logicalIdx) const
{
    const MultiLevelHeaderModel* m = static_cast<const MultiLevelHeaderModel*>(this->model());
    const int orient = orientation();
    const int levelCount = (orient == Qt::Horizontal) ? m->rowCount() : m->columnCount();

    int w = 0, h = 0;
    if (orient == Qt::Horizontal)
    {
        w = m->getColumnWidth(logicalIdx);
        for (int i = 0; i < levelCount; ++i)
        {
            h += m->getRowHeight(i);
        }
    }
    else
    {
        for (int i = 0; i < levelCount; ++i)
        {
            w += m->getColumnWidth(i);
        }
        h = m->getRowHeight(logicalIdx);
    }
    return QSize(w, h);
}

QModelIndex MultiLevelHeaderView::columnSpanIndex(const QModelIndex& currentIdx) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    const int curRow = currentIdx.row();
    const int curCol = currentIdx.column();
    int i = curCol;
    while (i >= 0)
    {
        QModelIndex spanIndex = m->index(curRow, i);
        QVariant span = spanIndex.data(COLUMN_SPAN_ROLE);
        if (span.isValid() && spanIndex.column() + span.toInt() - 1 >= curCol)
            return spanIndex;
        i--;
    }
    return QModelIndex();
}

QModelIndex MultiLevelHeaderView::rowSpanIndex(const QModelIndex& currentIdx) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    const int curRow = currentIdx.row();
    const int curCol = currentIdx.column();
    int i = curRow;
    while (i >= 0)
    {
        QModelIndex spanIndex = m->index(i, curCol);
        QVariant span = spanIndex.data(ROW_SPAN_ROLE);
        if (span.isValid() && spanIndex.row() + span.toInt() - 1 >= curRow)
            return spanIndex;
        i--;
    }
    return QModelIndex();
}

int MultiLevelHeaderView::columnSpanSize(int row, int from, int spanCount) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    int span = 0;
    for (int i = from; i < from + spanCount; ++i)
    {
        int colWidth = m->getColumnWidth(i);
        span += colWidth;
    }
    return span;
}


int MultiLevelHeaderView::rowSpanSize(int column, int from, int spanCount) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    int span = 0;
    for (int i = from; i < from + spanCount; ++i)
    {
        int rowHeight = m->getRowHeight(i);
        span += rowHeight;
    }
    return span;
}

bool MultiLevelHeaderView::getRootCell(int row, int column, int & rootCellRow, int & rootCellColumn) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
    return m->getRootCell(row, column, rootCellRow, rootCellColumn);
}

QRect MultiLevelHeaderView::getCellRect(int row, int column) const
{
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
    const int orient = orientation();
    QModelIndex cellIndex = m->index(row, column);
    auto colSpanVar = cellIndex.data(COLUMN_SPAN_ROLE);
    auto rowSpanVar = cellIndex.data(ROW_SPAN_ROLE);
    assert(colSpanVar.isValid() && rowSpanVar.isValid());
    int colSpan = colSpanVar.toInt();
    int rowSpan = rowSpanVar.toInt();
    int w = 0, h = 0, l = 0, t = 0;
    w = columnSpanSize(row, column, colSpan);
    h = rowSpanSize(column, row, rowSpan);
    if (orient == Qt::Horizontal)
    {
        l = sectionViewportPosition(column);
        for (int i = 0; i < row; ++i)
            t += m->getRowHeight(i);
    }
    else
    {
        for (int i = 0; i < column; ++i)
            l += m->getColumnWidth(i);
        t = sectionViewportPosition(row);
    }

    QRect rect(l, t, w, h);
    return rect;
}

/**
 * @return section numbers
 */
int MultiLevelHeaderView::getSectionRange(QModelIndex& index, int* beginSection, int* endSection) const
{
    int row = index.row(), column = index.column();
    int rootRow, rootCol;
    bool success = getRootCell(row, column, rootRow, rootCol);
    if (!success)
        return 0;
    const MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(model());
    QModelIndex rootIndex = m->index(rootRow, rootCol);
    if (orientation() == Qt::Horizontal)
    {
        int colSpanCnt = rootIndex.data(COLUMN_SPAN_ROLE).toInt();
        *beginSection = rootIndex.column();;
        *endSection = *beginSection + colSpanCnt - 1;
        index = rootIndex;
        return colSpanCnt;
    }
    else
    {
        int rowSpanCnt = rootIndex.data(ROW_SPAN_ROLE).toInt();
        *beginSection = rootIndex.row();;
        *endSection = *beginSection + rowSpanCnt - 1;
        index = rootIndex;
        return rowSpanCnt;
    }
}

void MultiLevelHeaderView::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    MultiLevelHeaderModel* m = static_cast<MultiLevelHeaderModel*>(this->model());
    const int orient = orientation();
    const int levelCount = (orient == Qt::Horizontal) ? m->rowCount() : m->columnCount();
    if (orient == Qt::Horizontal)
    {
        m->setColumnWidth(logicalIndex, newSize);
    }
    else
    {
        m->setRowHeight(logicalIndex, newSize);
    }

    std::set<Cell> cellsToBeDrawn = getCellsToBeDrawn(this, m, orient, levelCount, logicalIndex);
    for (const auto& cell : cellsToBeDrawn)
    {
        QRect sectionRect = getCellRect(cell.row, cell.column);
        if (orient == Qt::Horizontal)
        {
            sectionRect.setWidth(viewport()->width() - sectionRect.left());
        }
        else
        {
            sectionRect.setHeight(viewport()->height() - sectionRect.top());
        }
        viewport()->update(sectionRect);
    }
}

