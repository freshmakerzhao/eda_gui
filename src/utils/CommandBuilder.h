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


class CommandBuilder {
public:

    static CommandBuilder& instance();
    std::string generateImpPackCommands(const QString& projectSynthPath,const QString& projectImplPath,const QString& archName,const QString& topName = "top");
    std::string generateImpIOPlaceCommands(const QString& projectSynthPath,const QString& projectImplPath,const std::string& pythonPath,const QString& topName = "top");
    std::string generateImpConstrainsCommands(const QString& projectSynthPath,const QString& projectImplPath,const std::string& pythonPath,const QString& topName = "top");
    std::string generateImpPlaceCommands(const QString& projectSynthPath,const QString& projectImplPath,const QString& archName,const QString& topName = "top");
    std::string generateImpRouteCommands(const QString& projectSynthPath,const QString& archName,const QString& topName = "top");
    std::string generateImpementationCommands(const QString& projectSynthPath,const QString& archName,const QString& topName = "top");
    std::string generateFasmCommands(const QString& projectSynthPath,const QString& archName,const QString& topName = "top");
    std::string generateBitCommands(const QString& projectImplPath,const std::string& pythonPath,const QString& topName = "top");
    std::string generateDownloadBitCommands(const QString& projectImplPath, const std::string& digilentName,const std::string& bitName);

private:
    CommandBuilder();
    ~CommandBuilder();
    std::map<std::string,std::string> device2graphMap = {
            {"xc7a35t","xc7a50t_test"},
            {"xc7a50t","xc7a50t_test"},
            {"xc7a100t","xc7a100t_test"}
    };

    std::map<std::string,std::string> deviceMap = {
            {"xc7a35t","xc7a50t-test"},
            {"xc7a50t","xc7a50t-test"},
            {"xc7a100t","xc7a100t-test"}
    };
};


#endif //EDA_GUI_COMMANDBUILDER_H
