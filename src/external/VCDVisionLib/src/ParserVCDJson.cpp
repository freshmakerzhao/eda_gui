#include "ParserVCDJson.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QQueue>


// enum Type {
//     Null =  0x0,
//     Bool = 0x1,
//     Double = 0x2,
//     String = 0x3,
//     Array = 0x4,
//     Object = 0x5,
//     Undefined = 0x80
// };
// 为了报错使用。
QString enumToString(QJsonValue::Type type) {
    switch (type) {
    case QJsonValue::Null:
        return "Null";
    case QJsonValue::Bool:
        return "Bool";
    case QJsonValue::Double:
        return "Double";
    case QJsonValue::String:
        return "String";
    case QJsonValue::Array:
        return "Array";
    case QJsonValue::Object:
        return "Object";
    case QJsonValue::Undefined:
        return "Undefined";
    default:
        return "Unknown";
    }
}

ParserVCDJson &ParserVCDJson::instance(){
    static ParserVCDJson instance;
    return instance;
}


ParserVCDJson::ParserVCDJson() : QObject(){

}

ParserVCDJson::~ParserVCDJson(){

}

void ParserVCDJson::clean(){

    mVcdTimeScale.magnitude = 0;
    mVcdTimeScale.vcdTimeUnit = TimeUnit::s;
    mMaxTime = 0;

    for(auto it : mTopScope) {
        QQueue<VCDScope*> queueScope;
        queueScope.push_back(it);
        while(!queueScope.empty()) {
            VCDScope *currentScope = queueScope.head();
            queueScope.pop_front();
            for(auto chileScopeIt : currentScope->childScope) {
                queueScope.push_back(chileScopeIt);
            }
            for(auto signalIt : currentScope->vecSignals) {
                delete signalIt;
            }
            delete currentScope;
        }

    }
    mTopScope.clear();

    for(auto it = mHashSignalValue.cbegin(); it != mHashSignalValue.cend(); ++it)
    {
        delete it.value();
    }
    mHashSignalValue.clear();
}




int ParserVCDJson::parser(const QString &jsonPath ) {
    clean();
    QFile file(jsonPath);
    if (file.open(QIODevice::ReadOnly)) {
        qDebug() << "VCDJson open successful.";
        QJsonParseError jsonError;
        // 将字节数组转换为JSON文档
        QJsonDocument jsonVCDDoc = QJsonDocument::fromJson(file.readAll(), &jsonError);
        if(jsonError.error == QJsonParseError::NoError) {
            if(jsonVCDDoc.isObject()) {
                QJsonObject jsonVCDObj  =  jsonVCDDoc.object();
                //  timescale
                QJsonValue timeScale = jsonVCDObj["timeScale"];
                if(parserTimeScale(timeScale)){
                    return -1;
                }
                //  scope
                QJsonValue scopeListObj = jsonVCDObj["scopeList"];
                if(parserScope(scopeListObj)){
                    return -1;
                }
                //  timeChangeBlocks
                QJsonValue timeChangeListObj = jsonVCDObj["timeChangeBlocks"];
                if(parserTimeChangeBlocks(timeChangeListObj)){
                    return -1;
                }
            }else{
                emit error("The VCDjson file isn't json object.");
                return -1;
            }
        }else{
            emit error(QString("JSON parsing error: ") + jsonError.errorString());
            file.close();
            return -1; // 返回错误码
        }
    }else{
        emit error(QString("VCDJson open filed, error information:") + file.errorString());
        return -1;
    }


    for(auto it = mHashSignalValue.cbegin(); it != mHashSignalValue.cend(); ++it)
    {
        if(it.value()->vecTime.back()< mMaxTime){
            it.value()->vecTime.append(mMaxTime);
            it.value()->vecValue.append(it.value()->vecValue.back());
        }
    }

    QQueue<VCDScope*> queueScope;
    QSet<ValueTime*> setSignal;
    for(auto it : mTopScope){
        queueScope.push_back(it);
    }

    while(!queueScope.empty()){
        VCDScope *everyScope = queueScope.head();
        queueScope.pop_front();
        for(auto it : everyScope->childScope){
            queueScope.push_back(it);
        }
        for(auto everySignal : everyScope->vecSignals){
            if(everySignal->signalValueSize > 1){
                auto valueTime = mHashSignalValue.find(everySignal->idCode);
                if(valueTime != mHashSignalValue.end() && setSignal.find(valueTime.value()) == setSignal.end()){
                    setSignal.insert(valueTime.value());
                    for(auto& everyValue : valueTime.value()->vecValue){
                        QString allBitValue;
                        binaryPad(everyValue, everySignal->signalValueSize, allBitValue);
                        everyValue = allBitValue;
                    }
                }
            }
        }
    }

    qDebug()  << "Parser VCDFile successful" ;
    return 0;
}

