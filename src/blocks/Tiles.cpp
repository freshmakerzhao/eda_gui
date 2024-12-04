#include "Tiles.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <utility>

Tiles::Tiles(const QColor &color, int loc_x, int loc_y, int index_x,int index_y,int cur_width, int cur_height, std::string &cur_type)
    : loc_x(loc_x), loc_y(loc_y), index_x(index_x), index_y(index_y), tile_width(cur_width), tile_height(cur_height), tile_type(cur_type){
    this->tile_color = color;
    setFlags(ItemIsSelectable);
    // 开启悬浮操作
    setAcceptHoverEvents(true);
}

// 添加子模块
void Tiles::addSubBlock(SitesBlock *subBlock) {
    subBlock->setParentItem(this);
    child_items.append(subBlock);
}

// 碰撞检测大小为CLB_block_width，CLB_block_height
QRectF Tiles::boundingRect() const
{
    return QRectF(0, 0, tile_width, tile_height);
}

QPainterPath Tiles::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, tile_width, tile_height);
    return path;
}

void Tiles::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    // 控件当前是否被选中：QStyle::State_Selected，依赖 setFlags(ItemIsSelectable);
    // 选中则加深，否则为初始化时的颜色
    QColor fillColor = (option->state & QStyle::State_Selected) ? tile_color.darker(150) : tile_color;

    if (option->state & QStyle::State_MouseOver){
        // 鼠标悬浮高亮
        fillColor = fillColor.lighter(125);
    }

    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.125) {
        // 当缩小到非常小时，不需要内部细节，直接用颜色覆盖即可
        painter->fillRect(QRectF(0, 0, tile_width, tile_height).toAlignedRect(), fillColor);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    // int width = 0;
    // if (option->state & QStyle::State_Selected)
    //     width += 2;

    // pen.setWidth(width);

    pen.setWidthF(1.0 / lod);
    painter->setPen(pen);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));

    // 显示位置及大小
    painter->drawRect(QRectF(0, 0, tile_width, tile_height));
    painter->setBrush(b);

//    painter->setPen(QPen(Qt::black, 0));

    // Draw text
    if (lod >= 0.5 && tiles_name_visible_status) {
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline); // 强制字体以轮廓方式渲染
        painter->setFont(font);
        painter->save();
        painter->drawText(QRectF(0, 0, tile_width, 35),Qt::AlignCenter, QString(QString::fromStdString(tile_type)));
        painter->drawText(QRectF(0, 0, tile_width, 80),Qt::AlignCenter, QString("X%1Y%2").arg(loc_x).arg(loc_y));
        painter->restore();
    }
}

void Tiles::setColor(const QColor &color){
    tile_color = color;
}
void Tiles::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 发出信号，告知是哪tile被点击了
    emit BlockClicked(index_x, index_y);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Tiles::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Tiles::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Tiles::updateTilesNameVisibleStatus(bool status) {
    tiles_name_visible_status = status;
    update();
}


