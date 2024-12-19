#include "MessageWidget.h"
#include "utils/TimeUtilities.h"
#include <QTreeWidgetItem>
#include <QHeaderView>

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
        : QWidget(parent)
{
    init();
//    addMessage(1, "主模块", "a");
//    addMessage(2, "启动", "b");
//    addMessage(3, "模块", "c");
}

MessageWidget::~MessageWidget() {};

void MessageWidget::init()
{
    // 初始化控件
    treeWidget = new QTreeWidget(this);

    // 隐藏标题栏（无分栏）
    treeWidget->setHeaderHidden(true);

    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("搜索日志...");

    clearButton = new QPushButton("清除搜索", this);
    expandButton = new QPushButton("展开全部", this);
    collapseButton = new QPushButton("折叠全部", this);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(searchBox);
    topLayout->addWidget(clearButton);
    topLayout->addWidget(expandButton);
    topLayout->addWidget(collapseButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(treeWidget);
    setLayout(mainLayout);
    // 信号槽绑定
    connect(searchBox, &QLineEdit::textChanged, this, &MessageWidget::onSearchTextChanged);
    connect(clearButton, &QPushButton::clicked, this, &MessageWidget::onClearSearch);
    connect(expandButton, &QPushButton::clicked, this, &MessageWidget::onExpandAll);
    connect(collapseButton, &QPushButton::clicked, this, &MessageWidget::onCollapseAll);
}


//
//void MessageWidget::init()
//{
//    // 初始化控件
//    treeWidget = new QTreeWidget(this);
//    treeWidget->setHeaderLabels({"时间", "级别", "阶段", "消息"});
//    treeWidget->setUniformRowHeights(true);
//
//    searchBox = new QLineEdit(this);
//    searchBox->setPlaceholderText("搜索消息...");
//
//    clearButton = new QPushButton(QIcon(":/icons/clear.png"), "", this);
//    expandButton = new QPushButton("展开全部", this);
//    collapseButton = new QPushButton("折叠全部", this);
//
//    // 布局顶部搜索栏和按钮
//    QHBoxLayout *topLayout = new QHBoxLayout;
//    topLayout->addWidget(searchBox);
//    topLayout->addWidget(clearButton);
//    topLayout->addWidget(expandButton);
//    topLayout->addWidget(collapseButton);
//
//    // 主布局
//    QVBoxLayout *mainLayout = new QVBoxLayout(this);
//    mainLayout->addLayout(topLayout);
//    mainLayout->addWidget(treeWidget);
//
//    setLayout(mainLayout);
//
//    // 连接信号和槽
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

void MessageWidget::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        bool visible = filterTreeItem(item, text);
        item->setHidden(!visible);
    }
}

bool MessageWidget::filterTreeItem(QTreeWidgetItem *item, const QString &text)
{
    bool match = item->text(3).contains(text, Qt::CaseInsensitive);
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        match = filterTreeItem(child, text) || match;
    }
    item->setHidden(!match);
    return match;
}

void MessageWidget::onClearSearch()
{
    searchBox->clear();
}

void MessageWidget::onExpandAll()
{
    treeWidget->expandAll();
}

void MessageWidget::onCollapseAll()
{
    treeWidget->collapseAll();
}
