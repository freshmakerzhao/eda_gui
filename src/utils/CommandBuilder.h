/**
  ******************************************************************************
  * @file           : CommandBuilder.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/8
  ******************************************************************************
  */
#ifndef EDA_GUI_COMMANDBUILDER_H
#define EDA_GUI_COMMANDBUILDER_H
#include "qdebug.h"
#include "base/InitialConfig.h"
#include <sstream>
#include "utils/StringUtilities.h"
#include "ProcessManager.h"


class CommandBuilder {
public:

    static CommandBuilder& instance();
    /**
     * 下载码流
     * @param projectImplPath 项目路径\runs\impl
     * @param bitName
     * @param partName
     * @return
     */
    std::string generateDownloadBitCommands(const QString& bitstream, const QString& cable_name = "digilent_hs3");
    std::string generateDownloadFlashCommands(const QString& projectImplPath, const QString& partName, const QString& topName = "top");

    std::string generateReadBackRegisterCommands(const QString &partName, const QString &registerAddress);
    std::string generateReadMemoryCommands(const QString &partName, const QString &rbdFilePath);
private:
    CommandBuilder();
    ~CommandBuilder();
};


#endif //EDA_GUI_COMMANDBUILDER_H
