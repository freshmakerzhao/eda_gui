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

class SitesBlock : public QGraphicsObject {
Q_OBJECT
public:
    virtual void updateSitesVisibleStatus(bool status){}
    virtual void setColor(const QColor &color){}
signals:
    void SiteClicked(int tile_index_x, int tile_index_y,bool sites_visible_status,int index);
};


#endif //SITESBLOCK_H
