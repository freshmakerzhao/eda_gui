//
// Created by DELL on 2024/12/23.
//

#include "BelsBlock.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsView>
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
    if(visible_status) {

        if(!show_type)
            return;

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

        //当字符串的宽度小于等于width时，才显示
        int text_len = bel_type.size() * type_font_size;
        if (text_len <= width * scaleFactor) {
            QRectF text_rect = QRect(2, height * scaleFactor - type_font_size - 6, text_len, type_font_size * 2);
            painter->drawText(text_rect, Qt::AlignLeft | Qt::AlignTop, QString(QString::fromStdString(bel_type)));
        }
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
