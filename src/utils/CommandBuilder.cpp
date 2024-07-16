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


std::string CommandBuilder::generateImpPackCommands(const QString& projectSynthPath,const QString& projectImplPath,const QString& archName,const QString& topName){
    std::map<std::string,std::string> parameters;
    QString device;
    QString rr_device;
    device = archName + "-test";
    rr_device = archName + "_test";
    parameters["device"] = device.toStdString();
    parameters["max_router_iterations"] = "500";
    parameters["routing_failure_predictor"] = "off";
    parameters["router_high_fanout_threshold"] = "-1";
    parameters["constant_net_method"] = "route";
    parameters["route_chan_width"] = "-1";
    parameters["router_heap"] = "bucket";
    parameters["clock_modeling"] = "route";
    parameters["place_delta_delay_matrix_calculation_method"] = "dijkstra";
    parameters["place_delay_model"] = "delta";
    parameters["router_lookahead"] = "extended_map";
    parameters["check_route"] = "quick";
    parameters["strict_checks"] = "off";
    parameters["allow_dangling_combinational_nodes"] = "on";
    parameters["disable_errors"] = "check_unbuffered_edges:check_route";
    parameters["congested_routing_iteration_threshold"] = "0.8";
    parameters["incremental_reroute_delay_ripup"] = "off";
    parameters["base_cost_type"] = "delay_normalized_length_bounded";
    parameters["bb_factor"] = "10";
    parameters["acc_fac"] = "0.7";
    parameters["astar_fac"] = "1.8";
    parameters["initial_pres_fac"] = "2.828";
    parameters["pres_fac_mult"] = "1.2";
    parameters["check_rr_graph"] = "off";
    parameters["suppress_warnings"] = "noisy_warnings-xc7a50t_test_pack.log,sum_pin_class:check_unbuffered_edges:load_rr_indexed_data_T_values:check_rr_node:trans_per_R:check_route:set_rr_graph_tool_comment:calculate_average_switch";
    parameters["read_router_lookahead"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".lookahead.bin"});
    parameters["read_placement_delay_lookup"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".place_delay.bin"});
    parameters["pack"] = "";
    parameters["write_block_usage"] = StringUtilities::concatPath({projectImplPath.toStdString(), "block_usage.json"});

    std::stringstream cmd;

    // vprPath + arch.timing.xml + topName.eblif + 后面的参数
    std::string vprBinPath = ProcessManager::instance().getProperty("vpr_path");
    std::string archPath = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    std::string eblifPath = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    cmd << vprBinPath << " " << archPath << " " << eblifPath << " ";

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}


std::string CommandBuilder::generateImpIOPlaceCommands(const QString& projectSynthPath,const QString& projectImplPath,const std::string& pythonPath,const QString& topName){
    std::map<std::string,std::string> parameters;

    parameters["blif"] = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    parameters["map"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), ProcessManager::instance().getProperty("part_name"), "pinmap.csv"});
    parameters["net"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".net"});
    parameters["output"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".ioplace"});

    std::stringstream cmd;
    std::string executePath = ProcessManager::instance().getProperty("generate_ioplace_path");
    // pythonPath + executePath + 后面的参数

    cmd << pythonPath << " " << executePath ;

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }
    return cmd.str();
}

std::string CommandBuilder::generateImpConstrainsCommands(const QString& projectSynthPath,const QString& projectImplPath,const std::string& pythonPath,const QString& topName){
    std::map<std::string,std::string> parameters;

    parameters["blif"] = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    parameters["net"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".net"});
    parameters["arch"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    parameters["part"] = ProcessManager::instance().getProperty("part_name");
    parameters["vpr_grid_map"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "vpr_grid_map.csv"});
    parameters["db_root"] = ProcessManager::instance().getProperty("prjxray_db_path");
    parameters["input"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".ioplace"});
    parameters["output"] = StringUtilities::concatPath({projectImplPath.toStdString(), "constraints.place"});

    std::stringstream cmd;
    std::string executePath = ProcessManager::instance().getProperty("generate_constraints_path");
    // pythonPath + executePath + 后面的参数

    cmd << pythonPath << " " << executePath ;

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}


