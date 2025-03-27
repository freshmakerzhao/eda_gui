#include "VCDCustomPlot.h"

VCDCustomPlot::VCDCustomPlot(QWidget *parent): QCustomPlot(parent){
    mPlotLayout->remove( axisRect());

    mSelfQCPAxis =new  VCDCPAxisRect(this, false);
    mPlotLayout->addElement(0,0, mSelfQCPAxis);
    xAxis = mSelfQCPAxis->axis(QCPAxis::atBottom);
    xAxis2 = mSelfQCPAxis->axis(QCPAxis::atTop);
    yAxis = mSelfQCPAxis->axis(QCPAxis::atLeft);
    yAxis2 = mSelfQCPAxis->axis(QCPAxis::atRight);

    xAxis2->ticker()->setTickCount(mTickCount);
    xAxis2->setRange(0,100);

    yAxis->setRange(0, 4.4);

    createAxisTrace();

    // 设置样式, 黑底白字。
    setBackground(QColor(0, 0, 0));
    xAxis2->setBasePen(QPen(Qt::white));
    xAxis2->setTickPen(QPen(Qt::white));
    xAxis2->setSubTickPen(QPen(Qt::white));
    xAxis2->setTickLabelColor(Qt::white);
    xAxis2->setLabelColor(Qt::white);
}


VCDCustomPlot::~VCDCustomPlot(){
    for(size_t i = 0; i < mVecOneBitVCDGraph.size(); i++){
        delete mVecOneBitVCDGraph[i];
        mVecOneBitVCDGraph[i] = nullptr;
    }
    mVecOneBitVCDGraph.clear();

    for(size_t i = 0; i < mVecMoreOneBitVCDGraph.size(); ++i){
        delete mVecMoreOneBitVCDGraph[i];
        mVecMoreOneBitVCDGraph[i] = nullptr;
    }
    mVecMoreOneBitVCDGraph.clear();
}

void VCDCustomPlot::resizeEvent(QResizeEvent *event)
{
    // 调用父类的 resizeEvent 函数，确保默认的尺寸调整行为正常
    QCustomPlot::resizeEvent(event);
    emit updataSize(event);
}

VCDTableWidget::VCDTableWidget(QWidget *parent ):QTableWidget(parent){
}

VCDTableWidget::~VCDTableWidget(){

}

void VCDTableWidget::setVCDTableStyle(){
    horizontalHeader()->setVisible(false);    // 隐藏列索引
    verticalHeader()->setVisible(false);  // 隐藏行索引
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //关闭滚动滑条

    //  widget 黑底白字样式表。
    QString widgetStyleSheet = "QTableWidget {"
                               "    background-color: black;"
                               "    color: white;"
                               "}";
    setStyleSheet(widgetStyleSheet);

    //设置表头
    QTableWidgetItem *item = this->item(0, 0);
    if(item){
        item->setBackground(QColor(240, 240, 240));
        item->setForeground(QColor(Qt::black));
    }

    return;
}

// 单位为像素。
void VCDCustomPlot::setAppearanceSizeRatio(int reference){
    qDebug() << "Name 表头的行高:" << reference;
    //  referance一般是45，45的宽度一般用来放：TickLength， 坐标轴数字( (xAxis2->labelFont()).height())，游标。
    QFontMetrics fontMetrics(xAxis2->tickLabelFont());
    // 获取字体的行高
    int lineHeight = fontMetrics.lineSpacing();
    // 设置 x 轴刻度标签与坐标轴之间的距离为 10 像素
    xAxis->setTickLabelPadding(1);
    xAxis2->setTickLength(0, reference / 7);
    xAxis2->setSubTickLength(0, reference / 14);
    xAxis2->setPadding(reference - lineHeight - reference / 7 - 1); //  坐标轴偏移。

    // 和游标隔开.
    mLine->end->setCoords(0, 0);
    mLine->start->setCoords(0, -(reference -1));  // 差一像素顶格存放。
    // 外观
    mTracer->setSize(1);//设置大小
    mText->setPadding(QMargins(5, 1, 5, 1));

}

