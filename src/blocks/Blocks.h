/**
  ******************************************************************************
  * @File           : Blocks.h
  * @Author         : zs
  * @Description    : None
  * @Date           : 23-11-30
  ******************************************************************************
  */

#ifndef BLOCKS_H
#define BLOCKS_H
#include "base/Globals.h"
#include <QGraphicsItem>
#include <blocks/Sites.h>

class Blocks : public QGraphicsObject {
Q_OBJECT
public:
    virtual void addSubBlock(SitesBlock *subBlock){}
    virtual void updateTilesNameVisibleStatus(bool status){}
    virtual void setColor(const QColor &color){}
    virtual TILE getRectangle2D() = 0;
    QVector<SitesBlock*> child_items;
signals:
    void BlockClicked(int x, int y);
};


#endif //BLOCKS_H
