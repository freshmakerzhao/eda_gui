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
    BelsBlock(const QColor &color, int cur_width, int cur_height, int site_index, const std::string &cur_name, const std::string &bel_type, int bel_index);
    std::string getBelName() const;
signals:
    void BelClicked(int site_index, bool bel_visible_status, int bel_index);
private:
    bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
protected:
    int site_index;
    int index;
    std::string bel_type;
    std::string name;
};


#endif //HYBRDLINK_BELSBLOCK_H
