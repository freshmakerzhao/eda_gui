/**
  ******************************************************************************
  * @file           : SitesBlock.h
  * @author         : zs
  * @description      : None
  * @date           : 1/12/2023
  ******************************************************************************
  */

#ifndef SITESBLOCK_H
#define SITESBLOCK_H


#include <QGraphicsItem>
#include <unordered_set>
#include "BelsBlock.h"
#include "Block.h"

class SitesBlock : public Block {
Q_OBJECT
public:
    SitesBlock(const QColor &color, int cur_width, int cur_height, int tile_index_x, int tile_index_y, const std::string &site_type, const std::string &cur_name, int site_index);
    QVector<BelsBlock*> child_bel_items;
    void updateVisibleStatus(bool status) override;
    void setUsed(std::unordered_set<std::string> bels);
signals:
    void SiteClicked(int tile_index_x, int tile_index_y,bool sites_visible_status,int index);
private:
    bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
protected:
    int tile_index_x;
    int tile_index_y;
    int site_index;
    std::string site_type;
    std::string name;
};


#endif //SITESBLOCK_H
