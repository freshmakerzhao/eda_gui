

#ifndef ChipGridOperations_H
#define ChipGridOperations_H


#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include "utils/json.hpp"
#include "entity/TileGridDataMap.h"
#include <QCoreApplication>
#include <fstream>
#include <unordered_set>
#include "blocks/Block.h"
#include "blocks/TilesBlock.h"
#include <set>
//#include "ArchiveTool.h"

struct BoundingBox {
    double x0 = std::numeric_limits<double>::infinity(); // Minimum x
    double y0 = std::numeric_limits<double>::infinity(); // Minimum y
    double x1 = -std::numeric_limits<double>::infinity(); // Maximum x
    double y1 = -std::numeric_limits<double>::infinity(); // Maximum y
};

static std::map<std::string, std::map<std::string,int>> TYPE_TO_SIZE_FACTORS;

class ChipGridOperations : public QMainWindow{
    Q_OBJECT
public:
    std::string tileFilePath;
    std::string tileColorPath;
    std::string pinsInfoPath;
    TotalSize totalSize;

    // 架构信息
    std::vector<std::vector<NormalTile>> gridTypeMatrix; // 存储tile_grid
    std::vector<std::vector<TilesBlock*>> gridMatrix; // 存储绘图对象 graphicItem
    std::unordered_map<std::string, std::unordered_map<std::string, SitesBlock*>> siteBlockMap;
    std::vector<SubItem> usageGrid;

    // std::vector<std::string> used_site;
    std::unordered_map<std::string, std::set<std::array<std::string, 2>>> used_site;

    std::unordered_set<QGraphicsRectItem *> clock_region_rects;

    std::unordered_set<std::string> site_type_set;
    // Initialize clock region bounding box map
    std::map<std::string, BoundingBox> clock_region_bounding_boxes;

    ChipGridOperations();
    void buildTileGridAndCellsMatrix(std::string tileFilePathLocal, std::string tileColorPathLocal, std::string pinsInfoPathLocal);
    void setSkipTile(int curX, int curY, int height);
    void setColorsToTiles(NormalTile& tile,std::map<std::string, std::map<std::string, std::map<std::string, int>>> colorMap);

    bool showGridView(QGraphicsScene *scene);
    void buildPlaceUsageGrid(const std::string& usageJsonPath);
    bool showPlaceUsageGrid(QGraphicsScene *scene);
    NormalTile getTileInfo(int col,int row);
private:
    std::map<std::string, std::map<std::string, std::map<std::string, int>>> buildTileInfoMap(const nlohmann::basic_json<>& colorJson);
    // 根据传入type返回对应的tile
    void clearVector();
};


#endif //ChipGridOperations_H
