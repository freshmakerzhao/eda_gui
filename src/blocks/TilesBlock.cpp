#include "TilesBlock.h"
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <utility>
#include "blocks/SitesBlockFactory.h"

TilesBlock::TilesBlock(
        const QColor &color,
        int index_x,
        int index_y,
        bool show,
        NormalTile &info
) : Block(info.width, info.height, info.tile_name, color),
    index_x(index_x),
    index_y(index_y),
    show(show),
    info(info){
    setFlags(ItemIsSelectable);
    // 开启悬浮操作
    setAcceptHoverEvents(true);
}

std::string TilesBlock::getType() const {
    return info.types;
}

// 添加子模块
void TilesBlock::addSubBlock(SitesBlock *subBlock) {
    subBlock->setParentItem(this);
    child_items.append(subBlock);
}

bool TilesBlock::showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) {
    return false;
}

void TilesBlock::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(!show)
        return;
    if(!getVisibleStatus())
        return;

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);


    painter->drawRect(QRect(0, 0, width, height));
    //获取当前视图的缩放比例
    QGraphicsView *view = scene()->views().first();
    qreal scaleFactor = view->transform().m11();

    //保存当前的变换状态
    QTransform originalTransform = painter->transform();

//     使用反向缩放因子，确保文本大小不受缩放影响
    painter->scale(1 / scaleFactor, 1 / scaleFactor);
    QFont font("Times");
    font.setPointSize(type_font_size);
    painter->setFont(font);

    pen.setColor(Qt::blue);
    painter->setPen(pen);
    QString text = QString::fromStdString(this->name + " (" + info.types + ")");
    int text_len = text.size() * type_font_size;
    if (text_len <= width * scaleFactor) {
        QRectF text_rect = QRect(2, 2, width, 20);
        painter->drawText(
                text_rect,
                Qt::AlignLeft | Qt::AlignTop,
                text
        );
    }
}

void TilesBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(info.types == "NULL")
        return;
    // 发出信号，告知是哪tile被点击了
    emit BlockClicked(index_x, index_y);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void TilesBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(info.types == "NULL")
        return;
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void TilesBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(info.types == "NULL")
        return;
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

bool TilesBlock::getVisibleStatus() {
    return true;
}

void TilesBlock::updateVisibleStatus(bool status) {
}
bool TilesBlock::isShow() const {
    return show;
}
