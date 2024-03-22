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

void InfoWidget::updateSynthItem(const QString synthPath, const QString status, const QString startTime, const QString Elapsed , const QString partName){
    // 解析资源使用报告
    QFile file(synthPath + "/synth_stat.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;// 文件打开失败
    }
    QString jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject object = doc.object();
    runsModel->setItem(0, 0, new QStandardItem(QString("synth")));
    // 初始化个数
    int lut6Num = 0 , lutNum = 0, muxf6Num = 0 , ffNum = 0 , bramNum = 0 , fifo18Num = 0 , ranb18Num = 0 , ranb36Num = 0 , dspNum = 0;
    for (auto it = object.begin(); it != object.end(); ++it) {
//        QStandardItem *valueItem = new QStandardItem(QString::number(it.value().toInt()));
        if (it.key() == "$lut") {
            lutNum += it.value().toInt();
        } else if (it.key() == "MUXF6") {
            muxf6Num += it.value().toInt();
        } else if (it.key() == "FDRE_ZINI" || it.key() == "FDSE_ZINI" || it.key() == "FDCE_ZINI" || it.key() == "FDPE_ZINI") {
            ffNum += it.value().toInt();
        } else if (it.key() == "FIFO18E1_VPR") {
            fifo18Num += it.value().toInt();
        } else if (it.key() == "RAMB18E1_VPR") {
            ranb18Num += it.value().toInt();
        } else if (it.key() == "RAMB36E1_PRIM") {
            ranb36Num += it.value().toInt();
        } else if (it.key() == "DSP48E1_VPR") {
            dspNum += it.value().toInt();
        }
    }
    lut6Num = lutNum - muxf6Num; // LUT6
    bramNum = fifo18Num + (ranb18Num+1)/2 + ranb36Num; // BRAM

    runsModel->setItem(0, 1, new QStandardItem(status)); // Status
    runsModel->setItem(0, 2, new QStandardItem(QString::number(lut6Num))); // LUT6
    runsModel->setItem(0, 3, new QStandardItem(QString::number(ffNum))); // ff
    runsModel->setItem(0, 4, new QStandardItem(QString::number(bramNum))); // BRAM
    runsModel->setItem(0, 5, new QStandardItem(QString::number(dspNum))); // dsp
    runsModel->setItem(0, 6, new QStandardItem(startTime)); // 开始时间
    runsModel->setItem(0, 7, new QStandardItem(Elapsed)); // 持续时间
    runsModel->setItem(0, 8, new QStandardItem(partName)); // 封装名称
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
    runsView->expandAll();
}

InfoWidget::~InfoWidget()
{
    qDebug() << "[InfoWidget] Distructing...";
}

