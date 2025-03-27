#ifndef NormalBlock_H
#define NormalBlock_H

#include "SitesBlock.h"
#include "Block.h"
#include <QColor>
#include <QGraphicsItem>

class TilesBlock : public Block
{
    Q_OBJECT
public:
    TilesBlock(
        const QColor &color,
        int index_x, int index_y,
        bool show,
        NormalTile& info
    );

    void updateVisibleStatus(bool status) override;
    void addSubBlock(SitesBlock *subBlock);
    bool isShow() const;
    std::string getType() const;
    QVector<SitesBlock*> child_items;
    NormalTile& info;
signals:
    void BlockClicked(int tile_index_x, int tile_index_y);
protected:
    bool getVisibleStatus() override;
    bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int index_x;
    int index_y;
    bool show;

};

#endif // NormalBlock_H
