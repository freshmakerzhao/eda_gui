// Created by DELL on 2024/12/26.
//

#include "SitesMods.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include "BelsMods.h"
#include "grid/ChipGridOperations.h"

SitesIOB33::SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index, std::string &pin_name)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33", cur_name, site_index), pin_name(pin_name) {
    this->setPos(QPointF(600, SITE_GAP));
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

SitesIOB33S::SitesIOB33S(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index, std::string &pin_name)
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
    this->setPos(QPointF(600, SITE_GAP*(site_index+1) + height*site_index));
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
SitesIOB33M::SitesIOB33M(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index, std::string &pin_name)
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
    this->setPos(QPointF(600, SITE_GAP * (site_index + 1) + height * site_index));
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
SitesSliceL::SitesSliceL(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SLICEL", cur_name, site_index) {
    this->setPos(QPointF(SITE_GAP*(site_index+1) + GLOBAL_SITE_BLOCK_WIDTH*site_index, SITE_GAP));

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

SitesSliceM::SitesSliceM(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SLICEM", cur_name, site_index) {
    this->setPos(QPointF(SITE_GAP*(site_index+1) + GLOBAL_SITE_BLOCK_WIDTH*site_index, SITE_GAP));
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

SitesBUFIO::SitesBUFIO(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock (
        color,
        (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
        (GLOBAL_TILE_BLOCK_HEIGHT-(5 * SITE_GAP)) / 4,
        tile_index_x, tile_index_y, "BUFIO",
        cur_name, site_index
) {
    this->setPos(QPointF(
            SITE_GAP*(site_index/4+1) + width*(site_index/4),
            SITE_GAP*(site_index%4+1) + height*(site_index%4)
    ));
}

SitesBUFR::SitesBUFR(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
        (GLOBAL_TILE_BLOCK_HEIGHT-(5 * SITE_GAP)) / 4,
        tile_index_x, tile_index_y,
        "BUFR", cur_name, site_index
) {
    this->setPos(QPointF(
            SITE_GAP*(site_index/4+1) + width*(site_index/4),
            SITE_GAP*(site_index%4+1) + height*(site_index%4)
    ));
}

SitesIDELAYCTRL::SitesIDELAYCTRL(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock (
        color,
        (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
        (GLOBAL_TILE_BLOCK_HEIGHT-(3 * SITE_GAP)) / 2,
        tile_index_x, tile_index_y,
        "IDELAYCTRL", cur_name, site_index
) {
    this->setPos(QPointF(
            SITE_GAP*(site_index/4+1) + width*(site_index/4),
            SITE_GAP*(site_index%3+1) + height*(site_index%4)
    ));
}

SitesOLOGICE3::SitesOLOGICE3(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
        tile_index_x, tile_index_y,
        "OLOGICE3", cur_name, site_index
) {
    if (tile.is_multi_rows) {
        this->setPos(QPointF (
                SITE_GAP*(site_index/4+1) + this->getWidth()*(site_index/4),
                SITE_GAP*(site_index%4+1) + this->getHeight()*(site_index%4)
        )); //由于这里是跨行，所以位置需要重新设置一次
    } else {
        this->setPos(QPointF (
                SITE_GAP*(site_index/2+1) + width*(site_index/2),
                SITE_GAP*(site_index%2+1) + height*(site_index%2)
        ));
    }
}

SitesILOGICE3::SitesILOGICE3(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
        tile_index_x, tile_index_y,
        "ILOGICE3",  cur_name, site_index
) {
    if (tile.is_multi_rows) {
        this->setPos(QPointF(
                SITE_GAP * (site_index / 4 + 1) + this->getWidth() * (site_index / 4),
                SITE_GAP * (site_index % 4 + 1) + this->getHeight() * (site_index % 4)
        )); //由于这里是跨行，所以位置需要重新设置一次
    } else {
        this->setPos ( QPointF(
                SITE_GAP*(site_index/2+1) + width*(site_index/2),
                SITE_GAP*(site_index%2+1) + height*(site_index%2)
        ));
    }
}

SitesIDELAYE2::SitesIDELAYE2(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
        tile_index_x, tile_index_y,
        "IDELAYE2", cur_name, site_index
) {
    if (tile.is_multi_rows) {
        this->setPos ( QPointF(
                SITE_GAP*(site_index/4+1) + this->getWidth()*(site_index/4),
                SITE_GAP*(site_index%4+1) + this->getHeight()*(site_index%4)
        ));//由于这里是跨行，所以位置需要重新设置一次
    } else {
        this->setPos ( QPointF(
                SITE_GAP*(site_index/2+1) + width*(site_index/2),
                SITE_GAP*(site_index%2+1) + height*(site_index%2)
        ));
    }
}

SitesRAMB18E1::SitesRAMB18E1(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP) - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)) - (3 * (3 * SITE_GAP)))/2,
        tile_index_x, tile_index_y,
        "RAMB18E1", cur_name, site_index
) {
    this->setPos(QPointF(2 * (3 * SITE_GAP), (3 * SITE_GAP)*(site_index+2) + height*site_index));
}

SitesFIFO18E1::SitesFIFO18E1(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * ( 3 * SITE_GAP) - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)) - (3 * (3 * SITE_GAP)))/2,
        tile_index_x, tile_index_y,
        "FIFO18E1", cur_name, site_index
) {
    this->setPos(QPointF(2 * (3 * SITE_GAP), (3 * SITE_GAP)*(site_index+2) + height*site_index));
}

