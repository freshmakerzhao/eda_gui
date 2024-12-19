#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

// 定义日志存储结构
using LogStorage = QMap<QString, QMap<QString, QStringList>>; // Phase -> (SubPhase -> List of Messages)

class MessageWidget : public QWidget
{
Q_OBJECT

public:
    static MessageWidget *instance(QWidget *parent = nullptr);

    // 从 LogStorage 构建树
    void populateTreeFromLogStorage(const LogStorage &logStorage);

private slots:
    void onSearchTextChanged(const QString &text); // 搜索框文本变化时过滤
    void onClearSearch();                          // 清空搜索
    void onExpandAll();                            // 展开所有项
    void onCollapseAll();                          // 折叠所有项

private:
    explicit MessageWidget(QWidget *parent = nullptr); // 私有构造函数
    ~MessageWidget(); // 私有析构函数

    QTreeWidget *treeWidget;   // 树形控件
    QLineEdit *searchBox;      // 搜索框
    QPushButton *clearButton;  // 清除搜索按钮
    QPushButton *expandButton; // 展开按钮
    QPushButton *collapseButton; // 折叠按钮

    void init(); // 初始化UI
    bool filterTreeItem(QTreeWidgetItem *item, const QString &text); // 过滤树形项

    // 禁用拷贝构造和赋值操作
    Q_DISABLE_COPY(MessageWidget)
};

#endif // MESSAGEWIDGET_H
