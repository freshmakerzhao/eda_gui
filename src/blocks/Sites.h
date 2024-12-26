#ifndef SITES_H
#define SITES_H

#include <QColor>
#include "base/Globals.h"
#include <QGraphicsItem>
#include "SitesBlock.h"

class Sites : public SitesBlock {
public:
    Sites(const QColor &color, int tile_index_x, int tile_index_y, std::string &cur_name, int site_index)
            :SitesBlock(color, GLOBAL_SITE_BLOCK_WIDTH, GLOBAL_SITE_BLOCK_HEIGHT, tile_index_x, tile_index_y, "SITES", cur_name, site_index) {}
};
//class Sites : public SitesBlock
//{
//public:
//    // 颜色、名称中的坐标，宽度、高度、类型、在当前tile中的索引
//    Sites(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index);
//    QRectF boundingRect() const override;
//    QPainterPath shape() const override;
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
//    void updateSitesVisibleStatus(bool status) override;
//    void setColor(const QColor &color) override;
//    virtual std::string getSiteName() const override;
//protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
//
//private:
//    int site_width;
//    int site_height;
//    int site_index; // 当前site在tile中的索引
//    QColor site_color;
//    std::string site_type;
//    std::string site_name;
//    int tile_index_x;
//    int tile_index_y;
//    bool sites_visible_status = false;
//};

#endif // SITES_H
