import io
from vcd import reader
import json
import sys


# 类定义，添加__init__方法初始化属性
class signal:
    def __init__(self, signalType, signalReference, signalIdCode, signalSize, signalIndexLeft, signalIndexRight):
        self.signalType = signalType
        self.signalReference = signalReference
        self.signalIdCode = signalIdCode
        self.signalSize = signalSize
        self.signalIndexLeft = signalIndexLeft
        self.signalIndexRight = signalIndexRight


class scopeType:
    def __init__(self, scopeType, moduleName, listSignal):
        self.scopeType = scopeType
        self.moduleName = moduleName
        self.listSignal = listSignal


class signalValue:
    def __init__(self, idCode, value):
        self.idCode = idCode
        self.value = value


class timeChangeBlock:
    def __init__(self, time, listSignalValue):
        self.time = time
        self.listSignalValue = listSignalValue


class timeScale:
    def __init__(self, magnitude, unit):
        self.magnitude = magnitude
        self.unit = unit


# 自定义JSON编码器类，继承自json.JSONEncoder
class CustomJSONEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, timeScale):
            return {
                "magnitude": o.magnitude,
                "unit": o.unit
            }
        elif isinstance(o, signal):
            return {
                "signalType": o.signalType,
                "signalReference": o.signalReference,
                "signalIdCode": o.signalIdCode,
                "signalSize": o.signalSize,
                "signalIndexLeft": o.signalIndexLeft,
                "signalIndexRight": o.signalIndexRight
            }
        elif isinstance(o, scopeType):
            signals_list = []
            for signal_obj in o.listSignal:
                signals_list.append({
                    "signalType": signal_obj.signalType,
                    "signalReference": signal_obj.signalReference,
                    "signalIdCode": signal_obj.signalIdCode,
                    "signalSize": signal_obj.signalSize,
                    "signalIndexLeft": signal_obj.signalIndexLeft,
                    "signalIndexRight": signal_obj.signalIndexRight
                })
            return {
                "scopeType": o.scopeType,
                "moduleName": o.moduleName,
                "listSignal": signals_list
            }
        elif isinstance(o, signalValue):
            return {
                "idCode": o.idCode,
                "value": o.value
            }
        elif isinstance(o, timeChangeBlock):
            signal_values_list = []
            for signal_value_obj in o.listSignalValue:
                signal_values_list.append({
                    "idCode": signal_value_obj.idCode,
                    "value": signal_value_obj.value
                })
            return {
                "time": o.time,
                "listSignalValue": signal_values_list
            }
        else:
            # 确保这里对于其他未知类型（如果有）能正确调用父类默认处理方式，避免重复报错
            return super().default(o)

def serializVCD(vcdFileName, jsonFileName):
    listScopeInit = []
    scopeToken = scopeType("", "", [])
    timeChangeBlockElement = timeChangeBlock(0, [])
    listTimeChangeBlock = []
    timeScaleInit = timeScale(0, "")


    with io.open(vcdFileName, 'rb') as vcdFile:
        tokens = reader.tokenize(vcdFile)
        for token in tokens:
            # scope signal uoscope
            match token.kind :
                case reader.TokenKind.SCOPE:
                    if  scopeToken.scopeType != "":
                        listScopeInit.append(scopeToken)
                    scopeToken = scopeType(token.scope.type_.name, token.scope.ident, [])
                case reader.TokenKind.VAR:
                    signalToken = signal(token.var.type_.name, token.var.reference, token.var.id_code, token.var.size, 0, 0)
                    if token.var.bit_index is None:
                        signalToken.signalIndexLeft = 0
                        signalToken.signalIndexRight = 0
                    elif isinstance(token.var.bit_index, int):
                        signalToken.signalIndexLeft = token.var.bit_index
                        signalToken.signalIndexRight = token.var.bit_index
                    else:
                        signalToken.signalIndexLeft = token.var.bit_index[0]
                        signalToken.signalIndexRight = token.var.bit_index[1]

                    scopeToken.listSignal.append(signalToken)
                case reader.TokenKind.UPSCOPE:
                    if scopeToken.scopeType != "":
                        listScopeInit.append(scopeToken)
                        scopeToken = scopeType("", "", [])
                    listScopeInit.append(scopeType("UPSCOPE", "UPSCOPE", []))
                case reader.TokenKind.CHANGE_TIME:
                    timeChangeBlockElement = timeChangeBlock(str(token.time_change.real), [])
                    listTimeChangeBlock.append(timeChangeBlockElement)
                case reader.TokenKind.CHANGE_SCALAR:
                    timeChangeBlockElement.listSignalValue.append(signalValue(token.scalar_change.id_code, str(token.scalar_change.value)))
                case reader.TokenKind.CHANGE_VECTOR:
                    timeChangeBlockElement.listSignalValue.append(signalValue(token.vector_change.id_code, str(token.vector_change.value)))
                case reader.TokenKind.TIMESCALE:
                    timeScaleInit = timeScale(token.timescale.magnitude.value, token.timescale.unit.value)
                case reader.TokenKind.DATE | reader.TokenKind.VERSION | reader.TokenKind.ENDDEFINITIONS | reader.TokenKind.DUMPVARS | reader.TokenKind.END :
                    continue
                case _:
                    print(f"暂时不支持此数据类型： {token.kind.name}")

        data_to_dump = {
            "timeScale": timeScaleInit,
            "scopeList": listScopeInit,
            "timeChangeBlocks": listTimeChangeBlock
        }
        with open(jsonFileName, "w") as jsonFile:
            json.dump(data_to_dump, jsonFile, cls=CustomJSONEncoder)
    

def beginSerializVCD():
    if len(sys.argv) == 3:
        return serializVCD(sys.argv[1], sys.argv[2])
    else:
        print("序列化VCD输入的参数不正确")
        return 1
    

if __name__ == "__main__":
    beginSerializVCD()
