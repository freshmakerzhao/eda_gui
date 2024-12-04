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

protected:
    // 缩放等级
    const qreal SITE_LEVEL1 = 0.125;
    const qreal SITE_LEVEL2 = 0.5;
    const qreal SITE_LEVEL3 = 1.0;

signals:
    void SiteClicked(int tile_index_x, int tile_index_y,bool sites_visible_status,int index);
};


#endif //SITESBLOCK_H
