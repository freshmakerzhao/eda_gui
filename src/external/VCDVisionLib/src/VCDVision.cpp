#include "VCDVision.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include "ParserVCDJson.h"
#include <QObject>

VCDVision *VCDVision::mInstance = nullptr;
VCDVision *VCDVision::getInstance(){
    if(!mInstance) {
        mInstance = new VCDVision();
        mInstance->setAppearanceSizeRatio();         //设置尺寸
    }
    return mInstance;
}

int VCDVision::process(const QString &jsonFilePath){
    if(processValue(jsonFilePath)){
        return -1;
    }else{
        mInstance->reset();
        refreshWaveSignalName();
        refreshSignalValue();
        plotWave( );
    }
    return 0;
}

VCDVision::VCDVision(QWidget *parent): QWidget(parent) {

    establishLayout();

    connect(mVCDCustomPlot, &VCDCustomPlot::updataSize , this, &VCDVision::resizeAxisRect);

    connect(mVCDCustomPlot->getVCDCPAxis(), &VCDCPAxisRect::zoomAction, this, &VCDVision::zoomWave);
    connect(mVCDCustomPlot->getVCDCPAxis(), &VCDCPAxisRect::dragAction, this, &VCDVision::dragWave);
    connect(mVCDCustomPlot->getVCDCPAxis(), &VCDCPAxisRect::clickAction, this, &VCDVision::cursorDataRetrieval);

    connect(mTableNameWidget, &VCDTableWidget::updataSize , this, &VCDVision::resizeVCDTableWidget);
    connect(mTableNameWidget, &VCDTableWidget::whellAction, this, &VCDVision::wheelTable);
    connect(mTableValueWidget, &VCDTableWidget::whellAction, this, &VCDVision::wheelTable);

    connect(&ParserVCDJson::instance(), &ParserVCDJson::error, [this](const QString errorInformation){
        emit error(errorInformation);
    });

}

VCDVision::~VCDVision(){

    for(size_t i = 0; i <  mVecOneBitvalueTime.size(); ++i){
        delete mVecOneBitvalueTime[i];
        mVecOneBitvalueTime[i] = nullptr;
    }
    mVecOneBitvalueTime.clear();

    for(size_t i = 0; i <  mVecMoreOneBitValueTime.size(); ++i){
        delete mVecMoreOneBitValueTime[i];
        mVecMoreOneBitValueTime[i] = nullptr;
    }
    mVecMoreOneBitValueTime.clear();
}

int VCDVision::processValue(const QString &jsonFilePath){
    if(processVCDJsonFile(jsonFilePath)){
        return -1;
    }

    mNameValueTableBeginIndex = 0;

    mInitialTimeUint = mVcdTimeScale->vcdTimeUnit;
    mIndexTimeUint = mVcdTimeScale->vcdTimeUnit;
    return 0;
}

void VCDVision::establishLayout(){
    mVLayout = new QVBoxLayout(this);
    // 顶部按钮
    setButton(this);
    mVLayout->addWidget(mButtonReset);
    // 水平布局器
    mSplitter = new QSplitter(this); // 创建水平QSplitte
    setWaveSignalName(mSplitter);
    mSplitter->addWidget(mTableNameWidget);

    setWaveSignalValue(mSplitter);
    mSplitter->addWidget(mTableValueWidget);

    setWave(mSplitter);
    mSplitter->addWidget(mVCDCustomPlot);

    mSplitter->setSizes(QList<int>() << 60 << 40 << 500);
    mSplitter->setChildrenCollapsible(false); // 不允许部件折叠
    mVLayout->addWidget(mSplitter);
}

void VCDVision::resizeAxisRect(){
    int rowHeight = mTableNameWidget->rowHeight(0) / 1.5;
    QSize axisRectSize = mVCDCustomPlot->axisRect()->size();
    double yRangeUp = double(axisRectSize.height()) / double(rowHeight);
    mVCDCustomPlot->yAxis->setRange(0, yRangeUp);
    plotWave( );
}

void VCDVision::resizeVCDTableWidget(QResizeEvent *event){
    // 获取新的尺寸
    QSize newSize = event->size();
    double size = double(newSize.height() - mTableNameWidget->rowHeight(0)) / ((double)mTableNameWidget->rowHeight(0) / 1.5);
    mNameValueTableSize = std::fmod(size, 1.0)  != 0  ? size + 1 :  size;

    refreshWaveSignalName();
    refreshSignalValue();
    plotWave( );
}

