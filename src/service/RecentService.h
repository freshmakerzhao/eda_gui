#ifndef RECENTSERVICE_H
#define RECENTSERVICE_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "entity/XmlRecent.h"

class RecentService
{
public:
    //! 写入一个项目到 RECENT_PROJECTS, 必须传入 StandardPath
    static void writeRecentProject(const QString &standardHprPath);

    //! 获取 RECENT_PROJECTS 列表
    static std::vector<XmlRecent> readRecentProject();

    //! 清空 RECENT_PROJECTS
    static void clearRecentProject();

private:
    RecentService();
};

#endif // RECENTSERVICE_H