int ParserVCDJson::parserTimeScale(QJsonValue &timeScale){
    if(jsonValueRefOrValue2Int(timeScale["magnitude"], mVcdTimeScale.magnitude, QString("Magnitude isn't double,") + "<p>it's " +  enumToString(timeScale["magnitude"].type()) + ". </p>")){
        return -1;
    }

    QString unit;
    if(jsonValueRefOrValue2String(timeScale["unit"], unit, QString("unit isn't string,") + "<p>it's " +  enumToString(timeScale["unit"].type()) + ". </p>" )){
        return  -1;
    }
    mVcdTimeScale.vcdTimeUnit = hashTimeUnitConvert[unit];
    return 0;
}

int ParserVCDJson::parserScope(QJsonValue &scopeListObj){
    QJsonArray arrayVCDScope ;
    if(jsonValueRefOrValue2JsonArray(scopeListObj, arrayVCDScope, QString("ScopeListObj isn't array,") + "<p>it's " +   enumToString(scopeListObj.type()) + ". </p>")){
        return -1;
    }
    QVector<VCDScope*> vecScoped;  //depth
    for( auto scopeIt : arrayVCDScope){
        QJsonObject everyVcdScopeObj = scopeIt.toObject();
        QString scopeType;
        if(jsonValueRefOrValue2String(everyVcdScopeObj["scopeType"], scopeType, QString("scopeType isn't string,") + "<p>it's " +   enumToString(everyVcdScopeObj["scopeType"].type()) + ". </p>" )){
            return  -1;
        }

        if(scopeType == "UPSCOPE") {
            vecScoped.pop_back();
            continue;
        }

        VCDScope *everyVcdScope = new VCDScope();

        everyVcdScope->scopeType = scopeType;
        if(jsonValueRefOrValue2String(everyVcdScopeObj["moduleName"], everyVcdScope->scopeName, QString("moduleName isn't string,") + "<p>it's " +   enumToString(everyVcdScopeObj["moduleName"].type()) + ". </p>" )){
            return  -1;
        }

        if(vecScoped.empty()){
            mTopScope.push_back(everyVcdScope);
        }else {
            vecScoped.back()->childScope.push_back(everyVcdScope);
        }
        vecScoped.push_back(everyVcdScope);

        QJsonArray scopeSignalObj;
        if(jsonValueRefOrValue2JsonArray(everyVcdScopeObj["listSignal"], scopeSignalObj, QString("ListSignal isn't array,") + "<p>it's " +   enumToString(everyVcdScopeObj["listSignal"].type()) + ". </p>")){
            return -1;
        }

        for(auto signalIt : scopeSignalObj) {
            QJsonObject everyScopeSignalObj = signalIt.toObject();
            VCDSignal *everySignal = new VCDSignal();
            if(jsonValueRefOrValue2String(everyScopeSignalObj["signalType"], everySignal->signalType, QString("SignalType isn't string,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalType"].type()) + ". </p>" )){
                return  -1;
            }

            if(jsonValueRefOrValue2String(everyScopeSignalObj["signalReference"], everySignal->signalName, QString("SignalName isn't string,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalReference"].type()) + ". </p>" )){
                return  -1;
            }

            if(jsonValueRefOrValue2String(everyScopeSignalObj["signalIdCode"], everySignal->idCode, QString("IdCode isn't string,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalIdCode"].type()) + ". </p>" )){
                return  -1;
            }

            if(jsonValueRefOrValue2Int(everyScopeSignalObj["signalSize"], everySignal->signalValueSize, QString("SignalSize isn't double,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalIdCode"].type()) + ". </p>")){
                    return -1;
            }

            if(jsonValueRefOrValue2Int(everyScopeSignalObj["signalIndexLeft"], everySignal->signaIndexLeft, QString("SignalIndexLeft isn't double,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalIndexLeft"].type()) + ". </p>")){
                return -1;
            }

            if(jsonValueRefOrValue2Int(everyScopeSignalObj["signalIndexRight"], everySignal->signaIndexRight, QString("SignalIndexRight isn't double,") + "<p>it's " +  enumToString(everyScopeSignalObj["signalIndexRight"].type()) + ". </p>")){
                return -1;
            }

            everyVcdScope->vecSignals.append(everySignal);
        }
    }
    return 0;
}