void VCDVision::wheelTable(QWheelEvent *event){

    // 获取滚轮滚动的角度
    int degrees = event->angleDelta().y() / 8;
    int steps = degrees / 15;

    int waveNumber = getSignalWaveNumber();
    if(steps < 0 && waveNumber < mNameValueTableBeginIndex + mNameValueTableSize){
        return;
    }else if(steps >  mNameValueTableBeginIndex ){
        mNameValueTableBeginIndex = 0;
        return;
    }

    mNameValueTableBeginIndex = mNameValueTableBeginIndex - steps;

    if(mNameValueTableBeginIndex + mNameValueTableSize - 1 > waveNumber) { // steps 的绝对值可能大于1.
        mNameValueTableBeginIndex = waveNumber - mNameValueTableSize + 1;
    }


    qDebug() << "mNameValueTableBeginIndex:" << mNameValueTableBeginIndex;
    qDebug() << "mNamevalueTableSize:" << mNameValueTableSize;

    refreshWaveSignalName();
    refreshSignalValue();
    plotWave( );

}


void VCDVision::setAppearanceSizeRatio(){
    // name value 表头尺寸定义
    int headerRowHeight = mTableNameWidget->rowHeight(0)  *1.5;
    mTableNameWidget->setRowHeight(0, headerRowHeight);
    mTableValueWidget->setRowHeight(0, headerRowHeight);

    mVCDCustomPlot->setAppearanceSizeRatio(headerRowHeight);
}

int VCDVision::processVCDJsonFile(const  QString &jsonFilePath){

    ParserVCDJson &parserVCDJson = ParserVCDJson::instance();
    if(parserVCDJson.parser(jsonFilePath)){
        return -1;
    }

    mVcdTimeScale = parserVCDJson.getTimeScale();
    mTopScope = parserVCDJson.getTopScope();;
    mHashSignalValue = parserVCDJson.getHashSignalValue();

    // data
    mVecSignalOrder.clear();
    QQueue<VCDScope*> queueScope;
    for(auto it : *mTopScope){
        queueScope.push_back(it);
    }

    while(!queueScope.empty()){
        VCDScope *everyScope = queueScope.head();
        queueScope.pop_front();
        for(auto it : everyScope->childScope){
            queueScope.push_back(it);
        }
        mVecSignalOrder += everyScope->vecSignals;
    }

    return 0;
}

void VCDVision::setButton(QWidget *parent) {
    mButtonReset = new QPushButton ("Reset",parent);
    mButtonReset->setStyleSheet("QPushButton {"
                                "border: 2px solid #E0E0E0; "
                                "border-radius: 0px; "
                                "background-color: white; "
                                "color: #000000; "
                                "text-align: center; "
                                "text-decoration: none; "
                                "display: inline-block; "
                                "font-size: 16px; "
                                "transition-duration: 0.4s; "
                                "cursor: pointer; "
                                "}"
                                "QPushButton:hover {"
                                "background-color: #E0E0E0; "
                                "color: black; "
                                "}"
                                );
    mButtonReset->setFixedSize(60, 30); // 设置按钮的固定大小
    connect(mButtonReset, &QRadioButton::clicked, [&]() {
        reset();
        plotWave( );
    });

}

void VCDVision::reset(){
    mLeftTime = 0;
    mAxisLen = 100;
    mAmplificationFactor = 1;
    mIndexTimeUint = mInitialTimeUint;
    mShowTracer = false;
    setTracer();
}

void VCDVision::setWave(QWidget *parent){
    // wave
    // 绘制水平条
    mVCDCustomPlot = new VCDCustomPlot(parent);
    mTextItem = new QCPItemText(mVCDCustomPlot);
}

void VCDVision:: setWaveSignalName(QWidget *parent){

    // 创建表格
    // name
    mTableNameWidget = new VCDTableWidget(parent);
    //表格
    mTableNameWidget->setColumnCount(1);
    mTableNameWidget->setRowCount(1);
    QTableWidgetItem *item = new QTableWidgetItem("Name", Qt::AlignLeft );
    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 禁用编辑
    mTableNameWidget->setItem(0, 0, item);
    mTableNameWidget->setVCDTableStyle();

    // 设置双击单元格展开信号每一位的操作
    connect(mTableNameWidget,  &VCDTableWidget::itemDoubleClicked, this, &VCDVision::onCellDoubleClicked);
}

