//
// Created by DELL on 2024/12/26.
//

#include "SitesMods.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include "BelsMods.h"

SitesIOB33::SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33", cur_name, site_index), pin_name(pin_name) {
    child_bel_items.append(new BelsPad(color, tile_index_x, tile_index_y, site_index, cur_name+"/PAD", 0));
    child_bel_items.append(new BelsInbufEn(color, tile_index_x, tile_index_y, site_index, cur_name+"/INBUF_EN", 1));
    child_bel_items.append(new BelsOutbuf(color, tile_index_x, tile_index_y, site_index, cur_name+"/OUTBUF", 2));
    for(BelsBlock* bel_item : child_bel_items) {
        bel_item->setParentItem(this);
    }
    child_bel_items[0]->setPos(QPointF(300, 50));
    child_bel_items[1]->setPos(QPointF(120, 400));
    child_bel_items[2]->setPos(QPointF(480, 400));
}

void SitesIOB33::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(!getVisibleStatus())
        return;

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    if (lod >= 0.05 && !child_bel_items.isEmpty()) {
        setBelShow(true);
        painter->setBrush(Qt::NoBrush);
    } else {
        setBelShow(false);
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

    QString text = QString::fromStdString(this->name + " (" + site_type + ")");
    int text_len = text.size() * type_font_size;
    if (text_len <= width * scaleFactor) {
        QRectF text_rect = QRect(2, height * scaleFactor - type_font_size - 6, width, 20);
        painter->drawText(
                text_rect,
                Qt::AlignLeft | Qt::AlignTop,
                text
            );
    }
    int pin_name_len = pin_name.size() * type_font_size;
    if (pin_name_len <= width / 2 * scaleFactor) {
        QRectF pin_rect = QRect(2, 2, width, 20);
        painter->drawText (
                pin_rect,
                Qt::AlignLeft | Qt::AlignTop,
                QString::fromStdString(pin_name)
        );
    }
}

SitesIOB33S::SitesIOB33S(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33S", cur_name, site_index), pin_name(pin_name) {
    child_bel_items.append(new BelsPad(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33/PAD", 0));
    child_bel_items.append(new BelsInbufEn(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33/INBUF_EN", 1));
    child_bel_items.append(new BelsOutbuf(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33/OUTBUF", 2));
    for(BelsBlock* bel_item : child_bel_items) {
        bel_item->setParentItem(this);
    }
    child_bel_items[0]->setPos(QPointF(300, 50));
    child_bel_items[1]->setPos(QPointF(120, 400));
    child_bel_items[2]->setPos(QPointF(480, 400));
}

void SitesIOB33S::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(!getVisibleStatus())
        return;

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    if (lod >= 0.05 && !child_bel_items.isEmpty()) {
        setBelShow(true);
        painter->setBrush(Qt::NoBrush);
    } else {
        setBelShow(false);
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

    QString text = QString::fromStdString(this->name + " (" + site_type + ")");
    int text_len = text.size() * type_font_size;
    if (text_len <= width * scaleFactor) {
        QRectF text_rect = QRect(2, height * scaleFactor - type_font_size - 6, width, 20);
        painter->drawText(
                text_rect,
                Qt::AlignLeft | Qt::AlignTop,
                text
        );
    }
    int pin_name_len = pin_name.size() * type_font_size;
    if (pin_name_len <= width / 2 * scaleFactor) {
        QRectF pin_rect = QRect(2, 2, width, 20);
        painter->drawText (
                pin_rect,
                Qt::AlignLeft | Qt::AlignTop,
                QString::fromStdString(pin_name)
        );
    }
}
SitesIOB33M::SitesIOB33M(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33M", cur_name, site_index), pin_name(pin_name) {
    child_bel_items.append(new BelsPad(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33M/PAD", 0));
    child_bel_items.append(new BelsInbufEn(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33M/INBUF_EN", 1));
    child_bel_items.append(new BelsOutbuf(color, tile_index_x, tile_index_y, site_index, cur_name+"/IOB33M/OUTBUF", 2));
    for(BelsBlock* bel_item : child_bel_items) {
        bel_item->setParentItem(this);
    }
    child_bel_items[0]->setPos(QPointF(300, 50));
    child_bel_items[1]->setPos(QPointF(120, 400));
    child_bel_items[2]->setPos(QPointF(480, 400));
}

void SitesIOB33M::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
    if(!getVisibleStatus())
        return;

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    if (lod >= 0.05 && !child_bel_items.isEmpty()) {
        setBelShow(true);
        painter->setBrush(Qt::NoBrush);
    } else {
        setBelShow(false);
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

    QString text = QString::fromStdString(this->name + " (" + site_type + ")");
    int text_len = text.size() * type_font_size;
    if (text_len <= width * scaleFactor) {
        QRectF text_rect = QRect(2, height * scaleFactor - type_font_size - 6, width, 20);
        painter->drawText(
                text_rect,
                Qt::AlignLeft | Qt::AlignTop,
                text
        );
    }
    int pin_name_len = pin_name.size() * type_font_size;
    if (pin_name_len <= width / 2 * scaleFactor) {
        QRectF pin_rect = QRect(2, 2, width, 20);
        painter->drawText (
                pin_rect,
                Qt::AlignLeft | Qt::AlignTop,
                QString::fromStdString(pin_name)
        );
    }
}
SitesSliceL::SitesSliceL(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SLICEL", cur_name, site_index) {
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/A6LUT", 0));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/B6LUT", 1));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/C6LUT", 2));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/D6LUT", 3));

    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F7AMUX", 4));
    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F8AMUX", 5));
    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F7BMUX", 6));

    child_bel_items.append(new BelsCarry4(color, tile_index_x, tile_index_y, site_index, cur_name+"/CARRY4", 7));

    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/D5FF", 8));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/DFF", 9));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/C5FF", 10));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/CFF", 11));

    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/B5FF", 12));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/BFF", 13));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/A5FF", 14));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/AFF", 15));

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

SitesSliceM::SitesSliceM(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SLICEM", cur_name, site_index) {
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/A6LUT", 0));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/B6LUT", 1));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/C6LUT", 2));
    child_bel_items.append(new BelsLUT6(color, tile_index_x, tile_index_y, site_index, cur_name+"/D6LUT", 3));

    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F7AMUX", 4));
    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F8AMUX", 5));
    child_bel_items.append(new BelsSELMUX2_1(color, tile_index_x, tile_index_y, site_index, cur_name+"/F7BMUX", 6));

    child_bel_items.append(new BelsCarry4(color, tile_index_x, tile_index_y, site_index, cur_name+"/CARRY4", 7));

    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/D5FF", 8));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/DFF", 9));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/C5FF", 10));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/CFF", 11));

    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/B5FF", 12));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/BFF", 13));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/A5FF", 14));
    child_bel_items.append(new BelsRegInit(color, tile_index_x, tile_index_y, site_index, cur_name+"/AFF", 15));

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
