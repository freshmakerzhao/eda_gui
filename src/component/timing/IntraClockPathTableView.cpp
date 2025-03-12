#include "IntraClockPathTableView.h"

IntraClockPathTableView::IntraClockPathTableView(const json &list, const int &pathN, const IntraClockPaths &intraClockPaths, QWidget *parent)
    : QWidget(parent),
    model(new QStandardItemModel(this)),
    _pathN(pathN),
    worstNegativeSlack(0),
    totalNegativeSlack(0),
    worstHoldSlack(0),
    totalHoldSlack(0)
{
    model->setHorizontalHeaderLabels({"Name", "Slack", "From", "To", "Total Delay", "Logic Delay", "Net Delay"});
    tableView = new QTableView;
    tableView->verticalHeader()->hide();
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(tableView);

    try {
        for (const auto &entry : list) {
            // Path
            QList<QStandardItem*> row;
            row.append(new QStandardItem(QString("Path%1").arg(_pathN)));

            // Slack
            const auto& slack = entry["slack"].get<int>();
            row.append(new QStandardItem(QString::number(slack)));
            switch (intraClockPaths) {
            case IntraClockPaths::Setup:
                worstNegativeSlack = std::max(worstNegativeSlack, slack);
                totalNegativeSlack += slack;
                break;
            case IntraClockPaths::Hold:
                worstHoldSlack = std::max(worstHoldSlack, slack);
                totalHoldSlack += slack;
                break;
            default:
                break;
            }

            // From
            const auto& pathList = entry["path"];
            const auto& fromCell = QString::fromStdString(pathList[0]["from"]["cell"].get<std::string>());
            const auto& fromPort = QString::fromStdString(pathList[0]["from"]["port"].get<std::string>());
            row.append(new QStandardItem(QString("%1/%2").arg(fromCell, fromPort)));

            // To
            const auto& toCell = QString::fromStdString(pathList[pathList.size() - 1]["to"]["cell"].get<std::string>());
            const auto& toPort = QString::fromStdString(pathList[pathList.size() - 1]["to"]["port"].get<std::string>());
            row.append(new QStandardItem(QString("%1/%2").arg(toCell, toPort)));

            // Delay
            double totalDelay = 0.0f;
            double logicDelay = 0.0f;
            double netDelay = 0.0f;

            for (const auto &path : pathList) {
                if (path["type"].get<std::string>() == "routing") {
                    netDelay += path["delay"].get<double>();
                } else {
                    logicDelay += path["delay"].get<double>();
                }
            }

            totalDelay = logicDelay + netDelay;
            row.append(new QStandardItem(QString::number(totalDelay)));
            row.append(new QStandardItem(QString::number(logicDelay)));
            row.append(new QStandardItem(QString::number(netDelay)));

            model->appendRow(row);

            _pathN++;
        }
    } catch(const std::exception& e) {
        qDebug() << e.what();
    }
}

int IntraClockPathTableView::getSlack(const Slack &slack) const {
    int value = 0;
    switch (slack) {
    case Slack::WorstNegativeSlack:
        value = worstNegativeSlack;
        break;
    case Slack::TotalNegativeSlack:
        value = totalNegativeSlack;
        break;
    case Slack::WorstHoldSlack:
        value = worstHoldSlack;
        break;
    case Slack::TotalHoldSlack:
        value = totalHoldSlack;
        break;
    default:
        break;
    }

    return value;
}