void VCDVision::refreshWaveSignalName(bool refreshSignalNumber ){

    mHashMoreOneBitSignal.clear();
    mTableNameWidget->setRowCount(mNameValueTableSize + 1);

    size_t SignalOrderSize = mVecSignalOrder.size();
    size_t index = 0;  //  因为扩展位造成的。
    int totalRows = mTableNameWidget->rowCount();

    for(size_t i = 0; i < SignalOrderSize; ++i){

        QTableWidgetItem *everyQTableWidgetItem = nullptr;
        QString name = mVecSignalOrder[i]->signalName;
        if(mVecSignalOrder[i]->signalValueSize > 1){
            index++;
            if(index > mNameValueTableBeginIndex && index <= mNameValueTableBeginIndex + mNameValueTableSize){
                QString moreOneBitName = name + "[" + QString::number(mVecSignalOrder[i]->signaIndexLeft) + ":" + QString::number(mVecSignalOrder[i]->signaIndexRight) + "]";
                updateItem( mTableNameWidget, totalRows, everyQTableWidgetItem, index - mNameValueTableBeginIndex, moreOneBitName);
                mHashMoreOneBitSignal.insert(everyQTableWidgetItem, mVecSignalOrder[i]);
            }
            if(mVecSignalOrder[i]->expandDisplay){
                for(size_t j = 0;  j < mVecSignalOrder[i]->signalValueSize; ++j){
                    index++;
                    if(index > mNameValueTableBeginIndex && index <= mNameValueTableBeginIndex + mNameValueTableSize){
                        QString bitName = name + "[" + QString::number(mVecSignalOrder[i]->signaIndexLeft - j) + "]";
                        updateItem( mTableNameWidget, totalRows, everyQTableWidgetItem, index - mNameValueTableBeginIndex, bitName);
                    }
                }
            }
        }else{

            index++;
            if(index > mNameValueTableBeginIndex && index <= mNameValueTableBeginIndex + mNameValueTableSize){
                updateItem( mTableNameWidget, totalRows, everyQTableWidgetItem, index - mNameValueTableBeginIndex, name);
            }
        }
    }

     //  将多余的单元格置空。
    index++;
    emptyRemainItem(mTableNameWidget, totalRows, index - mNameValueTableBeginIndex );
}

void VCDVision::updateItem( VCDTableWidget* table, int totalRows, QTableWidgetItem *&everyQTableWidgetItem, int row, QString &text){
    everyQTableWidgetItem = table->item(row, 0);
    if(!everyQTableWidgetItem){
        everyQTableWidgetItem =  new QTableWidgetItem();
        Qt::ItemFlags flags = everyQTableWidgetItem->flags() & ~Qt::ItemIsEditable;
        everyQTableWidgetItem->setFlags(flags);
        table->setItem(row, 0, everyQTableWidgetItem);
    }
    everyQTableWidgetItem->setText(text);
}

void VCDVision::emptyRemainItem(VCDTableWidget* table, int totalRows,  int row){
    while(row < totalRows){
        auto it =  table->item(row, 0);
        if(it){
            it->setText("");
        }
        row++;
    }
}

void VCDVision::onCellDoubleClicked(QTableWidgetItem *item){
    auto it = mHashMoreOneBitSignal.find(item);
    if(it == mHashMoreOneBitSignal.end()){
        return;
    }
    it.value()->expandDisplay = ! it.value()->expandDisplay;

    refreshWaveSignalName();
    refreshSignalValue();
    plotWave( );
}

void VCDVision::setWaveSignalValue(QWidget *parent){
    // value
    mTableValueWidget = new VCDTableWidget(parent);
    //表格
    mTableValueWidget->setColumnCount(1);
    mTableValueWidget->setRowCount(1);

    QTableWidgetItem *item = new QTableWidgetItem("Value", Qt::AlignLeft );
    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 禁用编辑
    mTableValueWidget->setItem(0, 0, item);
    mTableValueWidget->setVCDTableStyle();
}

void VCDVision::refreshSignalValue(bool refreshSignalNumber){

    mTableValueWidget->setRowCount(mNameValueTableSize + 1);

    ValueTime *everysSignalValueTime = nullptr;
    size_t reaIndex = 0;
    int totalRows = mTableValueWidget->rowCount();

    for(size_t i = 0; i < mVecSignalOrder.size(); ++i){

        // 确定时间
        auto it = mHashSignalValue->find(mVecSignalOrder[i]->idCode);
        if(it == mHashSignalValue->end()){
            reaIndex++;
            if(mVecSignalOrder[i]->expandDisplay){
                reaIndex += mVecSignalOrder[i]->signalValueSize;
            }
            qDebug() << "The signal:" << mVecSignalOrder[i]->signalName << "has no value.";
            continue;
        }
         everysSignalValueTime = it.value();

         size_t j = 0;
        for(; j < everysSignalValueTime->vecTime.size(); ++j ){
             uint64_t time = everysSignalValueTime->vecTime[j];
            if(time < mTracerPositionValueTime) {
                continue;
            } else if(time == mTracerPositionValueTime){
                ++j;   // 为了后续处理的统一
                break;
            } else {   // mTracerPositionValueTime > time
                break;
            }
        }

        // 取相应时间的值。
        QString value =everysSignalValueTime->vecValue[j - 1];
        QTableWidgetItem *everyQTableWidgetItem = nullptr;
        if(mVecSignalOrder[i]->signalValueSize > 1){
            reaIndex++;
            if(reaIndex > mNameValueTableBeginIndex  && reaIndex <= mNameValueTableBeginIndex + mNameValueTableSize){
                QString valueHex = binary2Hex(value, mVecSignalOrder[i]->signalValueSize);
                updateItem( mTableValueWidget, totalRows, everyQTableWidgetItem, reaIndex - mNameValueTableBeginIndex, valueHex);
            }

            if(mVecSignalOrder[i]->expandDisplay){

                for(size_t k = 0; k < mVecSignalOrder[i]->signalValueSize; ++k){
                    reaIndex++;
                    if(reaIndex > mNameValueTableBeginIndex && reaIndex <= mNameValueTableBeginIndex + mNameValueTableSize){
                        QString bitValue = everysSignalValueTime->vecValue[j - 1].at(k);
                         updateItem( mTableValueWidget, totalRows, everyQTableWidgetItem, reaIndex - mNameValueTableBeginIndex, bitValue);
                    }
                }
            }
        }else{
            reaIndex++;
            if(reaIndex > mNameValueTableBeginIndex && reaIndex <= mNameValueTableBeginIndex + mNameValueTableSize){
                updateItem( mTableValueWidget, totalRows, everyQTableWidgetItem, reaIndex - mNameValueTableBeginIndex, value);
            }
        }
    }

    //  将多余的单元格置空。
    reaIndex++;
    emptyRemainItem(mTableValueWidget, totalRows, reaIndex - mNameValueTableBeginIndex );
}

