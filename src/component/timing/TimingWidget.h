#ifndef TIMINGWIDGET_H
#define TIMINGWIDGET_H

#include <QWidget>
#include <QFile>
#include <QTreeView>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>

#include "utils/ProjectManager.h"
#include "utils/json.hpp"
#include "IntraClockPathTableView.h"
#include "DesignTimingSummaryWidget.h"

class TimingWidget : public QWidget
{
    Q_OBJECT
public:
    TimingWidget(QWidget *parent = nullptr);

    void loadDataFromJson(const QString &timingResults = "") {
        QFile file(timingResults);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open file";
            return;
        }

        QTextStream in(&file);
        QString fileContent = in.readAll();
        try {
            nlohmann::basic_json<nlohmann::ordered_map> j = nlohmann::basic_json<nlohmann::ordered_map>::parse(fileContent.toStdString());

            // Loop through the setup and hold paths
            const auto& setupList = j["setup"];
            const auto& holdList = j["hold"];

            QStandardItem *root = _model->invisibleRootItem();
            QStandardItem *parentItem = new QStandardItem("Intra-Clock Paths");
            const auto &from = QString::fromStdString(setupList[0]["from"].get<std::string>());
            QStandardItem *intraClockPathItem = new QStandardItem(from);
            parentItem->appendRow(intraClockPathItem);
            root->appendRow(parentItem);

            QStandardItem *setupPage = new QStandardItem("Setup");
            QStandardItem *holdPage = new QStandardItem("Hold");
            setupPage->setData(2, Qt::UserRole);
            holdPage->setData(3, Qt::UserRole);
            intraClockPathItem->appendRow(setupPage);
            intraClockPathItem->appendRow(holdPage);

            IntraClockPathTableView *setupIntraClockPathTableView = new IntraClockPathTableView(setupList, 1, IntraClockPaths::Setup);
            IntraClockPathTableView *holdIntraClockPathTableView = new IntraClockPathTableView(holdList, setupList.size() + 1, IntraClockPaths::Hold);



            const float worstNegativeSlack = setupIntraClockPathTableView->getSlack(Slack::WorstNegativeSlack);
            const float totalNegativeSlack = setupIntraClockPathTableView->getSlack(Slack::TotalNegativeSlack);

            const float worstHoldSlack = holdIntraClockPathTableView->getSlack(Slack::WorstHoldSlack);
            const float totalHoldSlack = holdIntraClockPathTableView->getSlack(Slack::TotalHoldSlack);

            DesignTimingSummaryWidget *designTimingSummaryWidget = new DesignTimingSummaryWidget(worstNegativeSlack,
                                                                                                 totalNegativeSlack,
                                                                                                 worstHoldSlack,
                                                                                                 totalHoldSlack);

            stackedWidget->addWidget(designTimingSummaryWidget);
            stackedWidget->addWidget(setupIntraClockPathTableView);
            stackedWidget->addWidget(holdIntraClockPathTableView);

        } catch (const std::exception& e) {
            qDebug() << "JSON Parsing Error:" << e.what();
        }
    }
private:


    void setupTreeView()
    {
        QStandardItem *root = _model->invisibleRootItem();

        // 第一页
        QStandardItem *page1 = new QStandardItem("General Information");
        page1->setData(0, Qt::UserRole);  // 存储堆叠窗口索引
        root->appendRow(page1);

        // 第二页
        QStandardItem *page2 = new QStandardItem("Design Timing Summary");
        page2->setData(1, Qt::UserRole);
        root->appendRow(page2);
    }

    void setupStackedWidget()
    {

        QString topModuleName = ProjectManager::instance().getParameter(Project::TopModule);
        QString partName = ProjectManager::instance().getParameter(Project::DisplayPart);
        QString version = QApplication::applicationVersion().isEmpty() ? "unknown" : QApplication::applicationVersion();

        QString information = QString(R"(
        <div style="line-height: 1.5;">
            <b>Report:</b> Timing Summary Report<br>
            <b>Design:</b> %1<br>
            <b>Part:</b> %2<br>
            <b>Version:</b> %3<br>
        </div>
        )").arg(topModuleName, partName, version);

        QLabel* label = new QLabel(information, stackedWidget);

        label->setTextFormat(Qt::RichText);
        label->setWordWrap(true); // 如果窗口变小也能换行
        label->setAlignment(Qt::AlignTop | Qt::AlignLeft); // 左上对齐
        label->setTextInteractionFlags(Qt::TextSelectableByMouse); // 允许复制
        label->setMargin(10); // 增加内边距

        stackedWidget->addWidget(label);
    }

private slots:
    void onTreeSelectionChanged(const QModelIndex &current)
    {
        // 获取存储的堆叠索引
        QVariant indexData = current.data(Qt::UserRole);
        if (indexData.isValid()) {
            bool ok;
            int stackIndex = indexData.toInt(&ok);

            if (ok && stackIndex < stackedWidget->count()) {
                stackedWidget->setCurrentIndex(stackIndex);
            }
        }
    }

private:
    QTreeView *treeView;

    QStackedWidget *stackedWidget;

    QStandardItemModel* _model;

    QMap<QString, QWidget*> widgetMap;
};

#endif // TIMINGWIDGET_H
