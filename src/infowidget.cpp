#include "infowidget.h"

InfoWidget *InfoWidget::instance(QWidget *parent)
{
    static InfoWidget *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new InfoWidget(parent);
    }
    return m_instance;
}

void InfoWidget::appendMsg(const QString &str) {
    msg->appendPlainText(str);
}

void InfoWidget::appendLog(const QString &str) {
    log->appendPlainText(str);
}

void InfoWidget::setCurrentPage(int index) {
    tabWidget->setCurrentIndex(index);
}

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[InfoWidget] Constructing...";
    tabWidget = new QTabWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(tabWidget);

    // =========================== Csg =============================
    csl = new QPlainTextEdit(this);
    tabWidget->addTab(csl, "Tcl Console");
    // =========================== Msg =============================
    msg = new QPlainTextEdit(this), msg->setReadOnly(true);
    tabWidget->addTab(msg, "Messages");

    // =========================== Log =============================
    QWidget *logWidget = new QWidget(tabWidget);
    QHBoxLayout *hLayout = new QHBoxLayout(logWidget);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    log = new QPlainTextEdit(logWidget);
    log->setReadOnly(true);

    QToolBar *toolbar = new QToolBar("Tools", logWidget);
    toolbar->setFixedWidth(35); // 调整toolbar宽度
    toolbar->setOrientation(Qt::Vertical);
    hLayout->addWidget(toolbar);
    hLayout->addWidget(log);

    QAction *searchAction = new QAction("Search", toolbar);
    searchAction->setIcon(QIcon(":/resource/search.ico"));
    toolbar->addAction(searchAction);
    connect(searchAction, &QAction::triggered, [=](){
        QString searchText = QInputDialog::getText(this, "Search", "Enter text to search");
        if (!searchText.isEmpty()) {
            if(log->find(searchText, QTextDocument::FindBackward)) {
                QPalette palette = log->palette();
                palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
                log->setPalette(palette);
            } else {
                QMessageBox::information(this, tr("Warning"), tr("Not Found"), QMessageBox::Ok);
            }
        }
    });

    QAction *cleanAction = new QAction("Clean", toolbar);
    cleanAction->setIcon(QIcon(":/resource/clean.ico"));
    connect(cleanAction, &QAction::triggered, log, &QPlainTextEdit::clear);
    toolbar->addAction(cleanAction);
    tabWidget->addTab(logWidget, "Log");

    // ============================ Rpt ============================
    rpt = new QPlainTextEdit(this), rpt->setReadOnly(true);
    tabWidget->addTab(rpt, "Reports");


}

InfoWidget::~InfoWidget()
{
    qDebug() << "[InfoWidget] Distructing...";
}

