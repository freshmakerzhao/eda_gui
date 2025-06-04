#ifndef NormalBlock_H
#define NormalBlock_H

#include "Blocks.h"
#include <QColor>
#include <QGraphicsItem>
#include "Sites.h"
#include "base/Globals.h"

class Tiles : public Blocks
{
public:
    Tiles(const QColor &color, int loc_x, int loc_y, int index_x, int index_y, int cur_width, int cur_height, std::string &cur_type);

    void addSubBlock(SitesBlock *subBlock) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void updateTilesNameVisibleStatus(bool status) override;
    void setColor(const QColor &color) override;

    TILE getRectangle2D() override {
        QPointF scenePos = this->mapToScene(0, 0);
        return TILE(scenePos.x(), scenePos.y(), tile_width, tile_height);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int tile_width;
    int tile_height;
    std::string tile_type;
    int loc_x;
    int loc_y;
    int index_x;
    int index_y;
    QColor tile_color;
    bool tiles_name_visible_status = true;

};

#endif // NormalBlock_H
