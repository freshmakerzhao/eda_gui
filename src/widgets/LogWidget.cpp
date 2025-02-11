#include "LogWidget.h"
#include "service/PipeServer.h"
#include "base/Globals.h"
#include <QDebug>

LogWidget *LogWidget::instance(QWidget *parent)
{
    static LogWidget *_instance = nullptr;
    if (!_instance) {
        _instance = new LogWidget(parent);
    }
    return _instance;
}

void LogWidget::appendLog(const QString &phaseType, const QString &str) {
    // 获取当前文本光标
//    QTextCursor cursor = logTextEdit->textCursor();
//    // 将光标移动到文本末尾(否则会在用户鼠标点击位置插入信息）
//    cursor.movePosition(QTextCursor::End);
//    logTextEdit->setTextCursor(cursor);
//    // 插入log
//    logTextEdit->insertPlainText(str);
    std::string phaseTypeDebug = phaseType.toStdString();
    if(!phaseType.compare("synthesis", Qt::CaseInsensitive)) //不区分大小写比较
        synthesisLogWidget->logTextEdit->appendPlainText(str.chopped(1));
    else if(!phaseType.compare("implementation", Qt::CaseInsensitive))
        implementationLogWidget->logTextEdit->appendPlainText(str.chopped(1));
    else if(!phaseType.compare("simulation", Qt::CaseInsensitive))
        simulationLogWidget->logTextEdit->appendPlainText(str.chopped(1));

//    logTextEdit->appendPlainText(str);
}

void LogWidget::appendLog(const LogPipeContent &one_log) {
    this->appendLog(one_log.getPhase(), one_log.getMessageContent());
}

LogWidget::LogWidget(QWidget* parent)
    : QWidget(parent)
{
    phaseTabWidget = new QTabWidget(this);
    phaseTabWidget->setTabPosition(QTabWidget::South);//设置标签页在下方

    synthesisLogWidget = new SingleLogWidget("synthesis", this);
    implementationLogWidget = new SingleLogWidget("implementation", this);
    simulationLogWidget = new SingleLogWidget("simulation", this);

    phaseTabWidget->addTab(synthesisLogWidget, "Synthesis");
    phaseTabWidget->addTab(implementationLogWidget, "Implementation");
    phaseTabWidget->addTab(simulationLogWidget, "Simulation");

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(phaseTabWidget);
    // 获取 PipeServer 的单例实例
    PipeServer &pipeServer = PipeServer::instance();
}

SingleLogWidget::SingleLogWidget(const std::string &phase, QWidget* parent)
{
    QToolBar *toolBar = new QToolBar;
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
    toolBar->setIconSize(QSize(14 * GlobalConfig::SCALE_FACTOR, 14 * GlobalConfig::SCALE_FACTOR));
    toolBar->addSeparator();

    // Widget
    baseWidget = new QWidget;
    baseWidget->setParent(this);
    baseWidget->setVisible(false);

    QPushButton *backWardBtn = new QPushButton("Backward", baseWidget);
    QPushButton *forWardBtn = new QPushButton("Forward", baseWidget);
    QPushButton *clearBtn = new QPushButton("Clear", baseWidget);

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
    hLayout->addStretch();
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
