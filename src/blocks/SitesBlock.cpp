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
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>

SitesBlock::SitesBlock(const QColor &color, int cur_width, int cur_height, int tile_index_x, int tile_index_y, const std::string &site_type, const std::string &cur_name, int site_index)
: Block(cur_width, cur_height, cur_name, color), tile_index_x(tile_index_x), tile_index_y(tile_index_y),  site_type(site_type),  site_index(site_index) {}

bool SitesBlock::showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(lod >= 0.01) return false;
    if(used_status) {
        painter->fillRect(QRectF(0, 0, width, height), QColor(Qt::green));
    }
    for (auto bel_item: child_bel_items) {
        bel_item->updateVisibleStatus(false);
    }
    return true;
}

void SitesBlock::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(!visible_status)
        return;

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);


    if (lod >= 0.05) {
        for (auto bel_item: child_bel_items) {
            bel_item->updateVisibleStatus(true);
        }
        painter->setBrush(Qt::NoBrush);
    } else {
        for (auto bel_item: child_bel_items) {
            bel_item->updateVisibleStatus(false);
        }
    }

    painter->drawRect(QRect(0, 0, width, height));

    //获取当前视图的缩放比例
    QGraphicsView *view = scene()->views().first();
    qreal scaleFactor = view->transform().m11();

    //保存当前的变换状态
    QTransform originalTransform = painter->transform();

    // 使用反向缩放因子，确保文本大小不受缩放影响
    painter->scale(1 / scaleFactor, 1 / scaleFactor);
    QFont font("Times");
    font.setPointSize(type_font_size);
    painter->setFont(font);

    int text_len = site_type.size() * type_font_size;
    if (text_len <= width * scaleFactor) {
        QRectF text_rect = QRect(2, height * scaleFactor - type_font_size - 6, width, 20);
        painter->drawText(
                text_rect,
                Qt::AlignLeft | Qt::AlignTop,
                QString(QString::fromStdString(site_type))
        );
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

void SitesBlock::setUsed(std::unordered_set<std::string> bels) {
    used_status = true;
    for(auto bel_name : bels){
        for(auto bel : this->child_bel_items) {
            if(bel->getName() == bel_name)
                bel->setUsed();
        }
    }
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