void VCDCustomPlot::setVCDGraph( QVector<PlotValueTimes*> &vecPlotValueTime){
    size_t oneBitGraphIndex = 0;
    size_t oneBitGraphSize = mVecOneBitVCDGraph.size();

    size_t moreOneBitGraphIndex = 0;
    size_t moreOneBitGraphSize = mVecMoreOneBitVCDGraph.size();

    for(size_t i = 0; i < vecPlotValueTime.size(); ++i){

        // 空数据,占位
        if(vecPlotValueTime[i] == nullptr){
            continue;
        }

        if(vecPlotValueTime[i]->plotValueType == oneBit){
            PlotOneBitValueTimes *plotOneBitValueTimes = static_cast<PlotOneBitValueTimes *>(vecPlotValueTime[i]);

            double maxTime = 0;
            double minTime = 0;

            QCPGraph *zeroOr1Graph = nullptr;
            QCPGraph *xGraph = nullptr;
            QCPGraph *zGraph = nullptr;
            QCPGraph *fillGraph = nullptr;
            if(oneBitGraphIndex >= oneBitGraphSize){

                VCDOneBitGraph *OneBitGraph = new  VCDOneBitGraph(xAxis2, yAxis);
                zeroOr1Graph = OneBitGraph->zeroOr1Graph;
                xGraph = OneBitGraph->xGraph;
                zGraph = OneBitGraph->zGraph;
                fillGraph  = OneBitGraph->fillGraph;

                zeroOr1Graph->setChannelFillGraph(fillGraph);
                xGraph->setChannelFillGraph(fillGraph);
                zGraph->setChannelFillGraph(fillGraph);

                mVecOneBitVCDGraph.push_back(OneBitGraph);
            } else{
                zeroOr1Graph = mVecOneBitVCDGraph[oneBitGraphIndex]->zeroOr1Graph;
                xGraph = mVecOneBitVCDGraph[oneBitGraphIndex]->xGraph;
                zGraph = mVecOneBitVCDGraph[oneBitGraphIndex]->zGraph;
                fillGraph = mVecOneBitVCDGraph[oneBitGraphIndex]->fillGraph;
            }
            zeroOr1Graph->setData(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime, plotOneBitValueTimes->zeroAnd1ValueTime.vecValue, true);
            if(!plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.empty()) {
                maxTime = plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back();
                minTime = plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.first();
            }

            xGraph->setData(plotOneBitValueTimes->xValueTime.vecTime, plotOneBitValueTimes->xValueTime.vecValue, true);
            if(!plotOneBitValueTimes->xValueTime.vecTime.empty()) {
                maxTime = maxTime > plotOneBitValueTimes->xValueTime.vecTime.back()? maxTime : plotOneBitValueTimes->xValueTime.vecTime.back();
                minTime = minTime < plotOneBitValueTimes->xValueTime.vecTime.first()? minTime : plotOneBitValueTimes->xValueTime.vecTime.first();
            }

            zGraph->setData(plotOneBitValueTimes->zValueTime.vecTime, plotOneBitValueTimes->zValueTime.vecValue, true);
            if(!plotOneBitValueTimes->zValueTime.vecTime.empty()){
                maxTime = maxTime > plotOneBitValueTimes->zValueTime.vecTime.back()? maxTime : plotOneBitValueTimes->zValueTime.vecTime.back();
                minTime = minTime < plotOneBitValueTimes->zValueTime.vecTime.first()? minTime : plotOneBitValueTimes->zValueTime.vecTime.first();
            }

            fillGraph->setData({minTime, maxTime}, {i + 0.9, i + 0.9}, true);

            oneBitGraphIndex++;
        }else{

            VCDMoreOneBitGraph *moreOneBitGraph = nullptr;
            if(moreOneBitGraphIndex >= moreOneBitGraphSize){
                moreOneBitGraph = new VCDMoreOneBitGraph(xAxis2, yAxis);
                mVecMoreOneBitVCDGraph.push_back(moreOneBitGraph);
            }else {
                moreOneBitGraph = mVecMoreOneBitVCDGraph[moreOneBitGraphIndex];
            }
            moreOneBitGraphIndex++;

            PlotMoreOneBitValueTimes *plotMoreOneBitValueTimes = static_cast<PlotMoreOneBitValueTimes *>(vecPlotValueTime[i]);

            moreOneBitGraph->zeroOr1GraphFirst->setData(plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime, plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue, true);
            moreOneBitGraph->zeroOr1GraphSecond->setData(plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime, plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue, true);

            moreOneBitGraph->xGraphFirst->setData(plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime, plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue, true);
            moreOneBitGraph->xGraphSecond->setData(plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime, plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue, true);

            moreOneBitGraph->zGraphFirst->setData(plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime, plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue, true);
            moreOneBitGraph->zGraphSecond->setData(plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime, plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue, true);

            //  QCPText
            size_t j = 0;
            size_t QCPTextSize = moreOneBitGraph->saveQCPText.size();
            for(; j < plotMoreOneBitValueTimes->vecText.size(); ++j){
                QCPItemText *QCPText = nullptr;
                if( j < QCPTextSize){
                    QCPText = moreOneBitGraph->saveQCPText[j];
                }else{
                    QCPText = new QCPItemText(this);
                    QCPText->position->setAxes(xAxis2, yAxis);
                    moreOneBitGraph->saveQCPText.push_back(QCPText);

                    QCPText->setColor(Qt::white); // 文字颜色
                    QCPText->setBrush(QBrush(Qt::black)); // 背景填充色
                    QCPText->setPen(QPen(Qt::black)); // 边框颜色（与背景一致）
                }
                QCPText->setText(plotMoreOneBitValueTimes->vecText[j].text);
                QCPText->position->setCoords(plotMoreOneBitValueTimes->vecText[j].key, plotMoreOneBitValueTimes->vecText[j].value);
            }

            while(j < QCPTextSize){
                moreOneBitGraph->saveQCPText[j]->position->setCoords( -1, -1);
                ++j;
            }
        }

    }
    // 将池中剩余的对象内容置空
    for(;oneBitGraphIndex < mVecOneBitVCDGraph.size(); oneBitGraphIndex++ ){
        mVecOneBitVCDGraph[oneBitGraphIndex]->zeroOr1Graph->setData({}, {}, true);
        mVecOneBitVCDGraph[oneBitGraphIndex]-> xGraph->setData({}, {}, true);
        mVecOneBitVCDGraph[oneBitGraphIndex]->zGraph->setData({}, {}, true);
        mVecOneBitVCDGraph[oneBitGraphIndex]->fillGraph->setData({}, {}, true);
    }

    for(;moreOneBitGraphIndex < mVecMoreOneBitVCDGraph.size(); moreOneBitGraphIndex++ ){
        VCDMoreOneBitGraph *moreOneBitGraph = mVecMoreOneBitVCDGraph[moreOneBitGraphIndex];
        moreOneBitGraph->zeroOr1GraphFirst->setData({}, {}, true);
        moreOneBitGraph->zeroOr1GraphSecond->setData({}, {}, true);

        moreOneBitGraph->xGraphFirst->setData({}, {}, true);
        moreOneBitGraph->xGraphSecond->setData({}, {}, true);

        moreOneBitGraph->zGraphFirst->setData({}, {}, true);
        moreOneBitGraph->zGraphSecond->setData({}, {}, true);

        //  QCPText
        size_t j = 0;
        size_t QCPTextSize = moreOneBitGraph->saveQCPText.size();
        while(j < QCPTextSize){
            moreOneBitGraph->saveQCPText[j]->position->setCoords( -1, -1);
            ++j;
        }
    }

}

