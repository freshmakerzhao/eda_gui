

#ifndef ChipGridOperations_H
#define ChipGridOperations_H


#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include "base/Globals.h"
#include "grid/FPGAOpenGLWidget3.h"
#include "utils/json.hpp"
#include "entity/Cluster.h"
#include "blocks/Blocks.h"
#include <QCoreApplication>
#include <fstream>
#include <unordered_set>

#define NEXTPNR 0

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
    TotalSize totalSize;

    QList<QPointF> points;
    QList<QPointF> site_points;

    // 架构信息
    std::vector<std::vector<NormalTile>> gridTypeMatrix; // 存储tile_grid
    std::vector<std::vector<Blocks*>> gridMatrix; // 存储绘图对象 graphicItem
    std::vector<SubItem> usageGrid;

    // std::vector<std::string> used_site;
    std::unordered_set<std::string> used_site;

    std::vector<TILE> tiles_v;

    std::vector<SITE> sites_v;

    // std::unordered_set<std::string> site_type_set;
    // Initialize clock region bounding box map
    std::map<std::string, BoundingBox> clock_region_bounding_boxes;

    ChipGridOperations();
    void buildTileGridAndCellsMatrix(std::string tileFilePathLocal, std::string tileColorPathLocal);
    void setSkipTile(int curX, int curY, int height);
    void setColorsToTiles(NormalTile& tile,std::map<std::string, std::map<std::string, std::map<std::string, int>>> colorMap);

    bool showGridView(QGraphicsScene *scene);
    void setAllTileWhite(QGraphicsScene *scene);
    void buildPlaceUsageGrid(const std::string& usageJsonPath);
    bool showPlaceUsageGrid(QGraphicsScene *scene);
    void updateSitesVisibleStatus(bool sitesVisibleStatus);
    void updateTilesNameVisibleStatus(bool status);
    NormalTile getTileInfo(int col,int row);

    void setFPGAView3(FPGAOpenGLWidget3 *glwidget);
private:
    std::map<std::string, std::map<std::string, std::map<std::string, int>>> buildTileInfoMap(const nlohmann::basic_json<>& colorJson);
    // 根据传入type返回对应的tile
    void clearVector();

    FPGAOpenGLWidget3 *glwidget3;
};


#endif //ChipGridOperations_H
