
#include "ChipGridOperations.h"
#include "view.h"
#include "blocks/Tiles.h"
#include "base/Globals.h"
#include <iostream>
#include <regex>
#include <utility>
#include <algorithm>
#include <QDebug>
#include "utils/ProjectManager.h"
#include "blocks/SitesBlockFactory.h"
// 处理tile颜色、width、height

const std::unordered_set<std::basic_string<char>> SHOW_TILE = {
    "LIOB33",
    "LIOB33_SING",
    "RIOB33",
    "RIOB33_SING",
//    "HCLK_IOB",
    "LIOI3",
    "LIOI3_SING",
    "LIOI3_TBYTESRC",
    "LIOI3_TBYTETERM",
    "RIOI3",
    "RIOI3_SING",
    "RIOI3_TBYTESRC",
    "RIOI3_TBYTETERM",
    "HCLK_IOI3",
    "CMT_FIFO_R",
    "CMT_FIFO_L",
//    "HCLK_FIFO_L",
    "CMT_TOP_R_UPPER_T",
    "CMT_TOP_R_UPPER_B",
    "CMT_TOP_L_UPPER_T",
    "CMT_TOP_L_UPPER_B",
    "HCLK_CMT",
    "CMT_TOP_R_LOWER_T",
    "CMT_TOP_R_LOWER_B",
    "CMT_TOP_L_LOWER_T",
    "CMT_TOP_L_LOWER_B",
    "CLBLL_L",
    "CLBLL_R",
    "CLBLM_L",
    "CLBLM_R",
//    "HCLK_CLB",
    "BRAM_L",
    "BRAM_R",
//    "HCLK_BARM",
    "DSP_R",
    "DSP_L",
//    "HCLK_DSP_R",
    "MONTOR_TOP",
    "MONTOR_MID",
    "MONTOR_BOT",
    "CFG_CENTER_TOP",
    "CFG_CENTER_MID",
//    "CFG_CENTER_BOT",
//    "R_TERM_INT_GTX",
    "GTP_CHANNEL_3",
    "GTP_CHANNEL_3_MID_RIGHT",
    "GTP_CHANNEL_3_MID_LEFT",
    "GTP_CHANNEL_2",
    "GTP_CHANNEL_2_MID_RIGHT",
    "GTP_CHANNEL_3_MID_LEFT",
    "GTP_CHANNEL_1",
    "GTP_CHANNEL_1_MID_RIGHT",
    "GTP_CHANNEL_3_MID_LEFT",
    "GTP_CHANNEL_0",
    "GTP_CHANNEL_0_MID_RIGHT",
    "GTP_CHANNEL_3_MID_LEFT",
    "GTP_COMMON",
    "PCIE_BOT",
    "CLK_HROW_TOP_R",
    "CLK_HROW_BOT_R",
    "CLK_BUFG_TOP_R",
    "CLK_BUFG_BOT_R"
};

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