int VCDVision::getSignalWaveNumber(){
    size_t number = 0;
    for(auto it : mVecSignalOrder){
        if(it->expandDisplay){
            number += 1 + it->signalValueSize;
        }else{
            number += 1;
        }
    }
    return number;
}

QString VCDVision::binary2Hex(QString &binValue, int width){
    QString hexStr = "";
    int multipleOfFour = width;
    // 4的倍数。
    if(width % 4){
        multipleOfFour +=  (4 - width % 4);
    }
    QString temp(multipleOfFour, '0');
    std::copy(binValue.begin(), binValue.end(), temp.begin() + (multipleOfFour - width));

    for(int j = 0; j < multipleOfFour; j += 4) {
        QString fourBits = temp.mid(j, 4);

        int zNumber = fourBits.count('z', Qt::CaseInsensitive);
        int xNumber = fourBits.count('x', Qt::CaseInsensitive);

        if(zNumber == 0 && xNumber == 0 ) {
            int decimal = fourBits.toInt(nullptr, 2);
            if(decimal < 10) {
                hexStr.append(QChar('0' + decimal));
            } else {
                hexStr.append(QChar('A' + decimal - 10));
            }
        }else if(xNumber == 4) {
            hexStr.append('X');
        } else if(zNumber == 4) {
            hexStr.append('Z');
        }else if(xNumber == 0) {
            hexStr.append('z');
        }else {
            hexStr.append('x');
        }
    }
    return hexStr;
}


void VCDVision::zoomWave(QWheelEvent *event){

// uint64_t valueTick = mRightTiick - mLeftTick;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    const double delta = event->delta();
#else
    const double delta = event->angleDelta().y();
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QPointF pos = event->pos();
#else
    const QPointF pos = event->position();
#endif

    QPoint mousePos =pos.toPoint();
    // 获取 QCPAxisRect 的矩形区域
    QRect axisRectGeometry = mVCDCustomPlot->axisRect()->rect();
    // 判断鼠标位置是否在 QCPAxisRect 内
    if (!axisRectGeometry.contains(mousePos)) {
        return;
    }

    double wheelSteps = delta/120.0;     // a single step delta is +/-120 usually
    double rangeZoomFactorHorz = 0.85;
    double factor = qPow(rangeZoomFactorHorz, wheelSteps);
    factor = factor > 1 ? 2 : 0.5;   // 按倍数缩放

    double centor = mVCDCustomPlot->xAxis2->pixelToCoord(pos.x());
    if( centor < 0) {
        centor = 0;
    } else if(centor > 10000) {
        centor = 10000;
    }

    setTimeUintAndFactor();
    prepareScaleData(factor , centor);
    setTracer();
    plotWave( );
}

void VCDVision::setTimeUintAndFactor(){
    double factorOffset = mAxisLen / std::pow(1000, mIndexTimeUint - mInitialTimeUint );
    if(factorOffset < 10){
        if(mIndexTimeUint > mInitialTimeUint) {
            mIndexTimeUint -= 1;
        }
    } else if (factorOffset >= 10000) {
        if(mIndexTimeUint < mVecAllTimeUint.size() - 1) {
            mIndexTimeUint += 1;
        }
    }
}

void VCDVision::prepareScaleData (double factor, double centor){
    //  到时间精度即可，太小没有用.
    if(factor < 1 && mAmplificationFactor < 0.5) {
        return;
    }
    // 放大倍数太大会溢出，此倍数已经很大了。
    if(factor > 1 && mAmplificationFactor >= 72057594037927936) {
        return;
    }

    uint64_t oldLeftTimeTickLen = centor  *mAmplificationFactor;
    mAmplificationFactor = mAmplificationFactor  *factor;
    uint64_t newLeftTimeTickLen = centor  * mAmplificationFactor;

    if(oldLeftTimeTickLen > newLeftTimeTickLen) {
        mLeftTime = mLeftTime + (oldLeftTimeTickLen - newLeftTimeTickLen);
    }else {
        if((newLeftTimeTickLen - oldLeftTimeTickLen) >= mLeftTime) {
            mLeftTime = 0;
        } else {
            mLeftTime = mLeftTime - (newLeftTimeTickLen - oldLeftTimeTickLen) ;
        }
    }
    mAxisLen = 100  *mAmplificationFactor;
}


