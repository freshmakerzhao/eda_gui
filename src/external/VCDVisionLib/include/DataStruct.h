#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QVector>
#include <QHash>
#include <QTableWidgetItem>

struct  ValueTime {
    QVector<uint64_t> vecTime;
    QVector<QString> vecValue;
};

struct QCPTextInput{
    QString text;
    double key;
    double value;
};

enum PlotValueType {oneBit,  moreOneBit };

struct  PlotValueTime {
    QVector<double> vecTime;
    QVector<double> vecValue;
    void clear(){
        vecTime.clear();
        vecValue.clear();
    }
};

struct PlotValueTimes{
    PlotValueType plotValueType;
};

struct PlotOneBitValueTimes : PlotValueTimes{
    PlotValueTime zeroAnd1ValueTime;
    PlotValueTime xValueTime;
    PlotValueTime zValueTime;
    void clear(){
        zeroAnd1ValueTime.clear();
        xValueTime.clear();
        zValueTime.clear();
    }
};

struct PlotMoreOneBitValueTimes : PlotValueTimes{
    PlotOneBitValueTimes topPlot;
    PlotOneBitValueTimes downPlot;
    std::vector<QCPTextInput> vecText;
    void clear(){
        topPlot.clear();
        downPlot.clear();
        vecText.clear();
    }

};

enum TimeUnit{
    fs, ps, ns, us, ms, s
};  // 此处顺序应该和 const  QVector<QString> mVecAllTimeUint = {"fs", "ps", "ns", "us", "ms", "s"};一致

const  QHash<QString, TimeUnit> hashTimeUnitConvert = {{"s", TimeUnit::s }, {"ms", TimeUnit::ms}, {"us", TimeUnit::us}, {"ns", TimeUnit::ns}, {"ps", TimeUnit::ps}, {"fs", TimeUnit::fs} };

struct TimeScale {
    int magnitude = 0;
    TimeUnit vcdTimeUnit = TimeUnit::s;
};

struct ValueChange {
    uint64_t time;
    QString value;
};

struct VCDSignal {
    QString signalType;
    QString signalName;
    QString idCode;
    int signalValueSize;
    int signaIndexLeft;
    int signaIndexRight;
    bool expandDisplay = false;   // 波形展示时使用。
};

struct VCDScope {
    QVector<VCDScope*> childScope;
    QString scopeName;
    QString scopeType;
    QVector<VCDSignal*> vecSignals;
};


#endif // DATASTRUCT_H