SitesRAMB36E1::SitesRAMB36E1(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        (GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)),
        tile_index_x, tile_index_y,
        "RAMB36E1", cur_name, site_index
) {
    this->setPos(QPointF(3 * SITE_GAP, 3 * SITE_GAP));
}

SitesDSP48E1::SitesDSP48E1(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (3 * (3 * SITE_GAP)))/2,
        tile_index_x, tile_index_y,
        "DSP48E1", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + height*site_index));
}

SitesOUTFIFO::SitesOUTFIFO(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 12) - (3 * (3 * SITE_GAP))) / 2,
        tile_index_x, tile_index_y,
        "OUT_FIFO", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + height*site_index));
}

SitesINFIFO::SitesINFIFO(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 12) - (3 * (3 * SITE_GAP))) / 2,
        tile_index_x, tile_index_y,
        "OUT_FIFO", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + height*site_index));
}

SitesMMCME2ADV::SitesMMCME2ADV(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        (GLOBAL_TILE_BLOCK_HEIGHT * 16) - (2 * (3 * SITE_GAP)),
        tile_index_x, tile_index_y,
        "MMCEM2_ADV", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + height*site_index));
}

SitesPLLE2ADV::SitesPLLE2ADV(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        (GLOBAL_TILE_BLOCK_HEIGHT * 13) - (2 * (3 * SITE_GAP)),
        tile_index_x, tile_index_y,
        "PLLE2_ADV", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + this->getHeight()*site_index));
}

SitesPhaserOUTPHY::SitesPhaserOUTPHY (const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
        tile_index_x, tile_index_y,
        "PHASER_OUT_PHY", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + this->getHeight()*site_index));
}

SitesPhaserINPHY::SitesPhaserINPHY(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
        tile_index_x, tile_index_y,
        "PHASER_IN_PHY", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index+1) + this->getHeight()*site_index));
}

SitesPHYControl::SitesPHYControl(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
        tile_index_x, tile_index_y,
        "PHY_CONTROL", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP)*(site_index) + this->getHeight()*(site_index-1)));
}

SitesPhaserREF::SitesPhaserREF(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        (GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * 4650) - (2 * (3 * SITE_GAP)),
        tile_index_x, tile_index_y,
        "PHY_CONTROL", cur_name, site_index
) {
    this->setPos(QPointF((3 * SITE_GAP), (3 * SITE_GAP) + 4650*(site_index+2)));
}

