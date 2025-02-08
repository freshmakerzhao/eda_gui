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
                    "IDELAYCTRL", cur_name, site_index
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
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP) - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)) - (3 * (3 * SITE_GAP)))/2,
            tile_index_x, tile_index_y,
            "RAMB18E1", cur_name, site_index
    ) {}
};

class SitesFIFO18E1 : public SitesBlock {
public:
    SitesFIFO18E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * ( 3 * SITE_GAP) - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)) - (3 * (3 * SITE_GAP)))/2,
            tile_index_x, tile_index_y,
            "FIFO18E1", cur_name, site_index
    ) {}
};

class SitesRAMB36E1 : public SitesBlock {
public:
    SitesRAMB36E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            (GLOBAL_TILE_BLOCK_HEIGHT * 5) - (2 * (3 * SITE_GAP)),
            tile_index_x, tile_index_y,
            "RAMB36E1", cur_name, site_index
    ) {}
};

class SitesDSP48E1 : public SitesBlock {
public:
    SitesDSP48E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 5) - (3 * (3 * SITE_GAP)))/2,
            tile_index_x, tile_index_y,
            "DSP48E1", cur_name, site_index
    ) {}
};

class SitesIOB33 : public SitesBlock {
public:
    SitesIOB33(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name);
private:
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    std::string pin_name;
};

class SitesIOB33S : public SitesBlock {
public:
    SitesIOB33S(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name);
private:
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    std::string pin_name;
};

class SitesIOB33M : public SitesBlock {
public:
    SitesIOB33M(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index, std::string &pin_name);
private:
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    std::string pin_name;
};

class SitesSliceL : public SitesBlock {
public:
    SitesSliceL(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesSliceM : public SitesBlock {
public:
    SitesSliceM(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesOUTFIFO : public SitesBlock {
public:
    SitesOUTFIFO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 12) - (3 * (3 * SITE_GAP))) / 2,
            tile_index_x, tile_index_y,
            "OUT_FIFO", cur_name, site_index
    ) {}
};

class SitesINFIFO : public SitesBlock {
public:
    SitesINFIFO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 12) - (3 * (3 * SITE_GAP))) / 2,
            tile_index_x, tile_index_y,
            "OUT_FIFO", cur_name, site_index
    ) {}
};

class SitesMMCME2ADV : public SitesBlock {
public:
    SitesMMCME2ADV(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            (GLOBAL_TILE_BLOCK_HEIGHT * 16) - (2 * (3 * SITE_GAP)),
            tile_index_x, tile_index_y,
            "MMCEM2_ADV", cur_name, site_index
    ) {}
};

class SitesPLLE2ADV : public SitesBlock {
public:
    SitesPLLE2ADV(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            (GLOBAL_TILE_BLOCK_HEIGHT * 13) - (2 * (3 * SITE_GAP)),
            tile_index_x, tile_index_y,
            "MMCEM2_ADV", cur_name, site_index
    ) {}
};

class SitesPhaserOUTPHY : public SitesBlock {
public:
    SitesPhaserOUTPHY (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
            tile_index_x, tile_index_y,
            "PHASER_OUT_PHY", cur_name, site_index
    ) {}
};

class SitesPhaserINPHY : public SitesBlock {
public:
    SitesPhaserINPHY(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
            tile_index_x, tile_index_y,
            "PHASER_IN_PHY", cur_name, site_index
    ) {}
};

class SitesPHYControl : public SitesBlock {
public:
    SitesPHYControl(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            ((GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * (3 * SITE_GAP)))/4,
            tile_index_x, tile_index_y,
            "PHY_CONTROL", cur_name, site_index
    ) {}
};

class SitesPhaserREF : public SitesBlock {
public:
    SitesPhaserREF(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(
            color,
            GLOBAL_TILE_BLOCK_WIDTH - 2 * (3 * SITE_GAP),
            (GLOBAL_TILE_BLOCK_HEIGHT * 9) - (5 * 4650) - (2 * (3 * SITE_GAP)),
            tile_index_x, tile_index_y,
            "PHY_CONTROL", cur_name, site_index
    ) {}
};
#endif //HYBRDLINK_SITESMODS_H
