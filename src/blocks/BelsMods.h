//
// Created by DELL on 2024/12/24.
//

#ifndef HYBRDLINK_BELSMODS_H
#define HYBRDLINK_BELSMODS_H

#include "BelsBlock.h"

class BelsPad: public BelsBlock {
public:
    BelsPad(const QColor &color,int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 300, 300, tile_index_x, tile_index_y, site_index, cur_name, "PAD", bel_index) {}
};

class BelsInbufEn: public BelsBlock {
public:
    BelsInbufEn(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
        : BelsBlock(color, 300, 300, tile_index_x, tile_index_y, site_index, cur_name, "INBUF_EN", bel_index) {}
};

class BelsOutbuf: public BelsBlock {
public:
    BelsOutbuf(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 300, 300, tile_index_x, tile_index_y, site_index, cur_name, "OUTBUF", bel_index) {}
};

class BelsLUT6: public BelsBlock {
public:
    BelsLUT6(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 100, 120, tile_index_x, tile_index_y, site_index, cur_name, "LUT6", bel_index) {}
};

class BelsSELMUX2_1: public BelsBlock {
public:
    BelsSELMUX2_1(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 100, 80, tile_index_x, tile_index_y, site_index, cur_name, "SELMUX2_1", bel_index) {}
};

class BelsCarry4: public BelsBlock {
public:
    BelsCarry4(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 250, 620, tile_index_x, tile_index_y, site_index, cur_name, "CARRY4", bel_index) {}
};

class BelsRegInit: public BelsBlock {
public:
    BelsRegInit(const QColor &color, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, int bel_index)
            : BelsBlock(color, 60, 60, tile_index_x, tile_index_y, site_index, cur_name, "REG_INIT", bel_index) {}
};

#endif //HYBRDLINK_BELSMODS_H
