
#include "ChipGridOperations.h"
#include "view.h"
#include "blocks/Tiles.h"
#include "mainwindow.h"
#include "base/Globals.h"
#include <regex>
#include <utility>
// 处理tile颜色、width、height
std::map<std::string, std::map<std::string, std::map<std::string, int>>> ChipGridOperations::buildTileInfoMap(const nlohmann::basic_json<>& colorJson){
    std::map<std::string, std::map<std::string, std::map<std::string, int>>> tile_info_map;
    try {
        // 遍历 JSON 中的每个键值对
        for (auto it = colorJson.begin(); it != colorJson.end(); ++it) {
            const std::string& key = it.key();  // tile_type_name
            const nlohmann::json& color_content = it.value()["color"];  // 颜色rgb
            const nlohmann::json& size_content = it.value()["size"];  // size width height

            // 创建内层 map
            std::map<std::string, std::map<std::string, int>> inner_map;

            // 遍历内层 JSON 中的每个键值对
            for (auto inner_it = color_content.begin(); inner_it != color_content.end(); ++inner_it) {
                const std::string& inner_key = inner_it.key();  // R/G/B
                const int& inner_value = inner_it.value();  // 内层值
                inner_map["color"][inner_key] = inner_value;
            }
            for (auto inner_it = size_content.begin(); inner_it != size_content.end(); ++inner_it) {
                const std::string& inner_key = inner_it.key();  // width height
                const int& inner_value = inner_it.value();  // 内层值
                inner_map["size"][inner_key] = inner_value;
            }

            // 存储到外层 map
            tile_info_map[key] = inner_map;
        }
    } catch (const std::exception& e) {
        qDebug() << "Failed to parse JSON: " << e.what() ;
    }
    return tile_info_map;
}

void ChipGridOperations::setSkipTile(int curX, int curY, int height) {
    size_t cols_len = gridTypeMatrix[0].size(); // 获取该列目前存在的元素个数
    int end_y = curY + height ; // 跨行tile结束位置的tile的下一个tile的y坐标
    // 设置跨行tile覆盖的tile类型为 "SKIP"
    for (int i = curY + 1 ; i < cols_len && i < end_y; i++) {
        gridTypeMatrix[curX][i].types = "SKIP";
        gridTypeMatrix[curX][i].x_coordinate = curX * GLOBAL_TILE_BLOCK_WIDTH;
        gridTypeMatrix[curX][i].y_coordinate = i * GLOBAL_TILE_BLOCK_HEIGHT;
        gridTypeMatrix[curX][i].height = GLOBAL_TILE_BLOCK_HEIGHT;
    }
}

// 为tile赋颜色
void ChipGridOperations::setColorsToTiles(NormalTile& tile,std::map<std::string, std::map<std::string, std::map<std::string, int>>> colorMap) {
    // 检查 tile.types 是否在 tile_info_map 中存在
    auto it = colorMap.find(tile.types);
    if (it != colorMap.end()) {
        const std::map<std::string, int>& type_colors = colorMap[tile.types]["color"];
        tile.R = type_colors.at("R");
        tile.G = type_colors.at("G");
        tile.B = type_colors.at("B");
    } else {
        // 未找到types,用默认颜色并提示
//        qDebug() << "Tile type: " << tile.types << " not found in the color map. Using default color." << std::endl;
    }
}