void VCDVision::plotWave( ){
    setAxis();
    refreshWaveValue();
    mVCDCustomPlot->replot();
}

void VCDVision::refreshOneBitValueTime(uint64_t maxAxisRealTimeLen, ValueTime *signalValueTime, QVector<PlotValueTimes*> &vecPlotValueTime, PlotOneBitValueTimes *plotOneBitValueTimes, int index ){
    size_t order = vecPlotValueTime.size();

    LastValueType lastValueType = firstData;
    uint64_t everyTime = 0;
    uint64_t nextEveryTime = 0;

    for(size_t j = 0; j < signalValueTime->vecTime.size(); ++j){

        everyTime = signalValueTime->vecTime[j];

        if(j+1 < signalValueTime->vecTime.size() ){
            nextEveryTime = signalValueTime->vecTime[j + 1];
        }

        if(mLeftTime > everyTime &&  mLeftTime> nextEveryTime ) {
            continue;
        }else{
            if(signalValueTime->vecValue[j].at(index) == '0' || signalValueTime->vecValue[j].at(index) == '1'){

                if(everyTime > mLeftTime) {
                    if(everyTime < maxAxisRealTimeLen) {
                        plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back((everyTime - mLeftTime ) / mAmplificationFactor );
                    } else {
                        plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(maxAxisRealTimeLen / mAmplificationFactor );
                    }
                } else {
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(0);
                }

                if(lastValueType == isX   || lastValueType == isZ) {
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(std::nan(""));
                } else if(lastValueType == isZeroOr1) {
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.back());
                }

                plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(signalValueTime->vecValue[j].at(index) == '0'? 0.9 + order: 0.1 + order);

                switch (lastValueType){
                case isX:{
                    plotOneBitValueTimes->xValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->xValueTime.vecValue.push_back(0.5 + order);
                    plotOneBitValueTimes->xValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->xValueTime.vecValue.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.back());
                    break;
                }
                case isZ:{
                    plotOneBitValueTimes->zValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->zValueTime.vecValue.push_back(0.5 + order);
                    plotOneBitValueTimes->zValueTime.vecTime.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.back());
                    plotOneBitValueTimes->zValueTime.vecValue.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.back());
                    break;
                }
                case firstData:
                case isZeroOr1:
                default:{
                    break;
                }
                }

                lastValueType = isZeroOr1;
            }else if(signalValueTime->vecValue[j].at(index) == 'x' || signalValueTime->vecValue[j].at(index) == 'X'){
                if(everyTime > mLeftTime) {
                    if(everyTime < maxAxisRealTimeLen) {
                        plotOneBitValueTimes->xValueTime.vecTime.push_back((everyTime - mLeftTime ) / mAmplificationFactor );
                    } else {
                        plotOneBitValueTimes->xValueTime.vecTime.push_back(maxAxisRealTimeLen / mAmplificationFactor );
                    }
                } else {
                    plotOneBitValueTimes->xValueTime.vecTime.push_back(0);
                }

                if(lastValueType != isX && lastValueType != firstData){
                    plotOneBitValueTimes->xValueTime.vecValue.push_back(std::nan(""));
                    plotOneBitValueTimes->xValueTime.vecTime.push_back(plotOneBitValueTimes->xValueTime.vecTime.back());
                }  // 如果是x，无需处理。

                plotOneBitValueTimes->xValueTime.vecValue.push_back(0.5 + order);

                switch (lastValueType){
                case isZeroOr1 :{
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->xValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.back());

                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->xValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(0.5  + order);
                    break;
                }
                case isZ:{
                    // 这个分支应该不会进入，没有此种组合
                    plotOneBitValueTimes->zValueTime.vecTime.push_back(plotOneBitValueTimes->xValueTime.vecTime.back());
                    plotOneBitValueTimes->zValueTime.vecValue.push_back(0.5 + order);

                    plotOneBitValueTimes->zValueTime.vecTime.push_back(plotOneBitValueTimes->xValueTime.vecTime.back());
                    plotOneBitValueTimes->zValueTime.vecValue.push_back(0.5 + order);
                    break;
                }
                case firstData:
                case isX:
                default:{
                    break;
                }
                }

                lastValueType = isX;
            }else if(signalValueTime->vecValue[j].at(index) == 'z' || signalValueTime->vecValue[j].at(index) == 'Z'){
                if(everyTime > mLeftTime) {
                    if(everyTime < maxAxisRealTimeLen) {
                        plotOneBitValueTimes->zValueTime.vecTime.push_back((everyTime - mLeftTime ) / mAmplificationFactor );
                    } else {
                        plotOneBitValueTimes->zValueTime.vecTime.push_back(maxAxisRealTimeLen / mAmplificationFactor );
                    }
                } else {
                    plotOneBitValueTimes->zValueTime.vecTime.push_back(0);
                }

                if(lastValueType != isZ && lastValueType != firstData){
                    plotOneBitValueTimes->zValueTime.vecValue.push_back(std::nan(""));
                    plotOneBitValueTimes->zValueTime.vecTime.push_back(plotOneBitValueTimes->zValueTime.vecTime.back());
                }
                plotOneBitValueTimes->zValueTime.vecValue.push_back(0.5 + order);

                switch (lastValueType){
                case isZeroOr1 :{
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->zValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.back());

                    plotOneBitValueTimes->zeroAnd1ValueTime.vecTime.push_back(plotOneBitValueTimes->zValueTime.vecTime.back());
                    plotOneBitValueTimes->zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                    break;
                }
                case isX:{
                    plotOneBitValueTimes->xValueTime.vecTime.push_back(plotOneBitValueTimes->zValueTime.vecTime.back());
                    plotOneBitValueTimes->xValueTime.vecValue.push_back(0.5 + order);

                    plotOneBitValueTimes->xValueTime.vecTime.push_back(plotOneBitValueTimes->zValueTime.vecTime.back());
                    plotOneBitValueTimes->xValueTime.vecValue.push_back(0.5 + order);
                    break;
                }
                case firstData:
                case isZ:
                default:{
                    break;
                }
                }

                lastValueType = isZ;

            }
        }

        if(everyTime > maxAxisRealTimeLen){
            break;
        }
    }
    vecPlotValueTime.push_back(plotOneBitValueTimes);
}

