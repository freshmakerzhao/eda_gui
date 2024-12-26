#include "SitesSliceM.h"
#include "base/Globals.h"
#include "Belsmods.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

SitesSliceM::SitesSliceM(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SitesSliceM", cur_name, site_index) {
    child_bel_items.append(new BelsLUT6(color, site_index, cur_name+"/A6LUT", 0));
    child_bel_items.append(new BelsLUT6(color, site_index, cur_name+"/B6LUT", 1));
    child_bel_items.append(new BelsLUT6(color, site_index, cur_name+"/C6LUT", 2));
    child_bel_items.append(new BelsLUT6(color, site_index, cur_name+"/D6LUT", 3));

    child_bel_items.append(new BelsSELMUX2_1(color, site_index, cur_name+"/F7AMUX", 4));
    child_bel_items.append(new BelsSELMUX2_1(color, site_index, cur_name+"/F8AMUX", 5));
    child_bel_items.append(new BelsSELMUX2_1(color, site_index, cur_name+"/F7BMUX", 6));

    child_bel_items.append(new BelsCarry4(color, site_index, cur_name+"/CARRY4", 7));

    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/D5FF", 8));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/DFF", 9));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/C5FF", 10));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/CFF", 11));

    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/B5FF", 12));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/BFF", 13));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/A5FF", 14));
    child_bel_items.append(new BelsRegInit(color, site_index, cur_name+"/AFF", 15));

    for(BelsBlock* bel_item : child_bel_items) {
        bel_item->setParentItem(this);
    }

    int count = 0;
    for(int i=0; i<4; i++)
        child_bel_items[i + count]->setPos(QPointF(50, 40+i*160));
    count += 4;
    for(int i=0; i<3; i++)
        child_bel_items[i + count]->setPos(QPointF(250, 80+i*200));
    count += 3;
    child_bel_items[count]->setPos(QPointF(400, 40));
    count ++;
    for(int i=0; i<4; i++)
        child_bel_items[i + count]->setPos(QPointF(800, 20+i*80));
    count += 4;
    for(int i=0; i<4; i++)
        child_bel_items[i + count]->setPos(QPointF(800, 380+i*80));
    setAcceptHoverEvents(true);
}
//SitesSliceM::SitesSliceM(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index)
//    : site_width(cur_width), site_height(cur_height), tile_index_x(tile_index_x), tile_index_y(tile_index_y), site_type(cur_type), site_name(cur_name), site_index(site_index){
//    this->site_color = color;
//    setFlags(ItemIsSelectable);
//    // 开启悬浮操作
//    setAcceptHoverEvents(true);
//    //    setVisible(false); // 默认隐藏
//    // 创建多个子项
//    //    QGraphicsItem* childItem1 = new QGraphicsEllipseItem(0, 0, 50, 50, this);
//    //    QGraphicsItem* childItem2 = new QGraphicsRectItem(60, 0, 50, 50, this);
//
//    // 为子项设置可交互性
//    //    childItem1->setFlag(QGraphicsItem::ItemIsSelectable);
//    //    childItem1->setFlag(QGraphicsItem::ItemIsMovable);
//    //    childItem2->setFlag(QGraphicsItem::ItemIsSelectable);
//    //    childItem2->setFlag(QGraphicsItem::ItemIsMovable);
//}
//
//QRectF SitesSliceM::boundingRect() const
//{
//    return QRectF(0, 0, site_width, site_height);
//}
//
//QPainterPath SitesSliceM::shape() const
//{
//    QPainterPath path;
//    path.addRect(0, 0, site_width, site_height);
//    return path;
//}
//
//void SitesSliceM::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
//    if (lod < 0.125) {
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
//    painter->drawRect(QRect(5, 4, 10, 12));
//    painter->drawRect(QRect(5, 20, 10, 12));
//    painter->drawRect(QRect(5, 36, 10, 12));
//    painter->drawRect(QRect(5, 52, 10, 12));
//
//    painter->drawRect(QRect(25, 8, 5, 50));
//
//    painter->drawRect(QRect(40, 4, 25, 62));
//
//    painter->drawRect(QRect(80, 2, 6, 6));
//    painter->drawRect(QRect(80, 10, 6, 6));
//    painter->drawRect(QRect(80, 18, 6, 6));
//    painter->drawRect(QRect(80, 26, 6, 6));
//
//    painter->drawRect(QRect(80, 36, 6, 6));
//    painter->drawRect(QRect(80, 44, 6, 6));
//    painter->drawRect(QRect(80, 52, 6, 6));
//    painter->drawRect(QRect(80, 60, 6, 6));
//    // ------------------------------------------------------------
//    painter->setBrush(b);
//
//
//    if (lod >= 0.5 && sites_visible_status) {
//        QFont font("Times", 5);
//        font.setStyleStrategy(QFont::ForceOutline);
//        painter->setFont(font);
//        painter->save();
//        // painter->drawText(QRect(0, 0, site_width, 25), Qt::AlignCenter, QString(QString::fromStdString(site_type)));
//        painter->drawText(QRect(2, 0, site_width, 80), Qt::AlignLeft | Qt::AlignBottom, QString(QString::fromStdString(site_type)));
//        painter->restore();
//    }
//}
//
//void SitesSliceM::setColor(const QColor &color){
//    site_color = color;
//}
//
//std::string SitesSliceM::getSiteName() const
//{
//    return this->site_name;
//}
//
//void SitesSliceM::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    emit SiteClicked(tile_index_x,tile_index_y,sites_visible_status,site_index);
//    QGraphicsItem::mousePressEvent(event);
//    update();
//}
//
//void SitesSliceM::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if (event->modifiers() & Qt::ShiftModifier) {
//        update();
//        return;
//    }
//    QGraphicsItem::mouseMoveEvent(event);
//}
//
//void SitesSliceM::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mouseReleaseEvent(event);
//    update();
//}
//
//void SitesSliceM::updateSitesVisibleStatus(bool status) {
//    sites_visible_status = status;
//    update();
//}
