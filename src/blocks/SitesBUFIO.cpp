#include "SitesBUFIO.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

SitesBUFIO::SitesBUFIO(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index)
    : site_width(cur_width), site_height(cur_height), tile_index_x(tile_index_x), tile_index_y(tile_index_y), site_type(cur_type), site_name(cur_name), site_index(site_index){
    this->site_color = color;
    setFlags(ItemIsSelectable);
    // 开启悬浮操作
    setAcceptHoverEvents(true);
    //    setVisible(false); // 默认隐藏
    // 创建多个子项
    //    QGraphicsItem* childItem1 = new QGraphicsEllipseItem(0, 0, 50, 50, this);
    //    QGraphicsItem* childItem2 = new QGraphicsRectItem(60, 0, 50, 50, this);

    // 为子项设置可交互性
    //    childItem1->setFlag(QGraphicsItem::ItemIsSelectable);
    //    childItem1->setFlag(QGraphicsItem::ItemIsMovable);
    //    childItem2->setFlag(QGraphicsItem::ItemIsSelectable);
    //    childItem2->setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF SitesBUFIO::boundingRect() const
{
    return QRectF(0, 0, site_width, site_height);
}

QPainterPath SitesBUFIO::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, site_width, site_height);
    return path;
}

void SitesBUFIO::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
        QFont font("Times", 5);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        // painter->drawText(QRect(0, 0, site_width, 25), Qt::AlignCenter, QString(QString::fromStdString(site_type)));
        painter->drawText(QRect(2, 0, site_width, 80), Qt::AlignLeft | Qt::AlignBottom, QString(QString::fromStdString(site_type)));
        painter->restore();
    }
}

void SitesBUFIO::setColor(const QColor &color){
    site_color = color;
}

void SitesBUFIO::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit SiteClicked(tile_index_x,tile_index_y,sites_visible_status,site_index);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void SitesBUFIO::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void SitesBUFIO::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void SitesBUFIO::updateSitesVisibleStatus(bool status) {
    sites_visible_status = status;
    update();
}


