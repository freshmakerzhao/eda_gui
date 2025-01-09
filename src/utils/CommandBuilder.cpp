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

std::string CommandBuilder::generateDownloadBitCommands(const QString& bitstream, const QString& cable_name){
    std::stringstream cmd;
    cmd << "%BITSTREAMTOOL_PATH%";
    cmd << (" -c " + cable_name).toStdString();
    cmd << " " << (bitstream).toStdString();
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
