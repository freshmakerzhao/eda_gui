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

void InfoWidget::updateSynthItem(const QString synthPath)
{
    QFile file(synthPath + "/synth_stat.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject object = doc.object();

    runsModel->setItem(0, 0, new QStandardItem(QString("synth")));
    for (auto it = object.begin(); it != object.end(); ++it) {
        QStandardItem *valueItem = new QStandardItem(QString::number(it.value().toInt()));

        if (it.key() == "$lut") {
            runsModel->setItem(0, 2, valueItem);
        }
        // else if (it.key() == "") {

        // }
    }

    // QStandardItemModel *model = new QStandardItemModel(1, 2);
    // model->setHorizontalHeaderItem(0, new QStandardItem("Key"));
    // model->setHorizontalHeaderItem(1, new QStandardItem("Value"));
    // int row = 0;
    // for (auto it = object.begin(); it != object.end(); ++it) {
    //     QStandardItem *keyItem = new QStandardItem(it.key());
    //     QStandardItem *valueItem = new QStandardItem(QString::number(it.value().toInt()));
    //     model->setItem(row, 0, keyItem);
    //     model->setItem(row, 1, valueItem);
    //     ++row;
    // }
    // treeView->setModel(model);
}

void InfoWidget::updateImplItem(const QString implPath)
{
    QFile file(implPath + "/pb_type_count.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject object = doc.object();
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

    // ======================== Design Runs ========================
    runsView = new QTreeView(this);
    tabWidget->addTab(runsView, "Design Runs");
    runsModel = new QStandardItemModel(runsView);
    QStringList headers = {"Name",
                           "Status",
                           "LUT6",
                           "FF",
                           "BRAM",
                           "DSP",
                           "Start",
                           "Elapsed",
                           "Part"};
    runsModel->setHorizontalHeaderLabels(headers);
    runsView->setModel(runsModel);
    runsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // QStandardItem *synthItem = new QStandardItem;
    // model->appendRow(synthItem);
    // synthItem->setText("synth");
    // QStandardItem *implItem = new QStandardItem;
    // implItem->setText("impl");
    // synthItem->appendRow(implItem);

    runsView->expandAll();
}

InfoWidget::~InfoWidget()
{
    qDebug() << "[InfoWidget] Distructing...";
}

