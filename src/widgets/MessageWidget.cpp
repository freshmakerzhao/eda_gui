#include "MessageWidget.h"
#include "utils/TimeUtilities.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QToolBar>
#include <QFormLayout>
#include <QMessageBox>

MessageWidget *MessageWidget::instance(QWidget *parent)
{
    // 静态局部变量，确保只初始化一次
    static MessageWidget *_instance = nullptr;
    if (!_instance) {
        _instance = new MessageWidget(parent);
    }
    return _instance;
}

MessageWidget::MessageWidget(QWidget *parent)
        : QWidget(parent),logManager(LogManager::instance())
{
    init();
//    addMessage(1, "主模块", "a");
//    addMessage(2, "启动", "b");
//    addMessage(3, "模块", "c");
}

MessageWidget::~MessageWidget() {};


void MessageWidget::init()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->addSeparator();
    QAction *searchAction = new QAction(QIcon(":/icons/resource/icons/9-icon_search.png"),"Search", this);
    toolBar->addAction(searchAction);
    toolBar->addSeparator();
    QAction *cleanAction = new QAction(QIcon(":/icons/resource/icons/15-icon_discard.png"),"Clean", this);
    toolBar->addAction(cleanAction);
    toolBar->addSeparator();
    toolBar->setIconSize(QSize(20, 20));
    toolBar->addSeparator();

    // Widget
    baseWidget = new QWidget(this);
    baseWidget->setVisible(false);

    QPushButton *backWardBtn = new QPushButton("Backward", baseWidget);
    backWardBtn->setFixedWidth(90);
    QPushButton *forWardBtn = new QPushButton("Forward", baseWidget);
    forWardBtn->setFixedWidth(90);
    QPushButton *clearBtn = new QPushButton("Clear", baseWidget);
    clearBtn->setFixedWidth(90);

    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setFixedWidth(250);

    QFormLayout *fLayout = new QFormLayout(baseWidget);
    fLayout->setMargin(0);
    // fLayout->addRow("Search:", lineEdit);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(backWardBtn);
    hLayout->addWidget(forWardBtn);
    hLayout->addWidget(clearBtn);
    fLayout->addRow("Search:", hLayout);

    // 初始化 tree 控件
    treeWidget = new QTreeWidget(this);
    // 隐藏标题栏（无分栏）
    treeWidget->setHeaderHidden(true);

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);
    vlayout->addWidget(baseWidget);
    vlayout->addWidget(treeWidget);
    vlayout->setMargin(0);

    searchAction->setCheckable(true);

    // 清除所有message
    connect(cleanAction, &QAction::triggered, &logManager, &LogManager::clearLogs);
    // ====================== 搜索框相关 ======================
    connect(searchAction, &QAction::triggered, [this, searchAction]() {
        baseWidget->setVisible(!baseWidget->isVisible());
        searchAction->setChecked(baseWidget->isVisible());
    });

    // 搜索框实时过滤
    connect(lineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        filterTreeItems(treeWidget, text);
    });

    // Backward 按钮导航
    connect(backWardBtn, &QPushButton::clicked, this, [this]() {
        navigateSearchResult(-1);
    });

    // Forward 按钮导航
    connect(forWardBtn, &QPushButton::clicked, this, [this]() {
        navigateSearchResult(1);
    });

    // Clear 按钮清除搜索框中的内容
    connect(clearBtn, &QPushButton::clicked, lineEdit, &QLineEdit::clear);
    // ====================== 搜索框相关 ======================
//
//    searchBox = new QLineEdit(this);
//    searchBox->setPlaceholderText("搜索日志...");
//
//    clearButton = new QPushButton("清除搜索", this);
//    expandButton = new QPushButton("展开全部", this);
//    collapseButton = new QPushButton("折叠全部", this);
//
//    QHBoxLayout *topLayout = new QHBoxLayout;
//    topLayout->addWidget(searchBox);
//    topLayout->addWidget(clearButton);
//    topLayout->addWidget(expandButton);
//    topLayout->addWidget(collapseButton);

//    QVBoxLayout *mainLayout = new QVBoxLayout(this);
//    mainLayout->addLayout(topLayout);
//    mainLayout->addWidget(treeWidget);
//    setLayout(mainLayout);
//    // 信号槽绑定
//    connect(searchBox, &QLineEdit::textChanged, this, &MessageWidget::onSearchTextChanged);
//    connect(clearButton, &QPushButton::clicked, this, &MessageWidget::onClearSearch);
//    connect(expandButton, &QPushButton::clicked, this, &MessageWidget::onExpandAll);
//    connect(collapseButton, &QPushButton::clicked, this, &MessageWidget::onCollapseAll);
}

