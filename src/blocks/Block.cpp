//
// Created by DELL on 2024/12/24.
//

#include "Block.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Block::Block(int cur_width, int cur_height, const QColor &color)
    : width(cur_width), height(cur_height) {
    this->block_color = color;
    setFlag(ItemIsSelectable);
    //开启悬浮操作
    setAcceptHoverEvents(true);
}

QRectF Block::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

QPainterPath Block::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width, height);
    return path;
}

void Block::setColor(const QColor &color) {
    block_color = color;
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);

    Q_UNUSED(widget);

    if (!visible_status){
        return;
    }

    QColor fillColor = (option->state & QStyle::State_Selected) ? block_color.darker(150) : block_color;
    if (option->state & QStyle::State_MouseOver){
        // 鼠标悬浮高亮
        fillColor = fillColor.lighter(125);
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if(showThumbnail(painter, lod, fillColor))
        return;

    QBrush b = painter->brush();
    // 设置点击控件后使其变暗
    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->save();

    showComplete(painter, lod, fillColor);

    painter->restore();

    painter->setBrush(b);
}

void Block::updateVisibleStatus(bool status) {
    visible_status = status;
    update();
}

std::string Block::getName() const {
    return this->name;
}

int Block::getWidth() const {
    return this->width;
}

int Block::getHeight() const {
    return this->height;
}
