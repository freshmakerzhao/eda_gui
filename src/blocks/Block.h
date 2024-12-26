//
// Created by DELL on 2024/12/24.
//

#ifndef HYBRDLINK_BLOCK_H
#define HYBRDLINK_BLOCK_H
#include <QGraphicsItem>
#include <QColor>

class Block : public QGraphicsObject {
Q_OBJECT
public:
    Block(int cur_width, int cur_height, const QColor &color);
    void setColor(const QColor &color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    virtual void updateVisibleStatus(bool status);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    std::string getName() const;
protected:
    int width;
    int height;
    QColor block_color;
    std::string name;
    bool visible_status = false;
    virtual bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor){}
    virtual void showComplete(QPainter *painter, const qreal lod, QColor &fillColor){}
};


#endif //HYBRDLINK_BLOCK_H
