#include "SitesIOB33.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "base/Globals.h"
#include "Belsmods.h"

SitesIOB33::SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
    :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33", cur_name, site_index) {
    child_bel_items.append(new BelsPad(color, site_index, cur_name+"/PAD", 0));
    child_bel_items.append(new BelsInbufEn(color, site_index, cur_name+"/INBUF_EN", 1));
    child_bel_items.append(new BelsOutbuf(color, site_index, cur_name+"/OUTBUF", 2));
    for(BelsBlock* bel_item : child_bel_items) {
        bel_item->setParentItem(this);
    }
    child_bel_items[0]->setPos(QPointF(300, 50));
    child_bel_items[1]->setPos(QPointF(120, 400));
    child_bel_items[2]->setPos(QPointF(480, 400));
}
//SitesIOB33::SitesIOB33(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index)
//    : site_width(cur_width), site_height(cur_height), tile_index_x(tile_index_x), tile_index_y(tile_index_y), site_type(cur_type), site_name(cur_name), site_index(site_index){
//    this->site_color = color;
//    setFlags(ItemIsSelectable);
//    // 开启悬浮操作
//    setAcceptHoverEvents(true);
//    //    setVisible(false); // 默认隐藏
//    child_bel_items.append(new BelsPad(color, site_index, cur_name+"/PAD", 0));
//    child_bel_items.append(new BelsInbufEn(color, site_index, cur_name+"/INBUF_EN", 1));
//    child_bel_items.append(new BelsOutbuf(color, site_index, cur_name+"/OUTBUF", 2));
//    for(BelsBlock* bel_item : child_bel_items) {
//        bel_item->setParentItem(this);
//    }
//
//    child_bel_items[0]->setPos(QPointF(300, 50));
//    child_bel_items[1]->setPos(QPointF(120, 400));
//    child_bel_items[2]->setPos(QPointF(480, 400));
//}
//
//QRectF SitesIOB33::boundingRect() const
//{
//    return QRectF(0, 0, site_width, site_height);
//}
//
//QPainterPath SitesIOB33::shape() const
//{
//    QPainterPath path;
//    path.addRect(0, 0, site_width, site_height);
//    return path;
//}
//
//void SitesIOB33::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(widget);
//
//    if (!sites_visible_status){
//        return;
//    }
//    QColor fillColor = (option->state & QStyle::State_Selected) ? site_color.darker(150) : site_color;
//    if (option->state & QStyle::State_MouseOver){
//        // 鼠标悬浮高亮
//        fillColor = fillColor.lighter(125);
//    }
//
//    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
//    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
//    if (lod < 0.001) {
//        painter->fillRect(QRectF(0, 0, site_width, site_height), fillColor);
//        return;
//    }
//
//    QPen oldPen = painter->pen();
//    QPen pen = oldPen;
//    int width = 0;
//    if (option->state & QStyle::State_Selected)
//        width += 2;
//
//    pen.setWidth(width);
//    QBrush b = painter->brush();
//    // 设置点击控件后使其变暗
//    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));
//    // 显示位置及大小
//    painter->drawRect(QRect(0, 0,  site_width, site_height));
//    // ------------------------------------------------------------
//    //painter->drawRect(QRect(30, 5, 30, 30));    //BEL_PAD
//    //painter->drawRect(QRect(12, 40, 30, 30));   //BEL_INBUF_EN
//    //painter->drawRect(QRect(48, 40, 30, 30));   //BEL_OUTPUTBUF
//    // ------------------------------------------------------------
//    painter->setBrush(b);
//
//
//    if (lod >= 0.05 && sites_visible_status) {
//        QFont font("Times", 50);
//        font.setStyleStrategy(QFont::ForceOutline);
//        painter->setFont(font);
//        painter->save();
//        // painter->drawText(QRect(0, 0, site_width, 25), Qt::AlignCenter, QString(QString::fromStdString(site_type)));
//        painter->drawText(QRect(20, 0, site_width, 800), Qt::AlignLeft | Qt::AlignBottom, QString(QString::fromStdString(site_type)));
//        painter->restore();
//    }
//}
//
//void SitesIOB33::setColor(const QColor &color){
//    site_color = color;
//    for(BelsBlock* bel_item : child_bel_items) {
//        bel_item->setColor(color);
//    }
//}
//
//std::string SitesIOB33::getSiteName() const
//{
//    return this->site_name;
//}
//
//void SitesIOB33::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    emit SiteClicked(tile_index_x,tile_index_y,sites_visible_status,site_index);
//    QGraphicsItem::mousePressEvent(event);
//    update();
//}
//
//void SitesIOB33::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if (event->modifiers() & Qt::ShiftModifier) {
//        update();
//        return;
//    }
//    QGraphicsItem::mouseMoveEvent(event);
//}
//
//void SitesIOB33::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mouseReleaseEvent(event);
//    update();
//}
//
//void SitesIOB33::updateSitesVisibleStatus(bool status) {
//    sites_visible_status = status;
//    for(BelsBlock* bel_item : child_bel_items) {
//        bel_item->updateVisibleStatus(status);
//    }
//    update();
//}

