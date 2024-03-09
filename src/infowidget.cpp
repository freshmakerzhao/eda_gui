#include "infowidget.h"

InfoWidget *InfoWidget::instance(QWidget *parent)
{
    static InfoWidget *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new InfoWidget(parent);
    }
    return m_instance;
}

void InfoWidget::appendMsg(const QString &str)
{
    msg->appendPlainText(str);
}

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[InfoWidget] Constructing...";
    QTabWidget *tabWidget = new QTabWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(tabWidget);
    csl = new QPlainTextEdit(this);
    tabWidget->addTab(csl, "Tcl Console");
    msg = new QPlainTextEdit(this), msg->setReadOnly(true);
    tabWidget->addTab(msg, "Messages");
    log = new QPlainTextEdit(this), log->setReadOnly(true);
    tabWidget->addTab(log, "Log");
    rpt = new QPlainTextEdit(this), rpt->setReadOnly(true);
    tabWidget->addTab(rpt, "Reports");
}

InfoWidget::~InfoWidget()
{
    qDebug() << "[InfoWidget] Distructing...";
}