std::string CommandBuilder::generateImpPlaceCommands(const QString& projectSynthPath,const QString& projectImplPath,const QString& archName,const QString& topName){
    std::map<std::string,std::string> parameters;

    QString device;
    QString rr_device;
    device = archName + "-test";
    rr_device = archName + "_test";

    parameters["device"] = device.toStdString();
    parameters["max_router_iterations"] = "500";
    parameters["routing_failure_predictor"] = "off";
    parameters["router_high_fanout_threshold"] = "-1";
    parameters["constant_net_method"] = "route";
    parameters["route_chan_width"] = "500";
    parameters["router_heap"] = "bucket";
    parameters["clock_modeling"] = "route";
    parameters["place_delta_delay_matrix_calculation_method"] = "dijkstra";
    parameters["place_delay_model"] = "delta";
    parameters["router_lookahead"] = "extended_map";
    parameters["check_route"] = "quick";
    parameters["strict_checks"] = "off";
    parameters["allow_dangling_combinational_nodes"] = "on";
    parameters["disable_errors"] = "check_unbuffered_edges:check_route";
    parameters["congested_routing_iteration_threshold"] = "0.8";
    parameters["incremental_reroute_delay_ripup"] = "off";
    parameters["base_cost_type"] = "delay_normalized_length_bounded";
    parameters["bb_factor"] = "10";
    parameters["acc_fac"] = "0.7";
    parameters["astar_fac"] = "1.8";
    parameters["initial_pres_fac"] = "2.828";
    parameters["pres_fac_mult"] = "1.2";
    parameters["check_rr_graph"] = "off";
    parameters["suppress_warnings"] = "noisy_warnings-xc7a50t_test_pack.log,sum_pin_class:check_unbuffered_edges:load_rr_indexed_data_T_values:check_rr_node:trans_per_R:check_route:set_rr_graph_tool_comment:calculate_average_switch";
    parameters["read_rr_graph"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".rr_graph.real.bin"});
    parameters["read_router_lookahead"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".lookahead.bin"});
    parameters["read_placement_delay_lookup"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".place_delay.bin"});
    parameters["fix_clusters"] = StringUtilities::concatPath({projectImplPath.toStdString(), "constraints.place"});
    parameters["place"] = "";

    std::stringstream cmd;

    // vprPath + arch.timing.xml + topName.eblif + 后面的参数
    std::string vprBinPath = ProcessManager::instance().getProperty("vpr_path");
    std::string archPath = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    std::string eblifPath = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    cmd << vprBinPath << " " << archPath << " " << eblifPath << " ";

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}


std::string CommandBuilder::generateImpRouteCommands(const QString& projectSynthPath,const QString& archName,const QString& topName){
    std::map<std::string,std::string> parameters;

    QString device;
    QString rr_device;
    device = archName + "-test";
    rr_device = archName + "_test";

    parameters["device"] = device.toStdString();
    parameters["max_router_iterations"] = "500";
    parameters["routing_failure_predictor"] = "off";
    parameters["router_high_fanout_threshold"] = "-1";
    parameters["constant_net_method"] = "route";
    parameters["route_chan_width"] = "500";
    parameters["router_heap"] = "bucket";
    parameters["clock_modeling"] = "route";
    parameters["place_delta_delay_matrix_calculation_method"] = "dijkstra";
    parameters["place_delay_model"] = "delta";
    parameters["router_lookahead"] = "extended_map";
    parameters["check_route"] = "quick";
    parameters["strict_checks"] = "off";
    parameters["allow_dangling_combinational_nodes"] = "on";
    parameters["disable_errors"] = "check_unbuffered_edges:check_route";
    parameters["congested_routing_iteration_threshold"] = "0.8";
    parameters["incremental_reroute_delay_ripup"] = "off";
    parameters["base_cost_type"] = "delay_normalized_length_bounded";
    parameters["bb_factor"] = "10";
    parameters["acc_fac"] = "0.7";
    parameters["astar_fac"] = "1.8";
    parameters["initial_pres_fac"] = "2.828";
    parameters["pres_fac_mult"] = "1.2";
    parameters["check_rr_graph"] = "off";
    parameters["suppress_warnings"] = "noisy_warnings-xc7a50t_test_pack.log,sum_pin_class:check_unbuffered_edges:load_rr_indexed_data_T_values:check_rr_node:trans_per_R:check_route:set_rr_graph_tool_comment:calculate_average_switch";
    parameters["read_rr_graph"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".rr_graph.real.bin"});
    parameters["read_router_lookahead"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".lookahead.bin"});
    parameters["read_placement_delay_lookup"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".place_delay.bin"});
    parameters["route"] = "";

    std::stringstream cmd;

    // vprPath + arch.timing.xml + topName.eblif + 后面的参数
    std::string vprBinPath = ProcessManager::instance().getProperty("vpr_path");
    std::string archPath = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    std::string eblifPath = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    cmd << vprBinPath << " " << archPath << " " << eblifPath << " ";

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}


