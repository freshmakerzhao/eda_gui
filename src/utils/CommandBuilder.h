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
    std::string generateImpementationCommands(const std::string &partname,const std::string& projectPath);
    std::string generateImpPackCommands(const std::string &partname,const std::string& projectPath);
    std::string generateImpIOPlaceCommands(const std::string& pythonPath,const std::string& projectPath);
    std::string generateImpConstrainsCommands(const std::string& pythonPath,const std::string& projectPath);
    std::string generateImpPlaceCommands(const std::string &partname,const std::string& projectPath);
    std::string generateImpRouteCommands(const std::string &partname,const std::string& projectPath);
    std::string generateFasmCommands(const std::string &partname,const std::string& projectPath);
    std::string generateBitCommands(const std::string& pythonPath,const std::string &projectPath);
    std::string generateDownloadBitCommands(const std::string& digilentName,const std::string& projectPath,const std::string& bitName);

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
