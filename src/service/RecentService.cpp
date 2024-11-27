#include "RecentService.h"
#include "utils/XmlUtilities.h"
#include "base/InitialConfig.h"

void RecentService::writeRecentProject(const QString &standardHprPath)
{
    std::vector<XmlRecent> recentLists = {
        {0, standardHprPath.toStdString()}
    };
    try {
        XmlUtilities::instance().insertHybrdLinkXmlRecent(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS",
            recentLists
            );
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

std::vector<XmlRecent> RecentService::readRecentProject()
{
    std::vector<XmlRecent> recentList;
    try {
        recentList = XmlUtilities::instance().getRecentListFromFatherElementName(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS"
            );
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
    return recentList;
}

void RecentService::clearRecentProject()
{
    // 清空 RECENT_PROJECTS 下的 recent
    XmlUtilities::instance().clearNodesFromFatherElementName(
        InitialConfig::instance().xmlPath.toStdString().c_str(),
        "RECENT_PROJECTS");
}