std::string CommandBuilder::generateFasmCommands(const QString& projectSynthPath,const QString& archName,const QString& topName){
    std::map<std::string,std::string> parameters;


    QString device;
    QString rr_device;
    device = archName + "-test";
    rr_device = archName + "_test";

    parameters["device"] = device.toStdString();
    parameters["max_router_iterations"] = "500";
    parameters["routing_failure_predictor"] = "off";
    parameters["router_high_fanout_threshold"] = "-1";
    parameters["constant_net_method"] = "route";
    parameters["route_chan_width"] = "500";
    parameters["router_heap"] = "bucket";
    parameters["clock_modeling"] = "route";
    parameters["place_delta_delay_matrix_calculation_method"] = "dijkstra";
    parameters["place_delay_model"] = "delta";
    parameters["router_lookahead"] = "extended_map";
    parameters["check_route"] = "quick";
    parameters["strict_checks"] = "off";
    parameters["allow_dangling_combinational_nodes"] = "on";
    parameters["disable_errors"] = "check_unbuffered_edges:check_route";
    parameters["congested_routing_iteration_threshold"] = "0.8";
    parameters["incremental_reroute_delay_ripup"] = "off";
    parameters["base_cost_type"] = "delay_normalized_length_bounded";
    parameters["bb_factor"] = "10";
    parameters["acc_fac"] = "0.7";
    parameters["astar_fac"] = "1.8";
    parameters["initial_pres_fac"] = "2.828";
    parameters["pres_fac_mult"] = "1.2";
    parameters["check_rr_graph"] = "off";
    parameters["suppress_warnings"] = "noisy_warnings-xc7a50t_test_pack.log,sum_pin_class:check_unbuffered_edges:load_rr_indexed_data_T_values:check_rr_node:trans_per_R:check_route:set_rr_graph_tool_comment:calculate_average_switch";
    parameters["read_rr_graph"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".rr_graph.real.bin"});

    std::stringstream cmd;

    // vprPath + arch.timing.xml + topName.eblif + 后面的参数
    std::string vprBinPath = ProcessManager::instance().getProperty("generate_fasm_path");
    std::string archPath = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    std::string eblifPath = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    cmd <<  vprBinPath << " " << archPath << " " << eblifPath << " ";

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}

std::string CommandBuilder::generateBitCommands(const QString& projectImplPath,const std::string& pythonPath,const QString& topName){
    std::map<std::string,std::string> parameters;

    parameters["db-root"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("prjxray_db_path"), "artix7"});
    parameters["part"] = ProcessManager::instance().getProperty("part_name");
    parameters["part_file"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("prjxray_db_path"), "artix7", ProcessManager::instance().getProperty("part_name"), "part.yaml"});
    parameters["sparse"] = "";
    parameters["frm2bit"] = ProcessManager::instance().getProperty("fasm2bit_path");
    parameters["emit_pudc_b_pullup"] = "";
    parameters["fn_in"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".fasm"});
    parameters["bit_out"] = StringUtilities::concatPath({projectImplPath.toStdString(), topName.toStdString() + ".bit"});

    std::stringstream cmd;
    std::string xcfasmPath = ProcessManager::instance().getProperty("generate_bit_path");
    // pythonPath + executePath + 后面的参数

    cmd << pythonPath << " " << xcfasmPath;

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}

