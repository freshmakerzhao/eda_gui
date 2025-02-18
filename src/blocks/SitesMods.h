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
    Sites(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesBUFIO : public SitesBlock {
public:
    SitesBUFIO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesBUFR : public SitesBlock {
public:
    SitesBUFR(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesIDELAYCTRL : public SitesBlock {
public:
    SitesIDELAYCTRL(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesOLOGICE3 : public SitesBlock {
public:
    SitesOLOGICE3(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesILOGICE3 : public SitesBlock {
public:
    SitesILOGICE3(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesIDELAYE2 : public SitesBlock {
public:
    SitesIDELAYE2(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesRAMB18E1 : public SitesBlock {
public:
    SitesRAMB18E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesFIFO18E1 : public SitesBlock {
public:
    SitesFIFO18E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesRAMB36E1 : public SitesBlock {
public:
    SitesRAMB36E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesDSP48E1 : public SitesBlock {
public:
    SitesDSP48E1(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
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
    SitesOUTFIFO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesINFIFO : public SitesBlock {
public:
    SitesINFIFO(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesMMCME2ADV : public SitesBlock {
public:
    SitesMMCME2ADV(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPLLE2ADV : public SitesBlock {
public:
    SitesPLLE2ADV(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPhaserOUTPHY : public SitesBlock {
public:
    SitesPhaserOUTPHY (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPhaserINPHY : public SitesBlock {
public:
    SitesPhaserINPHY(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPHYControl : public SitesBlock {
public:
    SitesPHYControl(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPhaserREF : public SitesBlock {
public:
    SitesPhaserREF(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesGTP2Cannel : public SitesBlock {
public:
    SitesGTP2Cannel(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesIPAD : public SitesBlock {
public:
    SitesIPAD(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesOPAD : public SitesBlock {
public:
    SitesOPAD(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesGTP2Common : public SitesBlock {
public:
    SitesGTP2Common(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesIBufsGTE2 : public SitesBlock {
public:
    SitesIBufsGTE2(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesBUFHCE : public SitesBlock {
public:
    SitesBUFHCE (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesBUFGCTRL : public SitesBlock {
public:
    SitesBUFGCTRL (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesPCIE2_1 : public SitesBlock {
public:
    SitesPCIE2_1 (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};

class SitesBUFMRCE : public SitesBlock {
public:
    SitesBUFMRCE  (const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index);
};
#endif //HYBRDLINK_SITESMODS_H
