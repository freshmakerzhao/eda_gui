#include "FPGAOpenGLWidget3.h"
#include "qtimer.h"

FPGAOpenGLWidget3::FPGAOpenGLWidget3(QWidget *parent)
    : QOpenGLWidget(parent), _scaleFactor(0.1f) {
    setMouseTracking(true);
    setStyleSheet(
        "QToolTip {"
        "   background-color: #333333;"       // 背景色
        "   color: #C5C5C5;"                    // 文字颜色
        "   border: 1px solid #777777;"
        "   border-radius: 0px;"
        "}"
        );
    isDragging = false;

    // QSurfaceFormat format;
    // format.setVersion(3, 3);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // setFormat(format);

    // QTimer::singleShot(0, this, [this](){ update(); }); // 强制触发一次绘制
}

FPGAOpenGLWidget3::~FPGAOpenGLWidget3()
{
    makeCurrent();
    m_tileVbo.destroy();
    m_siteVbo.destroy();
    m_tileVao.destroy();
    m_siteVao.destroy();
    delete m_shaderProgram;
    doneCurrent();
}

void FPGAOpenGLWidget3::updateTiles(std::vector<TILE> &tiles)
{
    // qDebug() << "update Tiles...";

    _tiles = tiles;

    // 更新Tile顶点数据
    // QVector<VertexData> tileVertices;
    // for (auto& tile : _tiles) {
    // QVector3D color(48/255.0, 48/255.0, 48/255.0);
    QVector3D color(48/255.0, 48/255.0, 48/255.0);

    for(const auto& tile : _tiles) {
        // 每个tile的四个顶点（会自动闭合）
        tileVertices.append({{tile.topLeft.x, tile.topLeft.y}, color});
        tileVertices.append({{tile.topRight.x, tile.topRight.y}, color});
        tileVertices.append({{tile.bottomRight.x, tile.bottomRight.y}, color});
        tileVertices.append({{tile.bottomLeft.x, tile.bottomLeft.y}, color});
    }



    // m_tileVbo.bind();
    // if(m_tileVbo.size() < tileVertices.size() * sizeof(VertexData)) {
    //     // 仅当空间不足时重新分配
    //     m_tileVbo.allocate(tileVertices.constData(),
    //                        tileVertices.size() * sizeof(VertexData));
    // } else {
    //     // 空间足够则直接写入
    //     m_tileVbo.write(0, tileVertices.constData(),
    //                     tileVertices.size() * sizeof(VertexData));
    // }

    update();
}

void FPGAOpenGLWidget3::updateSites(std::vector<SITE> &sites)
{
    _sites = sites;
    // QVector3D color(1, 1, 1);
    QVector3D color(48/255.0, 48/255.0, 48/255.0);
    for(const auto& site : _sites) {
        siteVertices.append({{site.topLeft.x, site.topLeft.y}, color});
        siteVertices.append({{site.topRight.x, site.topRight.y}, color});
        siteVertices.append({{site.bottomRight.x, site.bottomRight.y}, color});
        siteVertices.append({{site.bottomLeft.x, site.bottomLeft.y}, color});
    }

    update();
}