void ChipGridOperations::buildTileGridAndCellsMatrix(std::string tileFilePathLocal, std::string tileColorPathLocal) {
    clearVector();
    this->tileColorPath = std::move(tileColorPathLocal);
    this->tileFilePath = std::move(tileFilePathLocal);
    std::ifstream tile_file(this->tileFilePath);
    std::ifstream color_file(this->tileColorPath);

    if (!tile_file) {
        qDebug() << "Could not open tile_file\n";
        qDebug() << QString::fromStdString(this->tileFilePath);
        return;
    }
    if (!color_file) {
        qDebug() << "Could not open color_file\n";
        qDebug() << QString::fromStdString(this->tileColorPath);
        return;
    }

    // ================ 解析 color_file ================

    nlohmann::json color_json_data;
    color_file >> color_json_data;
    color_file.close();

    // ================ 获取tile info的外部信息，如color,width,height（倍数） ================

    std::map<std::string, std::map<std::string, std::map<std::string, int>>> tile_info_map = buildTileInfoMap(color_json_data);

    // ================ 解析 tile_file ================

    nlohmann::json tile_json_data;
    tile_file >> tile_json_data;
    tile_file.close(); // 关闭文件

    // ================ 获取尺寸 ================

    totalSize.width = 0;
    totalSize.height = 0;

    for (const auto& entry : tile_json_data.items()) {
        // 行列是相反的，x是列号，y是行号
        int grid_x = entry.value()["grid_x"];
        int grid_y = entry.value()["grid_y"];
        totalSize.height = std::max(totalSize.height, grid_y);
        totalSize.width = std::max(totalSize.width, grid_x);
    }


    totalSize.height += 1;
    totalSize.width += 1;

    //  ================ 初始化grid  ================

    gridTypeMatrix.resize(totalSize.width, std::vector<NormalTile>(totalSize.height));

    for (const auto& entry  : tile_json_data.items()) {
        NormalTile item(entry.key(),entry.value());
        setColorsToTiles(item,tile_info_map);
        gridTypeMatrix[item.grid_x][item.grid_y] = item; // 位置信息与tilegrid中x,y相同
    }
    //  ================ 跨行类型 ================
    TYPE_TO_SIZE_FACTORS = {
            {"CMT_FIFO_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -5}}},
            {"CMT_TOP_R_UPPER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 13}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_UPPER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_LOWER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 9}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_LOWER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 16}, {"Y_GAP", -7}}},
            {"BRAM_L", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"DSP_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"DSP_L", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"LIOB33", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOB33", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOI3", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3_TBYTESRC", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3_TBYTETERM", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}}
    };

    //  ================ 处理grid跨行  ================
    for (int i = 0; i < totalSize.width; ++i) {
        for (int j = 0; j < totalSize.height; ++j) {
            // 获取当前 tile 的 type
            std::string types = gridTypeMatrix[i][j].types;
            auto it = TYPE_TO_SIZE_FACTORS.find(types);
            std::map<std::string,int> cur_factors;
            if (it != TYPE_TO_SIZE_FACTORS.end()) {
                // 跨行的tile
                cur_factors = TYPE_TO_SIZE_FACTORS[types];
            } else {
                // 普通tile
                cur_factors = {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 1}, {"Y_GAP", 0}};
            }

            auto it2 = tile_info_map.find(types);
            if (it2 == tile_info_map.end()) {
                gridTypeMatrix[i][j].height = GLOBAL_TILE_BLOCK_HEIGHT;
                gridTypeMatrix[i][j].width = GLOBAL_TILE_BLOCK_WIDTH;
            } else {
                // 计算当前 tile 的 height 和 width
                gridTypeMatrix[i][j].height = tile_info_map[types]["size"]["height"] * GLOBAL_TILE_BLOCK_HEIGHT;
                gridTypeMatrix[i][j].width = tile_info_map[types]["size"]["width"] * GLOBAL_TILE_BLOCK_WIDTH;
            }

            // 计算其放置在画布上的x,y坐标
            if (j > 0) {  // 如果不是列首的tile
                gridTypeMatrix[i][j].x_coordinate = gridTypeMatrix[i][j - 1].x_coordinate;
                // 当前tile的坐标是该列上一个y坐标+上一个tile的高度
                gridTypeMatrix[i][j].y_coordinate = gridTypeMatrix[i][j - 1].y_coordinate + gridTypeMatrix[i][j - 1].height;
            } else {  // 如果是列首的tile
                gridTypeMatrix[i][j].x_coordinate = i * GLOBAL_TILE_BLOCK_WIDTH;
                gridTypeMatrix[i][j].y_coordinate = 0;
            }
            // 处理跨行tile涉及的tile，由于跨行tile在tilegrid中其grid_x，grid_y描述的不是他起始位置，所以需要做处理
            if (it != TYPE_TO_SIZE_FACTORS.end()) {
                int start_y = j + cur_factors["Y_GAP"]; // 计算其真正的起始y
                // grid_type_matrix[i][start_y]位置一定为null，用当前tile将其覆盖
                gridTypeMatrix[i][start_y] = gridTypeMatrix[i][j];
                // 跨行flag
                gridTypeMatrix[i][start_y].is_multi_rows = true;
                if (start_y>=1){
                    // 如果该列存在上一个tile
                    gridTypeMatrix[i][start_y].y_coordinate = gridTypeMatrix[i][start_y - 1].y_coordinate + gridTypeMatrix[i][start_y - 1].height;
                } else {
                    gridTypeMatrix[i][start_y].y_coordinate = 0;
                }


                setSkipTile(i,start_y,cur_factors["HEIGHT_FACTOR"]);
            }
        }
    }
    gridMatrix.resize(totalSize.width, std::vector<Blocks*>(totalSize.height, nullptr));
    for(int i = 0; i < totalSize.width; ++i)
    {
        for(int j = 0; j < totalSize.height; ++j)
        {
            // 遇到skip跳过
            if(gridTypeMatrix[i][j].types == "SKIP"){
                continue;
            }
            gridMatrix[i][j] = new Tiles(
                    QColor(gridTypeMatrix[i][j].R, gridTypeMatrix[i][j].G, gridTypeMatrix[i][j].B),
                    gridTypeMatrix[i][j].loc_x,
                    gridTypeMatrix[i][j].loc_y,
                    i,
                    j,
                    gridTypeMatrix[i][j].width,
                    gridTypeMatrix[i][j].height,
                    gridTypeMatrix[i][j].types
            );
            gridMatrix[i][j]->setPos(QPointF(gridTypeMatrix[i][j].x_coordinate, gridTypeMatrix[i][j].y_coordinate));
            if (!gridTypeMatrix[i][j].is_multi_rows){
                // 非跨行的tile增加site
                for (size_t index = 0; index < gridTypeMatrix[i][j].cur_sites.size(); ++index) {
                    NormalSite site = gridTypeMatrix[i][j].cur_sites[index];
                    SitesBlock* site_block = new SitesSliceL(
                            Qt::white,
                            GLOBAL_SITE_BLOCK_WIDTH,
                            GLOBAL_SITE_BLOCK_HEIGHT,
                            i,
                            j,
                            site.type,
                            site.name,
                            site.index
                    );
                    gridMatrix[i][j]->addSubBlock(site_block);
                    site_block->setPos(QPointF(10*(index+1) + 90*index, 10));
                }
            }
        }
    }

    // --------------------- Clock Region ----------------------------
    // nlohmann::json tgj = tile_json_data;
    // // Iterate over JSON object
    // for (auto& [tile, tiledata] : tgj.items()) {
    //     int x = tiledata["grid_x"];
    //     int y = tiledata["grid_y"];

    //     // Read tile clock region info
    //     std::string clock_region = tiledata.contains("clock_region") && !tiledata["clock_region"].get<std::string>().empty()
    //                                    ? tiledata["clock_region"].get<std::string>()
    //                                    : "NULL";

    //     // Update clock region bounding box
    //     if (clock_region != "NULL") {
    //         if (clock_region_bounding_boxes.find(clock_region) == clock_region_bounding_boxes.end()) {
    //             // Initialize new clock region bounding box
    //             clock_region_bounding_boxes[clock_region] = BoundingBox();
    //         }

    //         // Update bounding box with min/max values
    //         clock_region_bounding_boxes[clock_region].x0 = std::min(clock_region_bounding_boxes[clock_region].x0, static_cast<double>(x));
    //         clock_region_bounding_boxes[clock_region].y0 = std::min(clock_region_bounding_boxes[clock_region].y0, static_cast<double>(y));
    //         clock_region_bounding_boxes[clock_region].x1 = std::max(clock_region_bounding_boxes[clock_region].x1, static_cast<double>(x));
    //         clock_region_bounding_boxes[clock_region].y1 = std::max(clock_region_bounding_boxes[clock_region].y1, static_cast<double>(y));
    //     }
    // }

    // // Print the clock region bounding boxes
    // for (const auto& region : clock_region_bounding_boxes) {
    //     std::cout << "Clock region: " << region.first
    //               << " BoundingBox(x0: " << region.second.x0
    //               << ", y0: " << region.second.y0
    //               << ", x1: " << region.second.x1
    //               << ", y1: " << region.second.y1 << ")" << std::endl;
    // }
}


bool ChipGridOperations::showGridView(QGraphicsScene *scene) {
    if (!scene) {
        return false;  // 如果 scene 为 nullptr，则返回 false
    }
    try {
        scene->clear();
        for (int i = 0; i < totalSize.width; ++i) {
            for (int j = 0; j < totalSize.height; ++j) {
                // 遇到skip跳过
                if (gridMatrix[i][j] == nullptr) {
                    continue;
                }
                scene->addItem(gridMatrix[i][j]);
            }
        }
    }catch (...) {
        return false;  // 如果在执行过程中抛出任何异常，则返回 false
    }

    QPen pen;
    pen.setWidth(7);
    pen.setColor(Qt::darkGreen);
    scene->addRect(QRect(0, 1*100, 78*210 + 100, 51*100), pen);
    pen.setColor(Qt::darkYellow);
    scene->addRect(QRect(0, 53*100, 78*210 + 100, 103*100 - 52*100), pen);
    pen.setColor(Qt::darkBlue);
    scene->addRect(QRect(0, 105*100, 78*210 + 100, 155*100 - 104*100), pen);
    pen.setColor(Qt::darkRed);
    scene->addRect(QRect(0, 157*100, 78*210 + 100, 207*100 - 156*100), pen);

    pen.setColor(Qt::darkBlue);
    scene->addRect(QRect(78*210 + 110, 1*100, 130*210 + 210 - 78*210 - 105, 51*100), pen);
    pen.setColor(Qt::darkRed);
    scene->addRect(QRect(78*210 + 110, 53*100, 147*210 + 210 - 78*210 - 105, 103*100 - 52*100), pen);
    pen.setColor(Qt::darkGreen);
    scene->addRect(QRect(78*210 + 110, 105*100, 147*210 + 210 - 78*210 - 105, 155*100 - 104*100), pen);
    pen.setColor(Qt::darkYellow);
    scene->addRect(QRect(78*210 + 110, 157*100, 130*210 + 210 - 78*210 - 105, 207*100 - 156*100), pen);

    return true;  // 如果成功执行了所有操作，则返回 true
}

ChipGridOperations::ChipGridOperations(){
}

void ChipGridOperations::setAllTileWhite(QGraphicsScene *scene) {
    for(int i = 0; i < totalSize.width; ++i)
    {
        for(int j = 0; j < totalSize.height; ++j)
        {
            if (gridMatrix[i][j] == nullptr) {
                continue;
            }
            for (SitesBlock* item : gridMatrix[i][j]->child_items) {
                item->setColor(QColor(Qt::white));
            }
            gridMatrix[i][j]->setColor(QColor(Qt::white));
        }
    }
    scene->update();
}

void ChipGridOperations::buildPlaceUsageGrid(const std::string& usageJsonPath){
    std::ifstream file(QString::fromStdString(usageJsonPath).toLocal8Bit().constData());
    if (!file) {
        qDebug() << "Could not open file\n";
        return;
    }

    // 解析 JSON
    nlohmann::json j;
    file >> j;
    file.close();
    TotalSize usageSize;
    size_t num_elements = j.size();

    usageSize.width = 210;
    usageSize.height = 456;

    used_site.reserve(128); // 预先分配内存
    /*
     * 根据 JSON 结构，可以通过提前获取多层嵌套的 JSON 引用来减少嵌套深度。
     * 提前处理可能的错误条件（如 key 不存在）也有助于减少查找开销。
    **/
    const auto& modules = j["modules"];
    for (const auto& module : modules) {
        if (!module.contains("cells")) continue;  // 提前检查 key 是否存在
        const auto& cells = module["cells"];
        for (const auto& cell : cells) {
            if (!cell.contains("attributes") || !cell["attributes"].contains("NEXTPNR_BEL")) continue;
            const auto& bells = cell["attributes"]["NEXTPNR_BEL"];
            for (const auto& bell : bells) {
                const std::string& value = bell.get_ref<const std::string&>();
                size_t pos = value.find('/');
                if (pos != std::string::npos) {
                    used_site.insert(value.substr(0, pos));
                } else {
                    used_site.insert(value);
                }
            }
        }
    }

    // qDebug() << "set: " << used_site.size();
    for (int i = 0; i < gridTypeMatrix.size(); ++i) {
        for (int j = 0; j < gridTypeMatrix[i].size(); ++j) {
            if (gridTypeMatrix[i][j].types == "SKIP") {
                continue;
            }
            for (auto site : gridTypeMatrix[i][j].cur_sites) {
                if (used_site.find(site.name) != used_site.end()) {
                    for (auto item : gridMatrix[i][j]->child_items) {
                        // item->setColor("SteelBlue");
                        QColor color(gridTypeMatrix[i][j].R, gridTypeMatrix[i][j].G, gridTypeMatrix[i][j].B);
                        item->setColor(color);
                    }
                }
            }
        }
    }

    used_site.clear();
}

bool ChipGridOperations::showPlaceUsageGrid(QGraphicsScene *scene) {
    if (!scene) {
        return false;  // 如果 scene 为 nullptr，则返回 false
    }
    try {
        auto len = usageGrid.size();
        for (int i = 0; i < len; ++i) {
            int x = usageGrid[i].father_x_coordinate - 1;
            int y = usageGrid[i].father_y_coordinate - 1;
            if (gridTypeMatrix[x][y].types == "SKIP" || gridMatrix[x][y] == nullptr || (x == 0 && y == 0)) {
                    continue;
            }

            if (usageGrid[i].child_loc >= gridMatrix[x][y]->child_items.size()){
                continue;
            }
            gridMatrix[x][y]->child_items[usageGrid[i].child_loc]->setColor(QString::fromStdString(usageGrid[i].color));
            // gridMatrix[x][y]->child_items[usageGrid[i].child_loc]->setColor(QString("gray"));
        }
        qDebug() << 456;
        usageGrid.clear();
    }catch (...) {
        return false;  // 如果在执行过程中抛出任何异常，则返回 false
    }

    return true;  // 如果成功执行了所有操作，则返回 true
}

NormalTile ChipGridOperations::getTileInfo(int col, int row) {
    if (col < totalSize.width && row < totalSize.height && gridMatrix[col][row] != nullptr)
        return gridTypeMatrix[col][row];
    else
        return NormalTile();
}

void ChipGridOperations::clearVector() {
    // 清空 grid_type_matrix
    for (auto& row : gridTypeMatrix) {
        row.clear();
        std::vector<NormalTile>().swap(row); // 释放内存
    }
    gridTypeMatrix.clear();
    std::vector<std::vector<NormalTile>>().swap(gridTypeMatrix); // 释放内存

    // 清空 grid_matrix
    for (auto& row : gridMatrix) {
        row.clear();
        std::vector<Blocks*>().swap(row); // 释放内存
    }
    gridMatrix.clear();
}

void ChipGridOperations::updateTilesNameVisibleStatus(bool status) {
    for(int i = 0; i < totalSize.width; ++i){
        for(int j = 0; j < totalSize.height; ++j) {
            // 遇到skip跳过
            if (gridTypeMatrix[i][j].types == "SKIP") {
                continue;
            }
            gridMatrix[i][j]->updateTilesNameVisibleStatus(status);
        }
    }
}

void ChipGridOperations::updateSitesVisibleStatus(bool sitesVisibleStatus) {
    for(int i = 0; i < totalSize.width; ++i){
        for(int j = 0; j < totalSize.height; ++j) {
            // 遇到skip跳过
            if (gridTypeMatrix[i][j].types == "SKIP") {
                continue;
            }
            // 获取当前位置的子项列表
            QVector<SitesBlock*> itemList = gridMatrix[i][j]->child_items;

            // 遍历子项列表并调用方法
            for (SitesBlock* item : itemList) {
                // 调用子项的方法
                item->updateSitesVisibleStatus(sitesVisibleStatus);
            }
        }
    }
}