void VCDVision::refreshMoreOneBitValueTime( uint64_t maxAxisRealTimeLen, VCDSignal *signal, QVector<PlotValueTimes*> &vecPlotValueTime, PlotMoreOneBitValueTimes *plotMoreOneBitValueTimes ){

    size_t order = vecPlotValueTime.size();
    LastValueType lastValueType = firstData;
    uint64_t everyTime = 0;
    uint64_t nextEveryTime = 0;
    ValueTime *signalValueTime = mHashSignalValue->find(signal->idCode).value();
    double gap = 1;
    LastValueType vectorLastValueType = firstData;

    QString lastValue = "";
    //mTextItem 用来检测lastValue的字符宽度。
    size_t vecTimeSize = signalValueTime->vecTime.size();
    for(size_t i = 0; i < vecTimeSize; ++i){

        everyTime = signalValueTime->vecTime[i];

        if (i+1< signalValueTime->vecTime.size() ) {
            nextEveryTime = signalValueTime->vecTime[i + 1];
        }

        if(mLeftTime > everyTime &&  mLeftTime> nextEveryTime ) {
            continue;
        } else {
            QString value = signalValueTime->vecValue[i];
            value = binary2Hex(value, signal->signalValueSize);

            int zNumber = value.count('z', Qt::CaseInsensitive);
            int xNumber = value.count('x', Qt::CaseInsensitive);

            if(zNumber == 0 && xNumber == 0) {

                if(vectorLastValueType == isZeroOr1){
                    plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);
                    plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);

                    plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                    plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);

                    if(i < vecTimeSize - 1){  // 结尾的处理
                        // 连接延长
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);

                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                    }

                } else {
                    if(vectorLastValueType == isX){
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(std::nan(""));

                    }else if(vectorLastValueType == isZ){
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(std::nan(""));
                    }

                    if(mLeftTime <= everyTime){   // 处理起始点，防止溢出。
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        if(i < vecTimeSize - 1){
                            plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);
                            plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                        }

                    }else{
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                    }
                }

                vectorLastValueType = isZeroOr1;
            }else if(xNumber > 0){
                if(vectorLastValueType == isX){
                    plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);
                    plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.5 + order);

                    plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                    plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.5 + order);

                    if(mLeftTime < everyTime){
                        // 连接延长
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);

                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                    }

                } else {
                    if(vectorLastValueType == isZeroOr1){
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(std::nan(""));

                    }else if(vectorLastValueType == isZ){
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(std::nan(""));
                    }
                    if(mLeftTime <= everyTime){   // 处理起始点，防止溢出。
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.5 + order);
                        if(i < vecTimeSize - 1){
                            plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);
                            plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                        }
                    }else{
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                    }
                }
                vectorLastValueType = isX;
            } else {
                if(vectorLastValueType == isZ){
                    plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);
                    plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.5 + order);

                    plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                    plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                    plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.5 + order);

                    if(i < vecTimeSize - 1){
                        // 连接延长
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);

                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                    }

                } else {
                    if(vectorLastValueType == isZeroOr1){
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zeroAnd1ValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zeroAnd1ValueTime.vecValue.push_back(std::nan(""));

                    }else if(vectorLastValueType == isX){
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back(  ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.xValueTime.vecValue.push_back(std::nan(""));

                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back( ((signalValueTime->vecTime.at(i) - gap) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.9 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.xValueTime.vecValue.push_back(std::nan(""));
                    }
                    if(mLeftTime <= everyTime){   // 处理起始点，防止溢出。
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.5 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.5 + order);
                        if(i < vecTimeSize - 1){
                            plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);
                            plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back((signalValueTime->vecTime.at(i) + gap - mLeftTime) / mAmplificationFactor);
                            plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                        }
                    }else{
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->topPlot.zValueTime.vecValue.push_back(0.1 + order);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecTime.push_back(0);
                        plotMoreOneBitValueTimes->downPlot.zValueTime.vecValue.push_back(0.9 + order);
                    }
                }
                vectorLastValueType = isZ;
            }

            //检测此字符是否有必要显示。
            if(plotMoreOneBitValueTimes->vecText.size() > 0){
                mTextItem->setText(lastValue);
                QFontMetrics fontMetrics(mTextItem->font());
                int textWidthPixel = fontMetrics.horizontalAdvance(mTextItem->text());
                double periodLen = ((signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor - plotMoreOneBitValueTimes->vecText.back().key )  *mXAxisUnitLengthPixelWidth;

                if(i < vecTimeSize - 1 && everyTime <= maxAxisRealTimeLen){
                    if(2 * textWidthPixel  <= periodLen){
                        plotMoreOneBitValueTimes->vecText.back().key +=  textWidthPixel / 4 + gap / mAmplificationFactor ;
                        plotMoreOneBitValueTimes->vecText.push_back({value, (signalValueTime->vecTime.at(i)  - mLeftTime) / mAmplificationFactor , 0.5 + order});
                    }else{
                        plotMoreOneBitValueTimes->vecText.back() = {value, (signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor , 0.5 + order};
                    }
                }else{  // 整个波形的最后一个 收口不需要显示值
                    if(2 * textWidthPixel <= periodLen){
                        plotMoreOneBitValueTimes->vecText.back().key +=  textWidthPixel / 4  + gap / mAmplificationFactor ;
                    }else{
                        plotMoreOneBitValueTimes->vecText.pop_back();
                    }
                }
            }else{
                plotMoreOneBitValueTimes->vecText.push_back({value, (signalValueTime->vecTime.at(i) - mLeftTime) / mAmplificationFactor , 0.5 + order});
            }

            lastValue = value;
        }
        if(everyTime > maxAxisRealTimeLen){
            break;
        }
    }
    mTextItem->setText("");
    vecPlotValueTime.push_back(plotMoreOneBitValueTimes);
}

