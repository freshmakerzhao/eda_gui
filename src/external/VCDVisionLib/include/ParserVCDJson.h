#ifndef PARSERVCDJSON_H
#define PARSERVCDJSON_H
#include <QString>
#include <QVector>
#include <QHash>
#include <QJsonValue>
#include "DataStruct.h"


class ParserVCDJson : public QObject  {

    Q_OBJECT
public:
    static ParserVCDJson &instance();

    int parser(const QString &jsonPath );
    void clean();

    inline const  TimeScale *getTimeScale() {return &mVcdTimeScale; }
    inline const  QVector<VCDScope*> *getTopScope() {return &mTopScope;}
    inline const  QHash<QString , ValueTime*> *getHashSignalValue() {return &mHashSignalValue;}
    inline const uint64_t getMaxTime(){return mMaxTime;};

signals:
    void error(const QString errorInformation);

protected:
    template <typename T>
    int jsonValueRefOrValue2String(const T &value, QString &str, const QString &&errorInformation);
    template <typename T>
    int jsonValueRefOrValue2JsonArray(const T &value, QJsonArray &array, const QString &&errorInformation);
    template <typename T>
    int jsonValueRefOrValue2Int(const T &value, int &num, const QString &&errorInformation);

    int binaryPad(QString& decValue, int width, QString &allBitValue);

private:
    ParserVCDJson();
    virtual ~ParserVCDJson();

    int parserTimeScale(QJsonValue &timeScale);
    int parserScope(QJsonValue &scopeListObj);
    int parserTimeChangeBlocks(QJsonValue &timeChangeListOb);


    TimeScale mVcdTimeScale ;
    QVector<VCDScope*> mTopScope;
    QHash<QString , ValueTime*> mHashSignalValue;
    uint64_t mMaxTime = 0;
};


#endif // PARSERVCDJSON_H
