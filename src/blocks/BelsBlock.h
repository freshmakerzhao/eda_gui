//
// Created by DELL on 2024/12/23.
//

#ifndef HYBRDLINK_BELSBLOCK_H
#define HYBRDLINK_BELSBLOCK_H

#include <QColor>
#include "Block.h"


class BelsBlock : public Block {
Q_OBJECT
public:
    BelsBlock(const QColor &color, int cur_width, int cur_height, int tile_index_x, int tile_index_y, int site_index, const std::string &cur_name, const std::string &bel_type, int bel_index);
    bool isMatches(const std::string &bel_name);
    void updateVisibleStatus(bool option) override;
    void launchClicked();
    bool isUsed();
signals:
    void BelClicked(int tile_index_x, int tile_index_y, int site_index, bool bel_visible_status, int bel_index, const std::string &bel_type, const std::string &name);
private:
    bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    bool getVisibleStatus() override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
protected:
    int tile_index_x;
    int tile_index_y;
    int site_index;
    int index;
    static bool bel_visible_status;
    std::string bel_type;
};


#endif //HYBRDLINK_BELSBLOCK_H
