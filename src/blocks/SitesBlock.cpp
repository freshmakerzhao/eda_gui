/**
  ******************************************************************************
  * @file           : SitesBlock.cpp
  * @author         : zs
  * @description      : None
  * @date           : 1/12/2023
  ******************************************************************************
  */

#include "SitesBlock.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

SitesBlock::SitesBlock(const QColor &color, int cur_width, int cur_height, int tile_index_x, int tile_index_y, const std::string &site_type, std::string &cur_name, int site_index)
: Block(cur_width, cur_height, color), tile_index_x(tile_index_x), tile_index_y(tile_index_y),  site_type(site_type), name(cur_name), site_index(site_index) {}

bool SitesBlock::showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(lod >= 0.02) return false;
    //painter->fillRect(QRectF(0, 0, width, height), fillColor);
    return true;
}

void SitesBlock::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    painter->drawRect(QRect(0, 0, width, height));

    if(lod >= 0.06 && visible_status) {
        for (BelsBlock *bel_item: child_bel_items) {
            bel_item->updateVisibleStatus(true);
        }
        QFont font("Times");
        font.setPointSizeF(50);
        painter->setFont(font);
        painter->drawText(QRect(20, 0, width, 800), Qt::AlignLeft | Qt::AlignBottom,
                          QString(QString::fromStdString(site_type)));
    } else {
        for (BelsBlock *bel_item: child_bel_items) {
            bel_item->updateVisibleStatus(false);
        }
    }
}

void SitesBlock::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit SiteClicked(tile_index_x, tile_index_y, visible_status, site_index);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void SitesBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void SitesBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void SitesBlock::updateVisibleStatus(bool status) {
    visible_status = status;
    if(!status) {
        for(BelsBlock* bel_item : child_bel_items) {
            bel_item->updateVisibleStatus(false);
        }
    }
    update();
}
