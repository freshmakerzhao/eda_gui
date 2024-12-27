//
// Created by DELL on 2024/12/23.
//

#include "BelsBlock.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

BelsBlock::BelsBlock(const QColor &color, int cur_width, int cur_height, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, const std::string &bel_type, int bel_index)
    : Block(cur_width, cur_height, color), tile_index_x(tile_index_x), tile_index_y(tile_index_y), site_index(site_index), name(cur_name), bel_type(bel_type) {

}

bool BelsBlock::showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(lod >= 0.001) return false;
        painter->drawRect(QRect(0, 0, width, height));
    return true;
}

void BelsBlock::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    painter->drawRect(QRect(0, 0, width, height));
    if(lod >= 0.2 && visible_status) {

        QFont font("Times");
        font.setPointSizeF(width/bel_type.size());
        painter->setFont(font);
        painter->drawText(QRect(0, 0, width, 50), Qt::AlignCenter, QString(QString::fromStdString(bel_type)));

    }
}

void BelsBlock::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit BelClicked(tile_index_x, tile_index_y, site_index, visible_status, index);
}

void BelsBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void BelsBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