int ParserVCDJson::parserTimeChangeBlocks(QJsonValue &timeChangeListObj){
    QJsonArray arrayTimeChange;
    if(jsonValueRefOrValue2JsonArray(timeChangeListObj, arrayTimeChange, QString("TimeChangeList of time change value block isn't array,") + "<p>it's " +  enumToString(timeChangeListObj.type()) + ". </p>")){
        return -1;
    }
    for(auto timeIt : arrayTimeChange) {
        QJsonObject everyTimeChangeObj = timeIt.toObject();

        QString timeStr;
        if(jsonValueRefOrValue2String(everyTimeChangeObj["time"], timeStr, QString("Time of time change value block isn't string,") + "<p>it's " + enumToString(everyTimeChangeObj["time"].type()) + ". </p>" )){
            return  -1;
        }

        bool ok;
        uint64_t time = timeStr.toULongLong(&ok);
        if(!ok){
            emit error(QString("Time of time change value block isn't integer number,") + "<p>it's " +  timeStr );
            return -1;
        }

        mMaxTime = time > mMaxTime? time : mMaxTime;
        QJsonArray signalValue;
        if(jsonValueRefOrValue2JsonArray(everyTimeChangeObj["listSignalValue"], signalValue, QString("ListSignalValue of time change value block isn't array,") + "<p>it's " +  enumToString(everyTimeChangeObj["listSignalValue"].type()) + ". </p>")){
            return -1;
        }

        for(auto signalValueIt : signalValue) {
            QJsonObject everySignalValueObj = signalValueIt.toObject();
            QString idCode;
            if(jsonValueRefOrValue2String(everySignalValueObj["idCode"], idCode, QString("IdCode of time change value block isn't string,") + "<p>it's " +  enumToString(everySignalValueObj["idCode"].type()) + ". </p>" )){
                return  -1;
            }

            auto idCodeIt = mHashSignalValue.find(idCode);
            ValueTime *valueTimeObj = nullptr;
            if(idCodeIt == mHashSignalValue.end() )  {
                valueTimeObj = new ValueTime();
                mHashSignalValue.insert(idCode, valueTimeObj);
            } else {
                valueTimeObj = idCodeIt.value();
            }
            valueTimeObj->vecTime.push_back(time) ;

            QString value;
            if(jsonValueRefOrValue2String(everySignalValueObj["value"], value, QString("Value of time change value block isn't string,") + "<p>it's " +  enumToString(everySignalValueObj["value"].type()) + ". </p>")){
                return  -1;
            }
            valueTimeObj->vecValue.push_back(value);
        }
    }
    return 0;
}

template <typename T>
int ParserVCDJson::jsonValueRefOrValue2String(const T &value, QString &str, const QString &&errorInformation){
    if(value.isString()) {
        str = value.toString();
    } else {
        emit error(errorInformation);
        return -1;
    }
    return 0;
}

template <typename T>
int ParserVCDJson::jsonValueRefOrValue2JsonArray(const T &value, QJsonArray &array, const QString &&errorInformation){
    if(value.isArray()) {
        array = value.toArray();
    } else {
        emit error(errorInformation);
        return -1;
    }
    return 0;
}

template <typename T>
int ParserVCDJson::jsonValueRefOrValue2Int(const T &value, int &num, const QString &&errorInformation){
    if(value.isDouble()) {
        num = value.toInt();
    } else {
        emit error(errorInformation);
        return -1;
    }
    return 0;
}

int ParserVCDJson::binaryPad(QString& decValue, int width, QString &allBitValue){
    QString temp(width, '0');
    if(decValue == "x" || decValue == "z"){
        temp.fill(decValue.at(0), width);
    }else if(decValue.count('z', Qt::CaseInsensitive) || decValue.count('x', Qt::CaseInsensitive)){
        allBitValue = decValue;
        qDebug() << "The value of decValue is not as expected:" << decValue;
    }else{
        int decimalValue  = decValue.toInt();
        QString binaryValue = QString::number(decimalValue, 2);
        size_t offset = width - binaryValue.size();
        if(binaryValue.size() > width){
            QString zValue(width, 'z');
            allBitValue = zValue;
            emit error(QString("The bit width of binary numbers is greater than the expected width. ") + "width:" + width + "binaryValue:" + binaryValue + "Z will be used to assign values");
            return -1;
        }
        std::copy(binaryValue.begin(), binaryValue.end(), temp.begin() + offset);
    }
    allBitValue = temp;
    return 0;
}
