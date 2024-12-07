#include "Sites.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Sites::Sites(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index)
    : site_width(cur_width), site_height(cur_height), tile_index_x(tile_index_x), tile_index_y(tile_index_y), site_type(cur_type), site_name(cur_name), site_index(site_index){
    this->site_color = color;
    setFlags(ItemIsSelectable);
    // 开启悬浮操作
    setAcceptHoverEvents(true);
//    setVisible(false); // 默认隐藏
}

QRectF Sites::boundingRect() const
{
    return QRectF(0, 0, site_width, site_height);
}

QPainterPath Sites::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, site_width, site_height);
    return path;
}

void Sites::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (!sites_visible_status){
        return;
    }
    QColor fillColor = (option->state & QStyle::State_Selected) ? site_color.darker(150) : site_color;
    if (option->state & QStyle::State_MouseOver){
        // 鼠标悬浮高亮
        fillColor = fillColor.lighter(125);
    }

    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.125) {
        painter->fillRect(QRectF(0, 0, site_width, site_height), fillColor);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    // 设置点击控件后使其变暗
    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));
    // 显示位置及大小
    painter->drawRect(QRect(0, 0,  site_width, site_height));
    painter->setBrush(b);


    if (lod >= 0.5 && sites_visible_status) {
        QFont font("Times", 9);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->drawText(QRect(0, 0, site_width, 25), Qt::AlignCenter, QString(QString::fromStdString(site_type)));
        painter->restore();
    }
}

void Sites::setColor(const QColor &color){
    site_color = color;
}

std::string Sites::getSiteName() const
{
    return this->site_name;
}

void Sites::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit SiteClicked(tile_index_x,tile_index_y,sites_visible_status,site_index);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Sites::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Sites::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Sites::updateSitesVisibleStatus(bool status) {
    sites_visible_status = status;
    update();
}