std::string CommandBuilder::generateDownloadBitCommands(
        const QString& projectImplPath,
        const QString& partName,
        const QString& topName){
    std::stringstream cmd;
    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");

    if (partName.contains("a100t")){
        //  100t 黑金
        cmd << " -c digilent_hs3";
    } else if (partName.contains("a35t")){
        //  35t 野火
        cmd << " -c ft2232";
    };
    cmd << " " << (projectImplPath + "/" + topName + ".bit").toStdString();
    return cmd.str();
}

std::string CommandBuilder::generateDownloadFlashCommands(
        const QString& projectImplPath,
        const QString& partName,
        const QString& topName){
    std::stringstream cmd;
    cmd << ProcessManager::instance().getProperty("openFPGALoader_path");

    if (partName.contains("a100t")){
        //  100t 黑金
        cmd << " -c ft2232-b arty";
    } else if (partName.contains("a35t")){
        //  35t 野火
        cmd << " -c digilent_hs3 -b alinx_ax7102";

    };
    cmd << " -f " << (projectImplPath + "/" + topName + ".bit").toStdString();
    return cmd.str();
}

std::string CommandBuilder::generateImpementationCommands(const QString& projectSynthPath,const QString& archName,const QString& topName){
    std::map<std::string,std::string> parameters;

    QString device;
    QString rr_device;
    device = archName + "-test";
    rr_device = archName + "_test";

    parameters["device"] = device.toStdString();
    parameters["max_router_iterations"] = "500";
    parameters["routing_failure_predictor"] = "off";
    parameters["router_high_fanout_threshold"] = "-1";
    parameters["constant_net_method"] = "route";
    parameters["route_chan_width"] = "500";
    parameters["router_heap"] = "bucket";
    parameters["clock_modeling"] = "route";
    parameters["place_delta_delay_matrix_calculation_method"] = "dijkstra";
    parameters["place_delay_model"] = "delta";
    parameters["router_lookahead"] = "extended_map";
    parameters["check_route"] = "quick";
    parameters["strict_checks"] = "off";
    parameters["allow_dangling_combinational_nodes"] = "on";
    parameters["disable_errors"] = "check_unbuffered_edges:check_route";
    parameters["congested_routing_iteration_threshold"] = "0.8";
    parameters["incremental_reroute_delay_ripup"] = "off";
    parameters["base_cost_type"] = "delay_normalized_length_bounded";
    parameters["bb_factor"] = "10";
    parameters["acc_fac"] = "0.7";
    parameters["astar_fac"] = "1.8";
    parameters["initial_pres_fac"] = "2.828";
    parameters["pres_fac_mult"] = "1.2";
    parameters["check_rr_graph"] = "off";
    parameters["suppress_warnings"] = "noisy_warnings-xc7a50t_test_pack.log,sum_pin_class:check_unbuffered_edges:load_rr_indexed_data_T_values:check_rr_node:trans_per_R:check_route:set_rr_graph_tool_comment:calculate_average_switch";
    parameters["read_rr_graph"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".rr_graph.real.bin"});
    parameters["read_router_lookahead"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".lookahead.bin"});
    parameters["read_placement_delay_lookup"] = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "rr_graph_" + rr_device.toStdString() + ".place_delay.bin"});

    std::stringstream cmd;

    // vprPath + arch.timing.xml + topName.eblif + 后面的参数
    std::string vprBinPath = ProcessManager::instance().getProperty("vpr_path");
    std::string archPath = StringUtilities::concatPath({ProcessManager::instance().getProperty("arch_path"), "arch.timing.xml"});
    std::string eblifPath = StringUtilities::concatPath({projectSynthPath.toStdString(), topName.toStdString() + ".eblif"});
    cmd << vprBinPath << " " << archPath << " " << eblifPath << " ";

    for(const auto& pair : parameters) {
        cmd << " --" << pair.first << " " << pair.second;
    }

    return cmd.str();
}
