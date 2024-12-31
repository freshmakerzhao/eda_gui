//
// Created by DELL on 2024/12/26.
//

#include "SitesMods.h"
#include "BelsMods.h"

SitesIOB33::SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
        :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "IOB33", cur_name, site_index) {
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
