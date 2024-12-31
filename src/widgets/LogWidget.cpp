#include "LogWidget.h"
#include "service/PipeServer.h"
#include "service/LogManager.h"

LogWidget *LogWidget::instance(QWidget *parent)
{
    static LogWidget *_instance = nullptr;
    if (!_instance) {
        _instance = new LogWidget(parent);
    }
    return _instance;
}

void LogWidget::appendLog(const QString &str) {
    // 获取当前文本光标
//    QTextCursor cursor = logTextEdit->textCursor();
//    // 将光标移动到文本末尾(否则会在用户鼠标点击位置插入信息）
//    cursor.movePosition(QTextCursor::End);
//    logTextEdit->setTextCursor(cursor);
//    // 插入log
//    logTextEdit->insertPlainText(str);
    logTextEdit->appendPlainText(str);
}

LogWidget::LogWidget(QWidget* parent)
    : QWidget(parent)
{
    init();
    // 获取 PipeServer 的单例实例
    PipeServer &pipeServer = PipeServer::instance();
}

void LogWidget::init()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->addSeparator();
    QAction *searchAction = new QAction(QIcon(":/icons/resource/icons/9-icon_search.png"),"Search", this);
    // searchAction->setIcon(QIcon(":/resource/search.ico"));
    toolBar->addAction(searchAction);
    toolBar->addSeparator();
    QAction *cleanAction = new QAction(QIcon(":/icons/resource/icons/15-icon_discard.png"),"Clean", this);
    // cleanAction->setIcon(QIcon(":/resource/clean.ico"));
    toolBar->addAction(cleanAction);
    toolBar->addSeparator();
    QAction *copyAction = new QAction(QIcon(":/icons/resource/icons/14-icon_copy_2.png"),"Copy", this);
    toolBar->addAction(copyAction);
    toolBar->setIconSize(QSize(20, 20));
    toolBar->addSeparator();

    // Widget
    baseWidget = new QWidget;
    baseWidget->setParent(this);
    // baseWidget->setFixedHeight(65);
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

    logTextEdit = new SearchTextEdit(this);
    logTextEdit->setReadOnly(true);
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);
    vlayout->addWidget(baseWidget);
    vlayout->addWidget(logTextEdit);
    // vlayout->addStretch();
    vlayout->setMargin(0);
    // vlayout->setSpacing(0);
    searchAction->setCheckable(true);
    connect(searchAction, &QAction::triggered, [this, searchAction](bool) {
        if (baseWidget->isVisible()) {
            baseWidget->setVisible(false);
            searchAction->setChecked(false);
        } else {
            baseWidget->setVisible(true);
            searchAction->setChecked(true);
        }
    });

    connect(backWardBtn, &QPushButton::clicked, [=]() {
        QString s = lineEdit->text();
        logTextEdit->searchBackward(s);
    });

    connect(forWardBtn, &QPushButton::clicked, [=]() {
        QString s = lineEdit->text();
        logTextEdit->searchForward(s);
    });

    connect(clearBtn, &QPushButton::clicked, [=]() {
        lineEdit->clear();
        logTextEdit->clearHighlight();
    });

    connect(cleanAction, &QAction::triggered, logTextEdit, &QPlainTextEdit::clear);

    connect(copyAction, &QAction::triggered, [=]() {
        QApplication::clipboard()->setText(logTextEdit->toPlainText());
    });
}