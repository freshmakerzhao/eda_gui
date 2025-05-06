#ifndef INTRACLOCKPATHTABLEVIEW_H
#define INTRACLOCKPATHTABLEVIEW_H

#include <utils/json.hpp>
#include <QtWidgets>

using json = nlohmann::basic_json<nlohmann::ordered_map>;

enum class IntraClockPaths
{
    Setup,
    Hold
};

enum class Slack
{
    WorstNegativeSlack,
    TotalNegativeSlack,
    WorstHoldSlack,
    TotalHoldSlack
};

class IntraClockPathTableView : public QWidget
{
    Q_OBJECT
public:
    IntraClockPathTableView(const json &list, const int &pathN, const IntraClockPaths &intraClockPaths, QWidget *parent = nullptr);

    float getSlack(const Slack &slack) const;

private:
    QTableView *tableView;
    QStandardItemModel *model;

private:
    int _pathN;

    float worstNegativeSlack;
    float totalNegativeSlack;

    float worstHoldSlack;
    float totalHoldSlack;

};

#endif // INTRACLOCKPATHTABLEVIEW_H
