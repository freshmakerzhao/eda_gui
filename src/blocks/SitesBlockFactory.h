//
// Created by DELL on 2025/2/21.
//

#ifndef HYBRDLINK_SITESBLOCKFACTORY_H
#define HYBRDLINK_SITESBLOCKFACTORY_H
#include <unordered_map>
#include <functional>
#include "SitesMods.h"
#include "entity/TileGridDataMap.h"


class SitesBlockFactory {
public:
    using CreateFunction = std::function<SitesBlock*(const QColor&, int, int, NormalTile&, std::string&, int, std::string&)>;

    static SitesBlockFactory& Instance();
    void registerType(const std::string& type, CreateFunction createFunction);
    SitesBlock* create(const std::string& type, const QColor &color, int i, int j, NormalTile& tile, std::string& name, int index, std::string& pin);

private:
    SitesBlockFactory();
    std::unordered_map<std::string, CreateFunction> creators;
};


#endif //HYBRDLINK_SITESBLOCKFACTORY_H
