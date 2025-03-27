#ifndef VCDVISION_H
#define VCDVISION_H
#include <QWidget>
#include "VCDCustomPlot.h"

/*
//  value name ||   value ||  wave                              ||
.                      .              .                                        .
.                      .              .                                        .
.                      .              .                                        .
.                      .              .                                        .
*/

class VCDVision : public QWidget {

    Q_OBJECT
public:
    static VCDVision *getInstance();
    int process(const QString &jsonFilePath);

protected:

    int processValue(const QString &mJsonFilePath);
    void establishLayout();
    void zoomWave(QWheelEvent *event);
    void dragWave(QMouseEvent *event, const QPointF &startPos);
    void cursorDataRetrieval(QMouseEvent *event, const QVariant &details);
    //因为尺寸改变调整对齐方式。
    void resizeAxisRect();
    void wheelTable(QWheelEvent *event);
    void resizeVCDTableWidget(QResizeEvent *event);

    enum LastValueType { firstData, isZeroOr1, isX, isZ};
    int processVCDJsonFile(const QString &jsonFilePath);
    void setWaveSignalName(QWidget *parent);
    void refreshWaveSignalName(bool refreshSignalNumber = true);
    void setWaveSignalValue(QWidget *parent);
    void refreshSignalValue(bool refreshSignalNumber = true);
    int getSignalWaveNumber();
    void setWave(QWidget *parent);
    void setButton(QWidget *parent);

    void plotWave();
    void prepareScaleData (double factor = 1, double centor = 5000);
    void setAxis();
    void setTimeUintAndFactor();

    void setTracer();
    void refreshWaveValue();
    QString binary2Hex(QString &binValue, int width);
    void refreshOneBitValueTime(uint64_t maxAxisRealTimeLen, ValueTime *signalValueTime, QVector<PlotValueTimes*> &vecPlotValueTime, PlotOneBitValueTimes *plotOneBitValueTimes, int index = 0 );
    void refreshMoreOneBitValueTime(uint64_t maxAxisRealTimeLen, VCDSignal *signal, QVector<PlotValueTimes*> &vecPlotValueTime, PlotMoreOneBitValueTimes *plotMoreOneBitValueTimes);

    void setAppearanceSizeRatio();
    void reset();
    void updateItem( VCDTableWidget* table, int totalRows, QTableWidgetItem *&everyQTableWidgetItem, int row, QString &text);
    void emptyRemainItem(VCDTableWidget* table, int totalRows,  int row);

private slots:
    void onCellDoubleClicked(QTableWidgetItem *item);


signals:
    void error(const QString errorInformation);

private:
    VCDVision(QWidget *parent = nullptr);
    virtual ~VCDVision();
    static VCDVision *mInstance;
    VCDCustomPlot *mVCDCustomPlot = nullptr;

    // 控件
    QVBoxLayout *mVLayout  = nullptr;
    QSplitter *mSplitter = nullptr;

    QPushButton *mButtonReset = nullptr;

    VCDTableWidget *mTableNameWidget = nullptr;
    VCDTableWidget *mTableValueWidget = nullptr;

    const TimeScale *mVcdTimeScale ;
    const QVector<VCDScope*> *mTopScope;
    const QHash<QString , ValueTime*> *mHashSignalValue;
    QVector<VCDSignal*> mVecSignalOrder;

    double mAmplificationFactor = 1;
    uint64_t mLeftTime = 0;
    uint64_t mAxisLen = 100;

    const  QVector<QString> mVecAllTimeUint = {"fs", "ps", "ns", "us", "ms", "s"};  // 此处顺序应该和enum TimeUnit{ fs, ps, ns, us, ms, s}; 一致
    int mInitialTimeUint = 1;
    int mIndexTimeUint = 1;

    double mUnitZoom = 1;
    const double mMoveFactor = 0.5;

    //textItem
    QCPLayer *mTopLayer = nullptr;
    const QString mTopLayerTextItem = "topLayerTextItem";

    // tracer
    uint64_t mTracerPositionValueTime = 0;
    bool mShowTracer = false;

    // 记录需要展开的信号
    QHash<QTableWidgetItem*, VCDSignal*> mHashMoreOneBitSignal;

    int mNameValueTableBeginIndex;
    int mNameValueTableSize;
    QCPItemText *mTextItem = nullptr;  // 用来计算此对象的字符宽度。
    double mXAxisUnitLengthPixelWidth = 4.1;

    QVector<PlotValueTimes*> mVecPlotValueTime;  // 池

    QVector<PlotOneBitValueTimes*> mVecOneBitvalueTime;  // 池
    QVector<PlotMoreOneBitValueTimes*> mVecMoreOneBitValueTime; // 池

};

#endif //VCDVISION_H
