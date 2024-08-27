/**
  ******************************************************************************
  * @file           : CommandBuilder.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/8
  ******************************************************************************
  */
#include "CommandBuilder.h"
#include <sstream>
#include "utils/StringUtilities.h"
#include "ProcessManager.h"
//获取实例
CommandBuilder& CommandBuilder::instance()
{
    static CommandBuilder instance;
    return instance;
}
CommandBuilder::CommandBuilder()
{
}

CommandBuilder::~CommandBuilder()
{
}

std::string CommandBuilder::generateBitCommands(const QString& projectImplPath,const std::string& pythonPath,const QString& topName){
    std::map<std::string,std::string> parameters;

//    parameters["db-root"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("prjxray_db_path"), "artix7"});
//    parameters["part"] = ProcessManager::instance().getProperty("part_name");
//    parameters["part_file"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("prjxray_db_path"), "artix7", ProcessManager::instance().getProperty("part_name"), "part.yaml"});
//    parameters["sparse"] = "";
//    parameters["frm2bit"] = ProcessManager::instance().getProperty("fasm2bit_path");
//    parameters["emit_pudc_b_pullup"] = "";
//    parameters["fn_in"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".fasm"});
//    parameters["bit_out"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".bit"});
//
    std::stringstream cmd;
//    std::string xcfasmPath = ProcessManager::instance().getProperty("generate_bit_path");
//    // pythonPath + executePath + 后面的参数
//
//    cmd << pythonPath << " " << xcfasmPath;
//
//    for(const auto& pair : parameters) {
//        cmd << " --" << pair.first << " " << pair.second;
//    }
//
    return cmd.str();
}

std::string CommandBuilder::generateDownloadBitCommands(
        const QString& projectImplPath,
        const QString& partName,
        const QString& topName){
    std::stringstream cmd;
//    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");
//
//    if (partName.contains("a100t")){
//        //  100t 黑金
//        cmd << " -c digilent_hs3";
//    } else if (partName.contains("a35t")){
//        //  35t 野火
//        cmd << " -c ft2232";
//    };
//    cmd << " " << (projectImplPath + "/" + topName).toStdString();
    return cmd.str();
}

std::string CommandBuilder::generateDownloadFlashCommands(
        const QString& projectImplPath,
        const QString& partName,
        const QString& topName){
    std::stringstream cmd;
//    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");
//
//    if (partName.contains("a100t")){
//        //  100t 黑金
//        cmd << " -c digilent_hs3 -b alinx_ax7102";
//    } else if (partName.contains("a35t")){
//        //  35t 野火
//        cmd << " -c ft2232";
//
//    };
//    cmd << " -f " << (projectImplPath + "/" + topName).toStdString();
    return cmd.str();
}

std::string CommandBuilder::generateReadBackRegisterCommands(const QString& partName,const QString& registerAddress){
    std::stringstream cmd;
//    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");
//
//    if (partName.contains("a100t")){
//        //  100t 黑金
//        cmd << " -c digilent_hs3 --read-register-from-address";
//    } else if (partName.contains("a35t")){
//        //  35t 野火
//        cmd << " -c digilent_hs3 --read-register-from-address";
//    };
//    cmd << " " << registerAddress.toStdString();
    return cmd.str();
}

std::string CommandBuilder::generateReadMemoryCommands(const QString& partName,const QString& rbdFilePath){
    std::stringstream cmd;
//    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");
//
//    if (partName.contains("a100t")){
//        //  100t 黑金
//        cmd << " -c digilent_hs3 -b arty_a7_100t --read-memory";
//    } else if (partName.contains("a35t")){
//        //  35t 野火
//        cmd << " -c digilent_hs3 -b arty_a7_100t --read-memory";
//    };
//    cmd << " " << rbdFilePath.toStdString();
    return cmd.str();
}
