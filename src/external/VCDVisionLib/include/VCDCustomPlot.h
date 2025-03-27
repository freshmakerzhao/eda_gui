#ifndef VCDCUSTOMPLOT_H
#define VCDCUSTOMPLOT_H
#include "qcustomplot.h"
#include "DataStruct.h"


struct VCDOneBitGraph{
    QCPGraph *zeroOr1Graph = nullptr;
    QCPGraph *xGraph = nullptr;
    QCPGraph *zGraph = nullptr;
    QCPGraph *fillGraph = nullptr;
    VCDOneBitGraph(QCPAxis *keyAxis, QCPAxis *valueAxis){
        int alpha = 70;  // 透明度通道。
        zeroOr1Graph = new QCPGraph(keyAxis, valueAxis);
        zeroOr1Graph->setPen(QPen(Qt::green, 1));
        QBrush ZeroOr1Brush(QColor(0, 255, 0, alpha ));
        zeroOr1Graph->setBrush(ZeroOr1Brush);

        xGraph = new QCPGraph(keyAxis, valueAxis);
        xGraph->setPen(QPen(Qt::red, 1));
        QBrush XBrush(QColor(255, 0, 0, alpha));
        xGraph->setBrush(XBrush);

        zGraph = new QCPGraph(keyAxis, valueAxis);
        zGraph->setPen(QPen(Qt::blue, 1));
        QBrush ZBrush(QColor(0, 0, 255, alpha));
        zGraph->setBrush(ZBrush);

        fillGraph = new QCPGraph(keyAxis, valueAxis);
        fillGraph->setPen(QPen(QColor(255, 255, 255, 0), 1));
    }

    virtual ~VCDOneBitGraph(){
        zeroOr1Graph = nullptr;
        xGraph = nullptr;
        zGraph = nullptr;
        fillGraph = nullptr;
    }
};

struct VCDMoreOneBitGraph{
    QCPGraph *zeroOr1GraphFirst = nullptr;
    QCPGraph *zeroOr1GraphSecond = nullptr;
    QCPGraph *xGraphFirst = nullptr;
    QCPGraph *xGraphSecond = nullptr;
    QCPGraph *zGraphFirst = nullptr;
    QCPGraph *zGraphSecond = nullptr;
    std::vector<QCPItemText*> saveQCPText;

    VCDMoreOneBitGraph(QCPAxis *keyAxis, QCPAxis *valueAxis){
        zeroOr1GraphFirst = new QCPGraph(keyAxis, valueAxis);
        zeroOr1GraphSecond = new QCPGraph(keyAxis, valueAxis);
        zeroOr1GraphFirst->setPen(QPen(Qt::green, 1));
        zeroOr1GraphSecond->setPen(QPen(Qt::green, 1));

        xGraphFirst = new QCPGraph(keyAxis, valueAxis);
        xGraphSecond = new QCPGraph(keyAxis, valueAxis);
        xGraphFirst->setPen(QPen(Qt::red, 1));
        xGraphSecond->setPen(QPen(Qt::red, 1));;

        zGraphFirst = new QCPGraph(keyAxis, valueAxis);
        zGraphSecond = new QCPGraph(keyAxis, valueAxis);
        zGraphFirst->setPen(QPen(Qt::blue, 1));
        zGraphSecond->setPen(QPen(Qt::blue, 1));
    }
    virtual ~VCDMoreOneBitGraph(){
        zeroOr1GraphFirst = nullptr;
        zeroOr1GraphSecond = nullptr;
        xGraphFirst = nullptr;
        xGraphSecond = nullptr;
        zGraphFirst = nullptr;
        zGraphSecond = nullptr;
        saveQCPText.clear();
    }
};

class VCDCPAxisRect;

class VCDCustomPlot : public QCustomPlot {

    Q_OBJECT
public:
    VCDCustomPlot(QWidget *parent = nullptr);
    virtual ~VCDCustomPlot();
    inline int getTickCount(){return mTickCount;}
    inline void setTickCount(int tickCount) {mTickCount = tickCount;}
    void setTracer(double xPosition, QString&&text);
    void setVCDGraph(QVector<PlotValueTimes*> &vecPlotValueTime);
    void setAppearanceSizeRatio(int reference);
    inline VCDCPAxisRect *getVCDCPAxis(){return mSelfQCPAxis; }

protected:
    void createAxisTrace();
    void resizeEvent(QResizeEvent *event) override;
private:
    QCPItemTracer *mTracer = nullptr;
    QCPItemLine *mLine = nullptr;
    QCPItemText *mText = nullptr;
    int mTickCount = 10;
    VCDCPAxisRect *mSelfQCPAxis = nullptr;

    std::vector<VCDOneBitGraph*> mVecOneBitVCDGraph;  // graph 池
    std::vector<VCDMoreOneBitGraph*> mVecMoreOneBitVCDGraph; // graph 池
signals:
    void updataSize(QResizeEvent *event);
};

class VCDCPAxis : public QCPAxis {
public:
    VCDCPAxis(QCPAxisRect *parent, AxisType type);
    virtual ~VCDCPAxis();

    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    inline void setTickVectorLabels(QVector<QString> &tickVectorLabels ){ mTickVectorLabels = tickVectorLabels; }

protected:
    QVector<QString> mTickVectorLabels ;

};

class VCDCPAxisRect : public QCPAxisRect {
    Q_OBJECT
public:
    VCDCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes=true);
    virtual ~VCDCPAxisRect();
    VCDCPAxis *addAxis(QCPAxis::AxisType type, VCDCPAxis *axis=nullptr);
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event, const QVariant &details) Q_DECL_OVERRIDE;

signals:
    void zoomAction(QWheelEvent *event);
    void dragAction(QMouseEvent *event, const QPointF &startPos);
    void clickAction(QMouseEvent *event, const QVariant &details);
};


class VCDTableWidget : public QTableWidget{
    Q_OBJECT
public:
    VCDTableWidget(QWidget *parent = nullptr);
    virtual ~VCDTableWidget();

    void setVCDTableStyle();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void updataSize(QResizeEvent *event);
    void whellAction(QWheelEvent *event);
};



#endif // VCDCUSTOMPLOT_H