SitesGTP2Cannel::SitesGTP2Cannel(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (GLOBAL_TILE_BLOCK_HEIGHT * 11) - 2 * SITE_GAP,
        tile_index_x, tile_index_y,
        "GTPE2_CHANNEL", cur_name, site_index
) {
    this->setPos(QPointF(SITE_GAP, SITE_GAP));
}
SitesIPAD::SitesIPAD(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index, std::string &pin)
        : SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        GLOBAL_SITE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "IPAD", cur_name, site_index
), pin_name(pin) {
    if(tile.types == "GTP_COMMON") {
        this->setPos(
            (GLOBAL_TILE_BLOCK_WIDTH)/2 + SITE_GAP,
            (site_index-2) * SITE_GAP + (site_index-3) * height + 3*GLOBAL_TILE_BLOCK_HEIGHT
        );
    } else {
        this->setPos(
            (GLOBAL_TILE_BLOCK_WIDTH)/2 + SITE_GAP,
            ((11*GLOBAL_TILE_BLOCK_HEIGHT - (4*height + 3 * 3*SITE_GAP))/2) + (site_index-1)*height + site_index*3*SITE_GAP
        );
    }
}
void SitesIPAD::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
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
SitesOPAD::SitesOPAD(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index, std::string &pin)
        : SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        GLOBAL_SITE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "OPAD", cur_name, site_index
), pin_name(pin) {
    this->setPos(
            (GLOBAL_TILE_BLOCK_WIDTH)/2 + SITE_GAP,
            ((11*GLOBAL_TILE_BLOCK_HEIGHT - (4*height + 3 * 3*SITE_GAP))/2) + (site_index-1)*height + site_index*3*SITE_GAP
    );
}
void SitesOPAD::showComplete(QPainter *painter, const qreal lod, QColor &fillColor) {
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
SitesGTP2Common::SitesGTP2Common(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (GLOBAL_TILE_BLOCK_HEIGHT * 7) - 2 * SITE_GAP,
        tile_index_x, tile_index_y,
        "GTPE2_COMMON", cur_name, site_index
) {
    this->setPos(QPointF(SITE_GAP*(site_index+1) + GLOBAL_SITE_BLOCK_WIDTH*site_index, SITE_GAP));
}
SitesIBufsGTE2::SitesIBufsGTE2(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
: SitesBlock(
        color,
        GLOBAL_SITE_BLOCK_WIDTH,
        (3 * GLOBAL_TILE_BLOCK_HEIGHT - SITE_GAP)/2 - SITE_GAP ,
        tile_index_x, tile_index_y,
        "IBUFDS_GTE2", cur_name, site_index
) {
    this->setPos(
            (GLOBAL_TILE_BLOCK_WIDTH)/2 + SITE_GAP,
            site_index * SITE_GAP + (site_index-1) * height
    );
}
SitesBUFGCTRL::SitesBUFGCTRL(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH/5,
        (4*GLOBAL_TILE_BLOCK_HEIGHT-SITE_GAP/2)/16-SITE_GAP/2,
        tile_index_x, tile_index_y,
        "BUFGCTRL", cur_name, site_index
) {
    this->setPos((GLOBAL_TILE_BLOCK_WIDTH-width)/2, (site_index + 1) * SITE_GAP/2 + site_index * height);
}

SitesBUFHCE::SitesBUFHCE(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        (GLOBAL_TILE_BLOCK_WIDTH - 3*SITE_GAP)/2 - 3*SITE_GAP,
        (8*GLOBAL_TILE_BLOCK_HEIGHT-3*SITE_GAP)/12 - 3*SITE_GAP,
        tile_index_x, tile_index_y,
        "SitesBUFHCE", cur_name, site_index
) {
    this->setPos(
            (site_index/12 + 1) * 3*SITE_GAP + (site_index/12) * width,
            (site_index%12 + 1) * 3*SITE_GAP + site_index%12 * height
    );
}

SitesPCIE2_1::SitesPCIE2_1(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        (3*GLOBAL_TILE_BLOCK_WIDTH - 2 * 3*SITE_GAP),
        (20*GLOBAL_TILE_BLOCK_HEIGHT - 2*SITE_GAP),
        tile_index_x, tile_index_y,
        "PCIE_2_1", cur_name, site_index
) {
    this->setPos(
        3*SITE_GAP,
        SITE_GAP
    );
}

SitesBUFMRCE::SitesBUFMRCE(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        (GLOBAL_TILE_BLOCK_WIDTH - 2*SITE_GAP),
        (GLOBAL_TILE_BLOCK_HEIGHT - SITE_GAP)/2 - SITE_GAP,
        tile_index_x, tile_index_y,
        "PCIE_2_1", cur_name, site_index
) {
    this->setPos(
        SITE_GAP,
        site_index*height + (site_index+1)*SITE_GAP
    );
}

SitesEFUSEUSR::SitesEFUSEUSR(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        3*GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "EFUSE_USR", cur_name, site_index
) {
    const int gap = 4*GLOBAL_TILE_BLOCK_HEIGHT/3;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap
    );
}

SitesDNAPort::SitesDNAPort(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        3*GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "DNA_PORT", cur_name, site_index
) {
    const int gap = 4*GLOBAL_TILE_BLOCK_HEIGHT/3;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap
    );
}

SitesICAP::SitesICAP(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "ICAP", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesBSCAN::SitesBSCAN(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "BSCAN", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesDCIRESET::SitesDCIRESET(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "DCIRESET", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesCAPTURE::SitesCAPTURE(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "CAPTURE", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesSTARTUP::SitesSTARTUP(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "STARTUP", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesFRAMEECC::SitesFRAMEECC(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "FRAME_ECC", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesUSRACCESS::SitesUSRACCESS(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        : SitesBlock(
        color,
        GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
        GLOBAL_TILE_BLOCK_HEIGHT,
        tile_index_x, tile_index_y,
        "USR_ACCESS", cur_name, site_index
) {
    const int gap = 3*SITE_GAP;
    this->setPos(
            3*SITE_GAP,
            site_index*height + (site_index+1)*gap + GLOBAL_TILE_BLOCK_HEIGHT
    );
}
SitesNull::SitesNull(const QColor &color, int tile_index_x, int tile_index_y, NormalTile tile, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SITESNULL", cur_name, site_index) {
    //不显示
}