void ChipGridOperations::buildTileGridAndCellsMatrix(std::string tileFilePathLocal, std::string tileColorPathLocal, std::string pinsInfoPathLocal) {
    clearVector();
    this->tileColorPath = std::move(tileColorPathLocal);
    this->tileFilePath = std::move(tileFilePathLocal);
    this->pinsInfoPath = std::move(pinsInfoPathLocal);
    std::ifstream tile_file(this->tileFilePath);
    std::ifstream color_file(this->tileColorPath);
    std::ifstream pins_file(this->pinsInfoPath);

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
    if (!pins_file) {
        qDebug() << "Could not open pins_file\n";
        qDebug() << QString::fromStdString(this->pinsInfoPath);
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

    // ================ 获取引脚信息 ================

    nlohmann::json pins_json_data;
    pins_file >> pins_json_data;
    pins_file.close(); // 关闭文件

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
    const std::string device = ProjectManager::instance().getParameter(Project::DisplayPart).toStdString();

    for (const auto& entry  : tile_json_data.items()) {
        NormalTile item(entry.key(), entry.value(), pins_json_data[device]);
        setColorsToTiles(item,tile_info_map);
        gridTypeMatrix[item.grid_x][item.grid_y] = item; // 位置信息与tilegrid中x,y相同
    }
    //  ================ 跨行类型 ================
    TYPE_TO_SIZE_FACTORS = {
            {"CMT_FIFO_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -5}}},
            {"CMT_FIFO_L", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -5}}},
            {"CMT_TOP_R_UPPER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 13}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_UPPER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_LOWER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 9}, {"Y_GAP", -7}}},
            {"CMT_TOP_L_LOWER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 9}, {"Y_GAP", -7}}},
            {"CMT_TOP_R_LOWER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 16}, {"Y_GAP", -7}}},
            {"CMT_TOP_L_LOWER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 16}, {"Y_GAP", -7}}},
            {"BRAM_L", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"BRAM_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"DSP_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"DSP_L", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 5}, {"Y_GAP", -4}}},
            {"LIOB33", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOB33", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOI3", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOI3_TBYTESRC", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"RIOI3_TBYTETERM", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3_TBYTESRC", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"LIOI3_TBYTETERM", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 2}, {"Y_GAP", -1}}},
            {"GTP_CHANNEL_3", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 11}, {"Y_GAP", -5}}},
            {"GTP_CHANNEL_2", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 11}, {"Y_GAP", -5}}},
            {"GTP_CHANNEL_1", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 11}, {"Y_GAP", -5}}},
            {"GTP_CHANNEL_0", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 11}, {"Y_GAP", -5}}},
            {"GTP_COMMON", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 7}, {"Y_GAP", -6}}},
            {"PCIE_BOT", {{"WIDTH_FACTOR", 3}, {"HEIGHT_FACTOR", 20}, {"Y_GAP", -9}}},
            {"CFG_CENTER_TOP", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 10}, {"Y_GAP", -9}}},
            {"CFG_CENTER_MID", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 21}, {"Y_GAP", -9}}},
            {"CMT_TOP_L_UPPER_T", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 13}, {"Y_GAP", -7}}},
            {"CMT_TOP_L_UPPER_B", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 12}, {"Y_GAP", -7}}},
            {"CLK_HROW_TOP_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 8}, {"Y_GAP", -4}}},
            {"CLK_HROW_BOT_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 8}, {"Y_GAP", -4}}},
            {"CLK_BUFG_TOP_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 4}, {"Y_GAP", -3}}},
            {"CLK_BUFG_BOT_R", {{"WIDTH_FACTOR", 1}, {"HEIGHT_FACTOR", 4}, {"Y_GAP", -3}}},
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
                gridTypeMatrix[i][j].types="NULL";
                // 跨行flag
                gridTypeMatrix[i][start_y].is_multi_rows = true;
                if (start_y >= 1) {
                    // 如果该列存在上一个tile
                    gridTypeMatrix[i][start_y].y_coordinate =
                            gridTypeMatrix[i][start_y - 1].y_coordinate + gridTypeMatrix[i][start_y - 1].height;
                } else {
                    gridTypeMatrix[i][start_y].y_coordinate = 0;
                }

                setSkipTile(i,start_y,cur_factors["HEIGHT_FACTOR"]);
            }
        }
    }
    gridMatrix.resize(totalSize.width, std::vector<Tiles*>(totalSize.height, nullptr));
    bool lastCowShow = true;
    int widthCount = 0;
    for(int i = 0; i < totalSize.width; ++i)
    {
        if(!lastCowShow)
            widthCount += GLOBAL_TILE_BLOCK_WIDTH;
        for(int j = 0; j < totalSize.height; ++j)
        {
//            // 遇到skip跳过
//            if(gridTypeMatrix[i][j].types == "SKIP")
//                continue;

            //设置为空类型(不显示)的map
            std::string type = gridTypeMatrix[i][j].types;
            auto it = SHOW_TILE.find(type);
            if(it == SHOW_TILE.end()) {
                gridMatrix[i][j] = new Tiles(
                        QColor(gridTypeMatrix[i][j].R, gridTypeMatrix[i][j].G, gridTypeMatrix[i][j].B),
                        i,
                        j,
                        false,
                        gridTypeMatrix[i][j]
                );
                if(j == 1)
                    lastCowShow = false;
                continue;
            } else {
                gridMatrix[i][j] = new Tiles(
//                        QColor(gridTypeMatrix[i][j].R, gridTypeMatrix[i][j].G, gridTypeMatrix[i][j].B),
                        Qt::darkBlue,
                        i,
                        j,
                        true,
                        gridTypeMatrix[i][j]
                );
                gridMatrix[i][j]->setPos(QPointF(gridTypeMatrix[i][j].x_coordinate - widthCount, gridTypeMatrix[i][j].y_coordinate));
                lastCowShow = true;
            }
            //例化siteBlcok
            for (size_t index = 0; index < gridTypeMatrix[i][j].cur_sites.size(); ++index) {
                NormalSite site = gridTypeMatrix[i][j].cur_sites[index];
                SitesBlock* site_block = SitesBlockFactory::Instance().create(
                        site.type, Qt::gray, i, j, gridTypeMatrix[i][j], site.name, site.index, site.pin
                );
                gridMatrix[i][j]->addSubBlock(site_block);
                size_t pos = site.name.find("_X");
                siteBlockMap[site.name.substr(0,pos)][site.name] = site_block;
                // ------------------------------
                site_type_set.insert(site.type);
                // ------------------------------
            }
        }
    }

    // ----------------------------------------------
    for (auto item : site_type_set) {
        qDebug() << QString::fromStdString(item);
    }
    // ----------------------------------------------

    // --------------------- Clock Region ----------------------------
    // Iterate over JSON object
    for (auto& col : gridMatrix) {
        for (auto& tile : col) {
           if (!tile->isShow())
               continue;
           if (tile->info.clock_region != "NULL") {
               std::string clockRegion = tile->info.clock_region;
               if(clock_region_bounding_boxes.find(clockRegion) == clock_region_bounding_boxes.end()) {
                   clock_region_bounding_boxes[clockRegion] = BoundingBox();
               }
               const std::unordered_set<std::string> edgesType = {
                   "CLK_HROW_TOP_R",
                   "CLK_HROW_BOT_R",
                   "CLK_BUFG_TOP_R",
                   "CLK_BUFG_BOT_R",
               };
               qreal x = edgesType.find(tile->getType()) != edgesType.end()
                       ? tile->x() + tile->getWidth()
                       : tile->x();
               clock_region_bounding_boxes[clockRegion].x0 = std::min(clock_region_bounding_boxes[clockRegion].x0, x);
               clock_region_bounding_boxes[clockRegion].y0 = std::min(clock_region_bounding_boxes[clockRegion].y0, tile->y());
               clock_region_bounding_boxes[clockRegion].x1 = std::max(clock_region_bounding_boxes[clockRegion].x1, x);
               clock_region_bounding_boxes[clockRegion].y1 = std::max(clock_region_bounding_boxes[clockRegion].y1, tile->y());
           }
        }
    }
    // Print the clock region bounding boxes
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

    // --------------------- Clock Region -----------------------
    const std::vector<QColor> colors = {
        QColor(Qt::blue),
        QColor(Qt::green),
        QColor(Qt::red),
        QColor(Qt::yellow),
        QColor(Qt::magenta),
        QColor(Qt::red),
        QColor(Qt::magenta),
        QColor(Qt::blue),
        QColor(Qt::green),
        QColor(Qt::yellow),
    };
    int colorIndex = 0;

    QPen pen;
    pen.setWidth(2);
    pen.setCosmetic(true);

    for (const auto& region : clock_region_bounding_boxes) {
        int x0 = region.second.x0;
        int y0 = region.second.y0;
        int x1 = region.second.x1;
        int y1 = region.second.y1;

        QRect rect(x0, y0, x1 - x0 + GLOBAL_TILE_BLOCK_WIDTH, y1 - y0 + GLOBAL_TILE_BLOCK_HEIGHT);

        QColor currentColor = colors[colorIndex];
        pen.setColor(currentColor);
        colorIndex = (colorIndex + 1) % colors.size();

        QGraphicsRectItem *rectItem = scene->addRect(rect, pen);
        rectItem->setVisible(true);
        clock_region_rects.insert(rectItem);
    }

    // --------------------- Clock Region -----------------------
    return true;
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
                const std::string &value = bell.get_ref<const std::string&>();
                size_t pos = value.find('/');
                if (pos != std::string::npos) {
                    const std::string site = value.substr(0, pos);
                    used_site[site].insert(value);
//                } else {
//                    used_site.insert.;
                }
            }
        }
    }
}