//void MessageWidget::init()
//{
//    // 初始化控件
//    treeWidget = new QTreeWidget(this);
//
//    // 隐藏标题栏（无分栏）
//    treeWidget->setHeaderHidden(true);
//
//    searchBox = new QLineEdit(this);
//    searchBox->setPlaceholderText("搜索日志...");
//
//    clearButton = new QPushButton("清除搜索", this);
//    expandButton = new QPushButton("展开全部", this);
//    collapseButton = new QPushButton("折叠全部", this);
//
//    QHBoxLayout *topLayout = new QHBoxLayout;
//    topLayout->addWidget(searchBox);
//    topLayout->addWidget(clearButton);
//    topLayout->addWidget(expandButton);
//    topLayout->addWidget(collapseButton);
//
//    QVBoxLayout *mainLayout = new QVBoxLayout(this);
//    mainLayout->addLayout(topLayout);
//    mainLayout->addWidget(treeWidget);
//    setLayout(mainLayout);
//    // 信号槽绑定
//    connect(searchBox, &QLineEdit::textChanged, this, &MessageWidget::onSearchTextChanged);
//    connect(clearButton, &QPushButton::clicked, this, &MessageWidget::onClearSearch);
//    connect(expandButton, &QPushButton::clicked, this, &MessageWidget::onExpandAll);
//    connect(collapseButton, &QPushButton::clicked, this, &MessageWidget::onCollapseAll);
//}

void MessageWidget::populateTreeFromLogStorage(const LogStorage &logStorage)
{
    treeWidget->clear();
    for (auto it = logStorage.constBegin(); it != logStorage.constEnd(); ++it) {
        const QString &phase = it.key();
        const QMap<QString, QStringList> &subPhaseMap = it.value();

        // 创建第一层节点（phase）
        QTreeWidgetItem *phaseItem = new QTreeWidgetItem(treeWidget);
        phaseItem->setText(0, phase);

        for (auto subIt = subPhaseMap.constBegin(); subIt != subPhaseMap.constEnd(); ++subIt) {
            const QString &subPhase = subIt.key();
            const QStringList &messages = subIt.value();

            if (subPhase.isEmpty()) {
                // 如果 sub_phase 为空，直接将日志附加到 phase 下
                for (const QString &message : messages) {
                    QTreeWidgetItem *messageItem = new QTreeWidgetItem(phaseItem);
                    messageItem->setText(0, message);
                }
            } else {
                // 否则，创建 sub_phase 节点并附加日志
                QTreeWidgetItem *subPhaseItem = new QTreeWidgetItem(phaseItem);
                subPhaseItem->setText(0, subPhase);

                for (const QString &message : messages) {
                    QTreeWidgetItem *messageItem = new QTreeWidgetItem(subPhaseItem);
                    messageItem->setText(0, message);
                }
            }
        }

        // 默认展开第一层节点
        phaseItem->setExpanded(true);
    }
}

void MessageWidget::onExpandAll()
{
    treeWidget->expandAll();
}

void MessageWidget::onCollapseAll()
{
    treeWidget->collapseAll();
}

void MessageWidget::filterTreeItems(QTreeWidget *treeWidget, const QString &text) {
    searchResults.clear();
    currentResultIndex = -1;

    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        filterTreeItemRecursive(item, text);
    }
}

void MessageWidget::filterTreeItemRecursive(QTreeWidgetItem *item, const QString &text) {
    bool match = item->text(0).contains(text, Qt::CaseInsensitive);
    for (int i = 0; i < item->childCount(); ++i) {
        filterTreeItemRecursive(item->child(i), text);
    }

    item->setHidden(!match && !hasVisibleChildren(item));
    if (match) {
        searchResults.append(item);
    }
}

bool MessageWidget::hasVisibleChildren(QTreeWidgetItem *item) {
    for (int i = 0; i < item->childCount(); ++i) {
        if (!item->child(i)->isHidden()) {
            return true;
        }
    }
    return false;
}

void MessageWidget::navigateSearchResult(int step) {
    if (searchResults.isEmpty()) {
        QMessageBox::warning(this, "Search", "No search results to navigate.");
        return;
    }

    currentResultIndex += step;
    if (currentResultIndex < 0) {
        currentResultIndex = searchResults.size() - 1;
    } else if (currentResultIndex >= searchResults.size()) {
        currentResultIndex = 0;
    }

    QTreeWidgetItem *item = searchResults[currentResultIndex];
    treeWidget->setCurrentItem(item);
    treeWidget->scrollToItem(item);
}