//
// Created by DELL on 2024/12/24.
//

#include "Block.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <unordered_map>
#include <typeindex>


Block::Block(int cur_width, int cur_height, const std::string &name, const QColor &color)
    : width(cur_width), height(cur_height), name(name) {
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

    if (!getVisibleStatus()){
        return;
    }

    QPen pen = painter->pen();
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    // 计算缩放等级，画面越小,细节越少，lod越接近于0; 画面越大，细节越多，lod越趋近于1
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if(showThumbnail(painter, lod, block_color))
        return;

    painter->save();

    pen.setColor(block_color);
    painter->setPen(pen);

    if(used_status)
        painter->setBrush(QColor(QColor(0, 255, 0, 127))); //若资源使用，标记为绿色

    painter->save();

    showComplete(painter, lod, block_color);

    painter->restore();

    //点击选中显示
    if (option->state & QStyle::State_Selected) {
        pen.setWidth(4);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->setBrush(QColor(100, 100, 100, 127));
        painter->drawRect(QRect(0, 0, width, height));
    }
    painter->restore();
}

void Block::setUsed() {
    used_status = true;
}

std::string Block::getName() const {
//    qDebug() << QString::fromStdString(this->name);
    return this->name;
}

int Block::getWidth() const {
    return this->width;
}

int Block::getHeight() const {
    return this->height;
}
