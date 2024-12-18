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
    setupUI();
    addMessage(1, "主模块", "a");
    addMessage(2, "启动", "b");
    addMessage(3, "模块", "c");
}

MessageWidget::~MessageWidget()
{
    // 析构函数留空
}

void MessageWidget::setupUI()
{
    // 初始化控件
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels({"时间", "级别", "阶段", "消息"});
    treeWidget->setUniformRowHeights(true);

    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("搜索消息...");

    clearButton = new QPushButton(QIcon(":/icons/clear.png"), "", this);
    expandButton = new QPushButton("展开全部", this);
    collapseButton = new QPushButton("折叠全部", this);

    // 布局顶部搜索栏和按钮
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(searchBox);
    topLayout->addWidget(clearButton);
    topLayout->addWidget(expandButton);
    topLayout->addWidget(collapseButton);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(treeWidget);

    setLayout(mainLayout);

    // 连接信号和槽
    connect(searchBox, &QLineEdit::textChanged, this, &MessageWidget::onSearchTextChanged);
    connect(clearButton, &QPushButton::clicked, this, &MessageWidget::onClearSearch);
    connect(expandButton, &QPushButton::clicked, this, &MessageWidget::onExpandAll);
    connect(collapseButton, &QPushButton::clicked, this, &MessageWidget::onCollapseAll);
}

void MessageWidget::addMessage(int level, const QString &message, const QString &phase, QTreeWidgetItem *parent)
{
    QString levelStr;
    switch (level) {
        case 1: levelStr = "INFO"; break;
        case 2: levelStr = "WARNING"; break;
        case 3: levelStr = "ERROR"; break;
        default: levelStr = "UNKNOWN"; break;
    }

    QString currentTime = TimeUtilities::getCurTimeAndFormat(); // 展示
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);
    item->setText(0, currentTime);
    item->setText(1, levelStr);
    item->setText(2, phase);
    item->setText(3, message);

    if (!parent) {
        treeWidget->expandItem(item);
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