void VCDCustomPlot::createAxisTrace(){
    //  text
    // 实例化QCPItemText

    mTracer = new QCPItemTracer(this);
    mTracer->setPen(QPen(Qt::red));
    mTracer->setBrush(QBrush(Qt::red));
    mTracer->setStyle(QCPItemTracer::tsCrosshair);
    mTracer->position->setAxes(xAxis2, yAxis);
    mTracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
    mTracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
    mTracer->position->setCoords(-10, 0);

    // mLine的位置使用像素来定位，而不是坐标。
    mLine = new QCPItemLine(this);
    mLine->setClipToAxisRect(false);
    mLine->setPen(QPen(Qt::red, 1));
    mLine->setHead(QCPLineEnding::esSpikeArrow);

    mLine->end->setParentAnchor(mTracer->position);
    mLine->start->setParentAnchor(mLine->end);


    mText = new QCPItemText(this);
    mText->setPen(QPen(Qt::NoPen));
    // 设置文本内容
    mText->setText("ticker");
    mText->setClipToAxisRect(false);
    mText->setPen(QPen(Qt::black));
    mText->setBrush(QBrush(Qt::yellow));
    mText->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);//上中间
    mText->position->setAxes(xAxis2, yAxis);
    mText->position->setParentAnchor(mLine->start); //将游标说明锚固在tracer位置处，实现自动跟随
}

void VCDCustomPlot::setTracer(double xPosition, QString&& text){
    mTracer->position->setCoords(xPosition, 0);
    mText->setText(text);
}

VCDCPAxisRect::VCDCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes) : QCPAxisRect(parentPlot, setupDefaultAxes) {
    VCDCPAxis *xAxis = addAxis(QCPAxis::atBottom);
    VCDCPAxis *yAxis = addAxis(QCPAxis::atLeft);
    VCDCPAxis *xAxis2 = addAxis(QCPAxis::atTop);
    VCDCPAxis *yAxis2 = addAxis(QCPAxis::atRight);
    setRangeDragAxes(xAxis, yAxis);
    setRangeZoomAxes(xAxis, yAxis);
    yAxis2->setVisible(false);
    yAxis2->grid()->setVisible(false);
    yAxis2->grid()->setZeroLinePen(Qt::NoPen);

    xAxis->setVisible(false);
    xAxis->grid()->setVisible(false);

    xAxis2->setVisible(true);
    xAxis2->grid()->setVisible(true);
    xAxis2->grid()->setZeroLinePen(Qt::NoPen);

    yAxis->setVisible(false);
    yAxis->setRangeReversed(true);
    yAxis->grid()->setVisible(true);
}

