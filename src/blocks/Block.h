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
    Block(int cur_width, int cur_height, const std::string &name,const QColor &color);
    void setColor(const QColor &color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    virtual void updateVisibleStatus(bool status);
    virtual void setUsed();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    std::string getName() const;
    int getWidth() const;
    int getHeight() const;
    void setTypeShow(const bool option);
    void setTypeFontSize(const int size);
protected:
    int width;
    int height;
    QColor block_color;
    std::string name;

    bool show_type = true; //是否显示type字符
    int type_font_size = 12; //显示type字符的大小
    bool visible_status = false;
    bool used_status = false;
    virtual bool showThumbnail(QPainter *painter, const qreal lod, QColor &fillColor) = 0;
    virtual void showComplete(QPainter *painter, const qreal lod, QColor &fillColor) = 0;
};


#endif //HYBRDLINK_BLOCK_H
