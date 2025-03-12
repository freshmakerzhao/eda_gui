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

    int getSlack(const Slack &slack) const;

private:
    QTableView *tableView;
    QStandardItemModel *model;

private:
    int _pathN;

    int worstNegativeSlack;
    int totalNegativeSlack;

    int worstHoldSlack;
    int totalHoldSlack;

};

#endif // INTRACLOCKPATHTABLEVIEW_H