void FPGAOpenGLWidget3::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << "OpenGL driver info:";
    qDebug() << "  Vendor:" << (const char*)glGetString(GL_VENDOR);
    qDebug() << "  Renderer:" << (const char*)glGetString(GL_RENDERER);
    qDebug() << "  Version:" << (const char*)glGetString(GL_VERSION);

    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QSurfaceFormat format = ctx->format();
    qDebug() << "OpenGL version:" << format.majorVersion() << "." << format.minorVersion();

    // QSurfaceFormat format = context()->format();
    // if (format.version() < qMakePair(3, 3)) {
    //     qWarning() << "Requires OpenGL 3.3 or higher";
    //     QMessageBox::critical(this, "Error", "This application requires OpenGL 3.3 or higher");
    // }

    // 1. 创建着色器程序
    m_shaderProgram = new QOpenGLShaderProgram(this);
    // m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
    //                                          "#version 330 core\n"
    //                                          "layout(location = 0) in vec2 position;\n"
    //                                          "layout(location = 1) in vec3 color;\n"
    //                                          "uniform mat4 modelViewProjection;\n"
    //                                          "out vec3 vColor;\n"
    //                                          "void main() {\n"
    //                                          "   gl_Position = modelViewProjection * vec4(position, 0.0, 1.0);\n"
    //                                          "   vColor = color;\n"
    //                                          "}");

    // m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
    //                                          "#version 330 core\n"
    //                                          "in vec3 vColor;\n"
    //                                          "out vec4 fragColor;\n"
    //                                          "void main() {\n"
    //                                          "   fragColor = vec4(vColor, 1.0);\n"
    //                                          "}");


    const bool isGLES = QOpenGLContext::currentContext()->isOpenGLES();
    qDebug("Using %s", isGLES ? "OpenGL ES" : "Desktop OpenGL");

    // GLSL版本和精度声明模板
    const QString versionHeader = isGLES ?
                                      QString("#version 100\n"
                                              "precision mediump float;\n") :  // ES必须声明精度
                                      QString("#version 120\n");

    // 顶点着色器模板
    const QString vertexShader = QString("%1"
                                         "attribute vec2 position;\n"
                                         "attribute vec3 color;\n"
                                         // "uniform mat4 modelViewProjection;\n"
                                         "uniform mat4 projection;"
                                         "uniform mat4 view;"
                                         "uniform mat4 model;"
                                         "varying vec3 vColor;\n"
                                         "void main() {\n"
                                         // "   gl_Position = modelViewProjection * vec4(position, 0.0, 1.0);\n"
                                         "   gl_Position = projection * view * model * vec4(position, 0.0, 1.0);\n"
                                         "   vColor = color;\n"
                                         "}").arg(versionHeader);

    // 片段着色器模板
    const QString fragmentShader = QString("%1"
                                            "varying vec3 vColor;\n"
                                           "void main() {\n"
                                           "   gl_FragColor = vec4(vColor, 1.0);\n"
                                           "}").arg(versionHeader);

    // 加载着色器
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);

    m_shaderProgram->link();

    // 2. 初始化Tile的VAO/VBO
    m_tileVao.create();
    m_tileVao.bind();

    m_tileVbo.create();
    m_tileVbo.bind();
    m_tileVbo.allocate(1000000 * sizeof(VertexData)); // 预分配空间

    // 配置Tile的顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));

    // 2. 解除Tile的VAO绑定
    m_tileVao.release();

    // 3. 初始化Site的VAO/VBO
    m_siteVao.create();
    m_siteVao.bind();

    m_siteVbo.create();
    m_siteVbo.bind();
    m_siteVbo.allocate(1000000 * sizeof(VertexData));

    // 重新配置Site的顶点属性（即使相同也需要重新设置）
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));

    // 4. 解除Site的VAO绑定
    m_siteVao.release();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 初始化视图矩阵
    m_projection.setToIdentity();
    m_view.setToIdentity();
    m_model.setToIdentity();
}

void FPGAOpenGLWidget3::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram->bind();

    // 设置变换矩阵
    m_shaderProgram->setUniformValue("projection", m_projection);
    m_shaderProgram->setUniformValue("view", m_view);

    // 应用缩放和平移
    QMatrix4x4 model;
    model.translate(m_translation.x(), m_translation.y());
    model.scale(m_zoomFactor);
    m_shaderProgram->setUniformValue("model", model);

    paintTiles();
    paintSites();

    // m_shaderProgram->release();

    glFinish();  // 确保命令执行完毕
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
        qDebug() << "OpenGL error:" << err;

    update();
}

void FPGAOpenGLWidget3::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projection.setToIdentity();
    // m_projection.ortho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -1.0f, 1.0f);
    m_projection.ortho(-w/2.0f, w/2.0f, h/2.0f, -h/2.0f, -1.0f, 1.0f);
}

