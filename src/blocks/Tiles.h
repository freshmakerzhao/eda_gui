#ifndef NormalBlock_H
#define NormalBlock_H

#include "Blocks.h"
#include <QColor>
#include <QGraphicsItem>

class Tiles : public Block
{
    Q_OBJECT
public:
    Tiles(const QColor &color, int loc_x, int loc_y, int index_x, int index_y, int cur_width, int cur_height, std::string &cur_type, std::string &name);

    void addSubBlock(SitesBlock *subBlock);
    void updateTilesNameVisibleStatus(bool status);
    void updateVisibleStatus(bool status) override;
    QVector<SitesBlock*> child_items;
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
    std::string tile_type;
    int loc_x;
    int loc_y;
    int index_x;
    int index_y;
    QColor tile_color;
    bool tiles_name_visible_status = false;

};

#endif // NormalBlock_H