bool ChipGridOperations::showPlaceUsageGrid(QGraphicsScene *scene) {
    if (!scene) {
        return false;  // 如果 scene 为 nullptr，则返回 false
    }
    try {
        for (int i = 0; i < gridTypeMatrix.size(); ++i) {
            for (int j = 0; j < gridTypeMatrix[i].size(); ++j) {
                if (gridTypeMatrix[i][j].types == "SKIP") {
                    continue;
                }
                for (auto site : gridTypeMatrix[i][j].cur_sites) {
                    if (used_site.find(site.name) != used_site.end()) {
                        for (auto item : gridMatrix[i][j]->child_items) {
                            if (item->getName() == site.name) {
                                //QColor color(gridTypeMatrix[i][j].R, gridTypeMatrix[i][j].G, gridTypeMatrix[i][j].B);
//                                item->setColor(QColor(Qt::green));
                                item->setUsed(used_site[site.name]);
                            }
                        }
                    }
                }
            }
        }
        used_site.clear();
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
        std::vector<Tiles*>().swap(row); // 释放内存
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

void ChipGridOperations::updateClockRegionVisibleStatus(const bool &clockRegionVisibleStatus)
{
    for (QGraphicsRectItem *it : clock_region_rects) {
        it->setVisible(clockRegionVisibleStatus);
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
                item->updateVisibleStatus(sitesVisibleStatus);
            }
        }
    }
}
