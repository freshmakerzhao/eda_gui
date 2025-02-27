#include "LogWidget.h"
#include "service/PipeServer.h"
#include "base/Globals.h"
#include <QDebug>

SingleLogWidget::SingleLogWidget(const std::string &phase, QWidget* parent)
{
    QToolBar *toolBar = new QToolBar;
    toolBar->addSeparator();
    QAction *searchAction = new QAction(QIcon(":/icons/resource/icons/9-icon_search.png"),"Search", this);
    toolBar->addAction(searchAction);
    toolBar->addSeparator();
    QAction *pauseAction = new QAction(QIcon(":/icons/resource/icons/12-3icon_pause.png"),"Pause", this);
    toolBar->addAction(pauseAction);
    toolBar->addSeparator();
    QAction *cleanAction = new QAction(QIcon(":/icons/resource/icons/15-icon_discard.png"),"Clean", this);
    toolBar->addAction(cleanAction);
    toolBar->addSeparator();
    QAction *copyAction = new QAction(QIcon(":/icons/resource/icons/14-icon_copy_2.png"),"Copy", this);
    toolBar->addAction(copyAction);
    toolBar->setIconSize(QSize(14 * GlobalConfig::SCALE_FACTOR, 14 * GlobalConfig::SCALE_FACTOR));
    toolBar->addSeparator();

    // Widget
    baseWidget = new QWidget;
    baseWidget->setVisible(false);

    QPushButton *backwardButton = new QPushButton("Backward");
    QPushButton *forwardButton = new QPushButton("Forward");
    QPushButton *clearButton = new QPushButton("Clear");

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setFixedWidth(250);

    QFormLayout *fLayout = new QFormLayout(baseWidget);
    fLayout->setMargin(0);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(backwardButton);
    hLayout->addWidget(forwardButton);
    hLayout->addWidget(clearButton);
    hLayout->addStretch();
    fLayout->addRow("Search:", hLayout);

    logTextEdit = new SearchTextEdit;
    logTextEdit->setReadOnly(true);
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);
    vlayout->addWidget(baseWidget);
    vlayout->addWidget(logTextEdit);
    vlayout->setMargin(0);
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

    pauseAction->setCheckable(true);
    connect(pauseAction, &QAction::triggered, [this, pauseAction](bool) {
        if (pauseStatus) {
            pauseAction->setChecked(false);
            pauseStatus = false;
        } else {
            pauseAction->setChecked(true);
            pauseStatus = true;
        }
    });

    connect(backwardButton, &QPushButton::clicked, [=]() {
        QString text = lineEdit->text();
        logTextEdit->searchBackward(text);
    });

    connect(forwardButton, &QPushButton::clicked, [=]() {
        QString text = lineEdit->text();
        logTextEdit->searchForward(text);
    });

    connect(clearButton, &QPushButton::clicked, [=]() {
        lineEdit->clear();
        logTextEdit->clearHighlight();
    });

    connect(cleanAction, &QAction::triggered, logTextEdit, &QPlainTextEdit::clear);

    connect(copyAction, &QAction::triggered, [=]() {
        QApplication::clipboard()->setText(logTextEdit->toPlainText());
    });
}

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
    if(!phaseType.compare("synthesis", Qt::CaseInsensitive) && !synthesisLogWidget->pauseStatus) //不区分大小写比较
        synthesisLogWidget->logTextEdit->appendPlainText(str.chopped(1));
    else if(!phaseType.compare("implementation", Qt::CaseInsensitive) && !implementationLogWidget->pauseStatus)
        implementationLogWidget->logTextEdit->appendPlainText(str.chopped(1));
    else if(!phaseType.compare("simulation", Qt::CaseInsensitive) && !simulationLogWidget->pauseStatus)
        simulationLogWidget->logTextEdit->appendPlainText(str.chopped(1));
    else if(!debugLogWidget->pauseStatus)
        debugLogWidget->logTextEdit->appendPlainText(str.chopped(1));

//    logTextEdit->appendPlainText(str);
}

void LogWidget::appendDebugLog(const QString &str) {
    if(!debugLogWidget->pauseStatus)
        debugLogWidget->logTextEdit->appendPlainText(str.chopped(1));
}

void LogWidget::appendLog(const LogPipeContent &one_log) {
    this->appendLog(one_log.getPhase(), one_log.getMessageContent());
}

void LogWidget::clearLog()
{
    synthesisLogWidget->logTextEdit->clear();
    implementationLogWidget->logTextEdit->clear();
    simulationLogWidget->logTextEdit->clear();
}

LogWidget::LogWidget(QWidget* parent)
    : QWidget(parent)
{
    phaseTabWidget = new QTabWidget;
    phaseTabWidget->setTabPosition(QTabWidget::South);//设置标签页在下方

    synthesisLogWidget = new SingleLogWidget("synthesis");
    implementationLogWidget = new SingleLogWidget("implementation");
    simulationLogWidget = new SingleLogWidget("simulation");
    debugLogWidget = new SingleLogWidget("debug");

    phaseTabWidget->addTab(synthesisLogWidget, "Synthesis");
    phaseTabWidget->addTab(implementationLogWidget, "Implementation");
    phaseTabWidget->addTab(simulationLogWidget, "Simulation");
    phaseTabWidget->addTab(debugLogWidget, "Program And Debug");

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);
    vlayout->addWidget(phaseTabWidget);
    // 获取 PipeServer 的单例实例
    PipeServer &pipeServer = PipeServer::instance();
}

void LogWidget::switchSynLog() {
    this->phaseTabWidget->setCurrentIndex(0);
}

void LogWidget::switchImpLog() {
    this->phaseTabWidget->setCurrentIndex(1);
}

void LogWidget::switchSimLog() {
    this->phaseTabWidget->setCurrentIndex(2);
}
