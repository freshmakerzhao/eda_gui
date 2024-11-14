#ifndef SITESBUFIO_H
#define SITESBUFIO_H

#include <QColor>
#include <QGraphicsItem>
#include "SitesBlock.h"

class SitesBUFIO : public SitesBlock
{
public:
    // 颜色、名称中的坐标，宽度、高度、类型、在当前tile中的索引
    SitesBUFIO(const QColor &color, int cur_width, int cur_height, int tile_index_x,int tile_index_y,std::string &cur_type, std::string &cur_name, int site_index);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void updateSitesVisibleStatus(bool status) override;
    void setColor(const QColor &color) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int site_width;
    int site_height;
    int site_index; // 当前site在tile中的索引
    QColor site_color;
    std::string site_type;
    std::string site_name;
    int tile_index_x;
    int tile_index_y;
    bool sites_visible_status = false;
};

#endif // SITESBUFIO_H
