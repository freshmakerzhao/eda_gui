#ifndef SITE_H
#define SITE_H

#include <QPainter>
#include <QColor>
#include <string>

class Site {
public:
    Site(const QColor &color, int width, int height, int x, int y, const std::string &type, const std::string &name, int index)
        : site_color(color), site_width(width), site_height(height), site_x(x), site_y(y), site_type(type), site_name(name), site_index(index) {
        lineVertices = {
            {0.0f, 0.0f},
            {static_cast<qreal>(site_width), 0.0f},
            {static_cast<qreal>(site_width), static_cast<qreal>(height)},
            {0.0f, static_cast<qreal>(site_height)}
        };
    }

    const std::vector<QPointF>& getLineVertices() const { return lineVertices; }

    void setPos(float x, float y) {
        xPos = x;
        yPos = y;
    }

    // 获取矩形的属性
    QColor color() const { return site_color; }
    int width() const { return site_width; }
    int height() const { return site_height; }
    int x() const { return site_x; }
    int y() const { return site_y; }
    std::string type() const { return site_type; }
    std::string name() const { return site_name; }
    int index() const { return site_index; }

    // 设置矩形的颜色
    void setColor(const QColor &color) { site_color = color; }

private:
    QColor site_color;
    int site_width;
    int site_height;
    int site_x;
    int site_y;
    std::string site_type;
    std::string site_name;
    int site_index;

    float xPos, yPos;

    std::vector<QPointF> lineVertices;
};

#endif // SITE_H
