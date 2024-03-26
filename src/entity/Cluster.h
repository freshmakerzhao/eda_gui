/**
  ******************************************************************************
  * @File           : Cluster.h
  * @Author         : zs
  * @Description    : None
  * @Date           : 23-10-27
  ******************************************************************************
  */

#ifndef CLUSTER_H
#define CLUSTER_H
#include "utils/json.hpp"
#include <string>
#include <utility>
#include <vector>
#include <QDebug>

// 不同tile类型对应的 宽度倍数 高度倍数 y坐标相对grid_y的差值

struct TotalSize {
    TotalSize() {
    }

    int width;
    int height;

};

struct SubItem {
    SubItem() = default;

    std::string sub_key;
    int child_loc = 0;
    std::string label;
    std::string style;
    std::string color;
    std::string tooltip;
    int father_x_coordinate = 0;
    int father_y_coordinate = 0;
    int x_coordinate = 0;
    int y_coordinate = 0;

    explicit SubItem(const nlohmann::basic_json<>& j) {
        sub_key = j.begin().key();
        std::size_t lastUnderscorePos = sub_key.rfind('_');  // 查找最后一个 "_" 的位置
        if (lastUnderscorePos != std::string::npos && lastUnderscorePos + 1 < sub_key.length()) {
            std::string num_str = sub_key.substr(lastUnderscorePos + 1);  // 提取最后一个 "_" 后面的子字符串
            child_loc = std::stoi(num_str);  // 将子字符串转换为整数
        }
        label = j.begin().value()["label"];
        style = j.begin().value()["style"];
        color = j.begin().value()["color"];
        tooltip = j.begin().value()["tooltip"];
    }
};


struct Cluster {
    Cluster() = default;

    int x_coordinate{};
    int y_coordinate{};
    std::string style;
    std::string color;
    std::string label;
    std::vector<SubItem> sub;

    // 从 JSON 对象构造 Cluster 实例
    Cluster(const nlohmann::basic_json<>& j) {
        x_coordinate = j["x_coordinate"];
        y_coordinate = j["y_coordinate"];
        style = j["style"];
        color = j["color"];
        label = j["label"];
        for (const auto& item : j["sub"]) {
            SubItem one(item);
            one.father_x_coordinate = x_coordinate;
            one.father_y_coordinate = y_coordinate;
            sub.push_back(one);
        }
    }
};

struct NormalSite {
    NormalSite(std::string  name, std::string  type, int index)
            : name(std::move(name)), type(std::move(type)), index(index) {
    }
    NormalSite() = default;
    std::string name;
    std::string type;
    int index{};
};

struct NormalTile {
    NormalTile() = default;

    std::string clock_region;
    int grid_x = 1; // tilegrid中的坐标
    int grid_y = 1;
    int loc_x = 1; // tilegrid中名字上的坐标
    int loc_y = 1;
    int x_coordinate = 0; // 绘图x坐标
    int y_coordinate = 0; // 绘图y坐标
    int height = 0; // 高度
    int width = 0; // 宽度
    int R = 246;
    int G = 245;
    int B = 244;
    std::string tile_name;
    std::string label;
    std::string types;
    bool is_multi_rows = false; // 是否跨越多行
    std::vector<NormalSite> cur_sites;
    // 从 JSON 对象构造 NormalTile 实例
    explicit NormalTile(const std::string& key, const  nlohmann::json& value) {
        grid_x = value["grid_x"];
        grid_y = value["grid_y"];
        types = value["type"];
        tile_name = key;
        std::size_t pos = key.rfind('_');  // 查找最后一个 "_" 的位置
        if (pos != std::string::npos && pos + 1 < key.length()) {
            std::string num_str = key.substr(pos + 1);  // 提取最后一个 "_" 后面的子字符串
            size_t x_pos = num_str.find('X');
            size_t y_yos = num_str.find('Y');

            // 提取 x 的值
            std::string x_str = num_str.substr(x_pos + 1, y_yos - x_pos - 1);
            loc_x = std::stoi(x_str);

            // 提取 y 的值
            std::string y_str = num_str.substr(y_yos + 1);
            loc_y = std::stoi(y_str);
        }
        nlohmann::json sites = value["sites"];
        int index = 0;
        for (auto it = sites.begin(); it != sites.end(); ++it) {
            std::string name = it.key();
            std::string type = it.value();
            NormalSite site(name, type, index);
            index++;
            cur_sites.push_back(site);
        }
    }
};

#endif //CLUSTER_H
