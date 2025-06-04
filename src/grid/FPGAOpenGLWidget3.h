#ifndef FPGAOPENGLWIDGET3_H
#define FPGAOPENGLWIDGET3_H

#include <QDebug>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QToolTip>
#include <QVector>
#include <QMessageBox>
#include "base/Globals.h"
#include "qdatetime.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class FPGAOpenGLWidget3 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    FPGAOpenGLWidget3(QWidget *parent = nullptr);

    ~FPGAOpenGLWidget3();

    void updateTiles(std::vector<TILE> &tiles);
    void updateSites(std::vector<SITE> &sites);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    // void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void paintTiles();
    void paintSites();

    // 新增现代OpenGL相关成员
    QOpenGLShaderProgram* m_shaderProgram;
    QOpenGLBuffer m_tileVbo;
    QOpenGLBuffer m_siteVbo;
    QOpenGLVertexArrayObject m_tileVao;
    QOpenGLVertexArrayObject m_siteVao;

    // 顶点数据结构
    struct VertexData {
        QVector2D position;
        QVector3D color;
    };

    QVector<VertexData> tileVertices;
    QVector<VertexData> siteVertices;

private:
    float _scaleFactor;    // 缩放因子
    const float MINIMUM_SCALEFACTOR = 0.035f;
    float _offsetX = 0, _offsetY = 0; // 视图偏移量

    // const float offset = 0.1f;

    bool isDragging; // 是否正在拖动
    QPoint lastMousePos; // 上次鼠标位置

    std::vector<TILE> _tiles;

    std::vector<SITE> _sites;

    std::map<std::string, SITE> _used_sites;

    // const int _scale = 1.0f;


    // -------------------------------------------
    // 视图矩阵相关
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view; // 未利用
    QMatrix4x4 m_model;
    float m_zoomFactor = 1.0f;
    QPoint m_lastMousePos;
    QVector2D m_translation;
};

#endif // FPGAOPENGLWIDGET3_H