void VCDVision::refreshWaveValue(){
    mVecPlotValueTime.clear();

    uint64_t maxAxisRealTimeLen = mLeftTime + mAxisLen;
    LastValueType lastValueType = firstData;
    uint64_t everyTime = 0;
    uint64_t nextEveryTime = 0;
    ValueTime *everysSignalValueTime = nullptr;
    size_t realndex = 0;
    size_t oneBitIndex = 0;
    size_t oneBitSize = mVecOneBitvalueTime.size();
    size_t moreOneBitIndex = 0;
    size_t moreOneBitSize = mVecMoreOneBitValueTime.size();

    for(size_t i = 0; i < mVecSignalOrder.size(); ++i){
        if(mVecSignalOrder[i]->signalValueSize == 1){
            if(realndex >= mNameValueTableBeginIndex && realndex < mNameValueTableBeginIndex + mNameValueTableSize){

                PlotOneBitValueTimes *plotOneBitValueTimes = nullptr ;
                if(oneBitIndex < oneBitSize){
                    plotOneBitValueTimes = mVecOneBitvalueTime.at(oneBitIndex);
                    plotOneBitValueTimes->clear();
                    oneBitIndex++;
                }else{
                    plotOneBitValueTimes = new PlotOneBitValueTimes ;
                    mVecOneBitvalueTime.push_back(plotOneBitValueTimes);
                    plotOneBitValueTimes->plotValueType = oneBit;
                }

                // 不支持的数据，或异常数据。
                auto it = mHashSignalValue->find(mVecSignalOrder[i]->idCode);
                if(it == mHashSignalValue->end()){
                    mVecPlotValueTime.push_back(nullptr);
                    continue;
                }
                ValueTime *signalValueTime = it.value();
                refreshOneBitValueTime(maxAxisRealTimeLen, signalValueTime, mVecPlotValueTime, plotOneBitValueTimes);
            }
            realndex++;
        }else{
            if(realndex >= mNameValueTableBeginIndex && realndex < mNameValueTableBeginIndex + mNameValueTableSize){

                PlotMoreOneBitValueTimes *plotMoreOneBitValueTimes = nullptr;
                if(moreOneBitIndex < moreOneBitSize){
                    plotMoreOneBitValueTimes = mVecMoreOneBitValueTime.at(moreOneBitIndex);
                    plotMoreOneBitValueTimes->clear();
                    moreOneBitIndex++;
                }else{
                    plotMoreOneBitValueTimes = new PlotMoreOneBitValueTimes ;
                    mVecMoreOneBitValueTime.push_back(plotMoreOneBitValueTimes);
                    plotMoreOneBitValueTimes->plotValueType = moreOneBit;
                }

                refreshMoreOneBitValueTime(maxAxisRealTimeLen, mVecSignalOrder[i], mVecPlotValueTime, plotMoreOneBitValueTimes );
            }
            realndex++;
            if(mVecSignalOrder[i]->expandDisplay){
                // 不支持的数据，或异常数据。
                auto it = mHashSignalValue->find(mVecSignalOrder[i]->idCode);
                if(it == mHashSignalValue->end()){
                    mVecPlotValueTime.push_back(nullptr);
                    continue;
                }
                ValueTime *signalValueTime = it.value();

                for(size_t j = 0; j < mVecSignalOrder[i]->signalValueSize; ++j){

                    if(realndex >= mNameValueTableBeginIndex && realndex < mNameValueTableBeginIndex + mNameValueTableSize){
                        // 池。
                        PlotOneBitValueTimes *plotOneBitValueTimes = nullptr ;
                        if(oneBitIndex < oneBitSize){
                            plotOneBitValueTimes = mVecOneBitvalueTime.at(oneBitIndex);
                            plotOneBitValueTimes->clear();
                            oneBitIndex++;
                        }else{
                            plotOneBitValueTimes = new PlotOneBitValueTimes ;
                            mVecOneBitvalueTime.push_back(plotOneBitValueTimes);
                            plotOneBitValueTimes->plotValueType = oneBit;
                        }


                        refreshOneBitValueTime(maxAxisRealTimeLen, signalValueTime, mVecPlotValueTime, plotOneBitValueTimes, j);
                    }
                    realndex++;
                }
            }
        }
    }

    mVCDCustomPlot->setVCDGraph(mVecPlotValueTime);
}

