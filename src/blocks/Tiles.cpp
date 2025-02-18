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
    if(tile_type == "NULL")
        return;
    Q_UNUSED(widget);
    // 控件当前是否被选中：QStyle::State_Selected，依赖 setFlags(ItemIsSelectable);
    // 选中则加深，否则为初始化时的颜色
    QColor fillColor = (option->state & QStyle::State_Selected) ? tile_color.darker(150) : tile_color;

//    if (option->state & QStyle::State_MouseOver){
//        // 鼠标悬浮高亮
//        fillColor = fillColor.lighter(125);
//    }

    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    painter->fillRect(QRectF(0, 0, tile_width, tile_height), fillColor);

//    if(!child_items.isEmpty())
//        if(lod > 0.01)
//            child_items[0]->updateVisibleStatus(false);
//        else
//            child_items[0]->updateVisibleStatus(true);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    pen.setCosmetic(true);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));

    // 显示位置及大小
    painter->drawRect(QRect(0, 0, tile_width, tile_height));
    painter->setBrush(b);

    painter->setPen(QPen(Qt::black, 0));

    // Draw text
    if ( tiles_name_visible_status) {
        QFont font("Times", 100);
        font.setStyleStrategy(QFont::ForceOutline); // 强制字体以轮廓方式渲染
        painter->setFont(font);
        painter->save();
        painter->drawText(QRect(0, 0, tile_width, 350),Qt::AlignCenter, QString(QString::fromStdString(tile_type)));
        painter->drawText(QRect(0, 0, tile_width, 800),Qt::AlignCenter, QString("X%1Y%2").arg(loc_x).arg(loc_y));
        painter->restore();
    }
}

void Tiles::setColor(const QColor &color){
    tile_color = color;
}
void Tiles::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(tile_type == "NULL")
        return;
    // 发出信号，告知是哪tile被点击了
    emit BlockClicked(index_x, index_y);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Tiles::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(tile_type == "NULL")
        return;
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Tiles::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(tile_type == "NULL")
        return;
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Tiles::updateTilesNameVisibleStatus(bool status) {
    tiles_name_visible_status = status;
    update();
}