VCDCPAxisRect::~VCDCPAxisRect(){

}

VCDCPAxis *VCDCPAxisRect::addAxis(QCPAxis::AxisType type, VCDCPAxis *axis)
{
    VCDCPAxis *newAxis = axis;
    if (!newAxis)
    {
        newAxis = new VCDCPAxis(this, type);
    } else // user provided existing axis instance, do some sanity checks
    {
        if (newAxis->axisType() != type)
        {
            qDebug() << Q_FUNC_INFO << "passed axis has different axis type than specified in type parameter";
            return nullptr;
        }
        if (newAxis->axisRect() != this)
        {
            qDebug() << Q_FUNC_INFO << "passed axis doesn't have this axis rect as parent axis rect";
            return nullptr;
        }
        if (axes().contains(newAxis))
        {
            qDebug() << Q_FUNC_INFO << "passed axis is already owned by this axis rect";
            return nullptr;
        }
    }
    if (!mAxes[type].isEmpty()) // multiple axes on one side, add half-bar axis ending to additional axes with offset
    {
        bool invert = (type == QCPAxis::atRight) || (type == QCPAxis::atBottom);
        newAxis->setLowerEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, !invert));
        newAxis->setUpperEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, invert));
    }
    mAxes[type].append(newAxis);

    // reset convenience axis pointers on parent QCustomPlot if they are unset:
    if (mParentPlot && mParentPlot->axisRectCount() > 0 && mParentPlot->axisRect(0) == this)
    {
        switch (type)
        {
        case QCPAxis::atBottom: { if (!mParentPlot->xAxis) mParentPlot->xAxis = newAxis; break; }
        case QCPAxis::atLeft: { if (!mParentPlot->yAxis) mParentPlot->yAxis = newAxis; break; }
        case QCPAxis::atTop: { if (!mParentPlot->xAxis2) mParentPlot->xAxis2 = newAxis; break; }
        case QCPAxis::atRight: { if (!mParentPlot->yAxis2) mParentPlot->yAxis2 = newAxis; break; }
        }
    }

    return newAxis;
}

void VCDCPAxisRect::wheelEvent(QWheelEvent *event){
     emit zoomAction(event);
}

void VCDCPAxisRect::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) {
    emit dragAction(event, startPos);
}

void VCDCPAxisRect::mousePressEvent(QMouseEvent *event, const QVariant &details) {
    emit clickAction(event,  details);
}

VCDCPAxis::VCDCPAxis(QCPAxisRect *parent, AxisType type):QCPAxis(parent, type){

}

VCDCPAxis::~VCDCPAxis(){

}

void VCDCPAxis::draw(QCPPainter *painter) {
    QVector<double> subTickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<double> tickPositions; // the final coordToPixel transformed vec tor passed to QCPAxisPainter
    QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
    tickPositions.reserve(mTickVector.size());
    tickLabels.reserve(mTickVector.size());
    subTickPositions.reserve(mSubTickVector.size());

    if (mTicks)
    {
        for (int i=0; i<mTickVector.size(); ++i)
        {
            tickPositions.append(coordToPixel(mTickVector.at(i)));
            if (mTickLabels && i < mTickVectorLabels.size())
                tickLabels.append(mTickVectorLabels.at(i));
        }

        if (mSubTicks)
        {
            const int subTickCount = mSubTickVector.size();
            for (int i=0; i<subTickCount; ++i)
                subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
        }
    }

    // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to draw the axis.
    // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
    mAxisPainter->type = mAxisType;
    mAxisPainter->basePen = getBasePen();
    mAxisPainter->labelFont = getLabelFont();
    mAxisPainter->labelColor = getLabelColor();
    mAxisPainter->label = mLabel;
    mAxisPainter->substituteExponent = mNumberBeautifulPowers;
    mAxisPainter->tickPen = getTickPen();
    mAxisPainter->subTickPen = getSubTickPen();
    mAxisPainter->tickLabelFont = getTickLabelFont();
    mAxisPainter->tickLabelColor = getTickLabelColor();
    mAxisPainter->axisRect = mAxisRect->rect();
    mAxisPainter->viewportRect = mParentPlot->viewport();
    mAxisPainter->abbreviateDecimalPowers = mScaleType == stLogarithmic;
    mAxisPainter->reversedEndings = mRangeReversed;
    mAxisPainter->tickPositions = tickPositions;
    mAxisPainter->tickLabels = tickLabels;
    mAxisPainter->subTickPositions = subTickPositions;
    mAxisPainter->draw(painter);
}

void VCDTableWidget::resizeEvent(QResizeEvent *event) {
    emit updataSize(event);
}

void VCDTableWidget::wheelEvent(QWheelEvent *event) {
    emit whellAction(event);
}