void FPGAOpenGLWidget3::wheelEvent(QWheelEvent *event)
{
    // // 获取鼠标当前位置的坐标（相对于视图）
    // const QPointF mousePos = event->position();

    // // 根据滚轮的增量来计算缩放因子
    // // float zoomFactor = event->angleDelta().y() > 0 ? 1.1f : 0.9f;
    // float zoomFactor = event->angleDelta().y() > 0 ? 1.6f : 0.6f;

    // // 获取当前缩放因子，并根据滚轮方向调整
    // _scaleFactor *= zoomFactor;
    // if (_scaleFactor < MINIMUM_SCALEFACTOR) {
    //     _scaleFactor = MINIMUM_SCALEFACTOR;
    //     update();
    //     return;
    // }

    // // 获取缩放前的坐标系中光标的位置（相对OpenGL视图的坐标系）
    // float cursorX = mousePos.x();
    // float cursorY = mousePos.y();

    // // 计算缩放时需要保持的偏移量
    // _offsetX = cursorX - (cursorX - _offsetX) * zoomFactor;
    // _offsetY = cursorY - (cursorY - _offsetY) * zoomFactor;

    // // 重绘
    // update();

    // --------------------------------------------------------------------------
    // 获取鼠标在窗口中的位置
    QPoint mousePos = event->position().toPoint();

    // 转换为OpenGL坐标系 (原点在中心，y向上)
    QPointF glMousePos(mousePos.x() - width()/2.0f,
                       height()/2.0f - mousePos.y());

    // 计算缩放前的世界坐标
    QVector2D worldPosBefore = (QVector2D(glMousePos) - m_translation) / m_zoomFactor;

    // 调整缩放因子 (限制最小和最大缩放)
    float zoomDelta = event->angleDelta().y() > 0 ? 1.4f : 0.6f;
    m_zoomFactor *= zoomDelta;
    m_zoomFactor = qBound(0.02f, m_zoomFactor, 10.0f); // 限制缩放范围

    // 计算缩放后的世界坐标
    QVector2D worldPosAfter = (QVector2D(glMousePos) - m_translation) / m_zoomFactor;

    // 调整平移以使鼠标下的点保持静止
    m_translation += (worldPosAfter - worldPosBefore) * m_zoomFactor;
}

void FPGAOpenGLWidget3::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
}

void FPGAOpenGLWidget3::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::RightButton) {
        // 计算鼠标移动距离并转换为世界坐标
        QPoint delta = event->pos() - m_lastMousePos;
        // m_translation += QVector2D(delta.x(), -delta.y()); // 注意y轴方向
        m_translation += QVector2D(delta.x(), delta.y()); // 注意y轴方向

        m_lastMousePos = event->pos();
    }
}

// void FPGAOpenGLWidget3::mouseReleaseEvent(QMouseEvent *event)
// {
//     // if (event->button() & Qt::RightButton) {
//     // 右键释放时停止拖动
//     isDragging = false;
//     // }
// }

void FPGAOpenGLWidget3::paintTiles()
{
    m_tileVao.bind();
    m_tileVbo.bind();
    m_tileVbo.write(0, tileVertices.constData(), tileVertices.size() * sizeof(VertexData));

    // 绘制为线框矩形（GL_LINE_LOOP每个矩形独立）
    for(int i = 0; i < _tiles.size(); ++i) {
        glDrawArrays(GL_LINE_LOOP, i*4, 4);
    }
    m_tileVao.release();
}

void FPGAOpenGLWidget3::paintSites()
{
    m_siteVao.bind();
    m_siteVbo.bind();
    m_siteVbo.write(0, siteVertices.constData(), siteVertices.size() * sizeof(VertexData));

    // 绘制为线框矩形（GL_LINE_LOOP每个矩形独立）
    for(int i = 0; i < _sites.size(); ++i) {
        glDrawArrays(GL_LINE_LOOP, i*4, 4);
    }
    m_siteVao.release();
}

