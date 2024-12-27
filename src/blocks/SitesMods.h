//
// Created by DELL on 2024/12/26.
//

#ifndef HYBRDLINK_SITESMODS_H
#define HYBRDLINK_SITESMODS_H

#include <QColor>
#include "base/Globals.h"
#include <QGraphicsItem>
#include "SitesBlock.h"

class Sites : public SitesBlock {
public:
    Sites(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SITES", cur_name, site_index) {}
};

class SitesBUFIO : public SitesBlock {
public:
    SitesBUFIO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock (
                    color,
                    (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
                    (GLOBAL_TILE_BLOCK_HEIGHT-(5 * SITE_GAP)) / 4,
                    tile_index_x, tile_index_y, "BUFIO",
                    cur_name, site_index
                    ) {}
};

class SitesBUFR : public SitesBlock {
public:
    SitesBUFR(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
                    color,
                    (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
                    (GLOBAL_TILE_BLOCK_HEIGHT-(5 * SITE_GAP)) / 4,
                    tile_index_x, tile_index_y,
                    "BUFR", cur_name, site_index
                    ) {}
};

class SitesIDELAYCTRL : public SitesBlock {
public:
    SitesIDELAYCTRL(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock (
                    color,
                    (GLOBAL_TILE_BLOCK_WIDTH-(4 * SITE_GAP)) / 3,
                    (GLOBAL_TILE_BLOCK_HEIGHT-(3 * SITE_GAP)) / 2,
                    tile_index_x, tile_index_y,
                    "BUFR", cur_name, site_index
                    ) {}
};

class SitesOLOGICE3 : public SitesBlock {
public:
    SitesOLOGICE3(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
                    color,
                    GLOBAL_SITE_BLOCK_WIDTH,
                    (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
                    tile_index_x, tile_index_y,
                    "OLOGICE3", cur_name, site_index
                    ) {}
};

class SitesILOGICE3 : public SitesBlock {
public:
    SitesILOGICE3(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
                color,
                GLOBAL_SITE_BLOCK_WIDTH,
                (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
                tile_index_x, tile_index_y,
                "ILOGICE3",  cur_name, site_index
                ) {}
};

class SitesIDELAYE2 : public SitesBlock {
public:
    SitesIDELAYE2(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
                color,
                GLOBAL_SITE_BLOCK_WIDTH,
                (GLOBAL_TILE_BLOCK_HEIGHT - (3 * SITE_GAP))/2,
                tile_index_x, tile_index_y,
                "IDELAYE2", cur_name, site_index
                ) {}
};

class SitesRAMB18E1 : public SitesBlock {
public:
    SitesRAMB18E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 4 * SITE_GAP,
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (5 * SITE_GAP))/2,
            tile_index_x, tile_index_y,
            "RAMB18E1", cur_name, site_index
    ) {}
};

class SitesFIFO18E1 : public SitesBlock {
public:
    SitesFIFO18E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 4 * SITE_GAP,
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (5 * SITE_GAP))/2,
            tile_index_x, tile_index_y,
            "FIFO18E1", cur_name, site_index
    ) {}
};

class SitesRAMB36E1 : public SitesBlock {
public:
    SitesRAMB36E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * SITE_GAP,
            (GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * SITE_GAP),
            tile_index_x, tile_index_y,
            "RAMB36E1", cur_name, site_index
    ) {}
};

class SitesDSP48E1 : public SitesBlock {
public:
    SitesDSP48E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * SITE_GAP,
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (3 * SITE_GAP))/2,
            tile_index_x, tile_index_y,
            "DSP48E1", cur_name, site_index
    ) {}
};

class SitesIOB33 : public SitesBlock {
public:
    SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesSliceL : public SitesBlock {
public:
    SitesSliceL(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesSliceM : public SitesBlock {
public:
    SitesSliceM(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

#endif //HYBRDLINK_SITESMODS_H