void VCDVision::dragWave(QMouseEvent *event, const QPointF &startPos){

    QCPAxis *ax = mVCDCustomPlot->xAxis2;
    double diff = (ax->pixelToCoord(startPos.x()) - ax->pixelToCoord(event->pos().x()))  *mAmplificationFactor  *mMoveFactor;
    qDebug() << "diff" << diff;
    if(diff < 0){  // left
        uint64_t leftTickLen = (- diff) ;
        if(mLeftTime > leftTickLen) {
            mLeftTime = mLeftTime - leftTickLen;
        } else {
            mLeftTime = 0;
        }
    } else if(diff > 0) {    // right
        mLeftTime = mLeftTime + diff;
    } else {
        return;
    }

    setTracer();
    plotWave( );
}

void VCDVision::setAxis(){
    double reducetionFactor = std::pow(1000, mIndexTimeUint - mInitialTimeUint );
    QString timeUint = mVecAllTimeUint[mIndexTimeUint];
    QVector<QString> tickVectorLabels;
    for(int i = 0; i <= mVCDCustomPlot->getTickCount(); ++i){
        QString tickLable = QString::number( (mLeftTime / reducetionFactor + i *( mAxisLen / reducetionFactor /10))) ;
        tickVectorLabels.push_back(tickLable + timeUint);
    }

    VCDCPAxis *xAxis2 = static_cast<VCDCPAxis*>(mVCDCustomPlot->xAxis2);
    if(xAxis2) {
        xAxis2->setTickVectorLabels(tickVectorLabels);
    }
    VCDCPAxis *yAxis = static_cast<VCDCPAxis*>(mVCDCustomPlot->yAxis);
    yAxis->setTickVectorLabels(tickVectorLabels);
}

void VCDVision::cursorDataRetrieval(QMouseEvent *event, const QVariant &details){
    Q_UNUSED(details)
    mShowTracer = true;
    double positionLen = mVCDCustomPlot->xAxis2->pixelToCoord(event->pos().x());
    mTracerPositionValueTime = mLeftTime + positionLen  *mAmplificationFactor ;  //  取附近的整数，因为VCD的时间是不连续的。
    setTracer();
    mVCDCustomPlot->replot();
    refreshSignalValue();
}

void VCDVision::setTracer(){
    if(!mShowTracer) {
        mVCDCustomPlot->setTracer(-10, QString("0"));
        return;
    }
    if(mTracerPositionValueTime < mLeftTime || mTracerPositionValueTime > mLeftTime + mAxisLen) {
        mVCDCustomPlot->setTracer(-10, QString("0"));
    } else {
        double reducetionFactor = std::pow(1000, mIndexTimeUint - mInitialTimeUint );
        QString positionValueTimeStr = QString::number(mTracerPositionValueTime / reducetionFactor) + mVecAllTimeUint[mIndexTimeUint];  //坐标轴时间
        double trickerPosition = (mTracerPositionValueTime - mLeftTime) / mAmplificationFactor;   // 坐标轴位置
        mVCDCustomPlot->setTracer(trickerPosition, std::move(positionValueTimeStr ));
    }
}










