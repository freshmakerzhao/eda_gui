#include "taskview.h"

TaskView *TaskView::instance(QWidget *parent)
{
    static TaskView *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new TaskView(parent);
    }
    return m_instance;
}

TaskView::TaskView(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[TaskView] Constructing...";
    navTree = new QTreeWidget(this);
    navTree->viewport()->installEventFilter(this); //事件过滤
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(navTree);
    layout->setMargin(0);
    navTree->setColumnCount(1);
    navTree->setHeaderHidden(true);
    // ================== 综合 ==================
    QTreeWidgetItem *synthItem = new QTreeWidgetItem(navTree, QStringList() << "Synthesis");
    synthItem->setIcon(0, QIcon(""));// 在QIcon("")放置图标地址:/QIcon.ico
    QTreeWidgetItem *synthRunItem = new QTreeWidgetItem(synthItem, QStringList() << "Run");
    synthRunItem->setIcon(0, QIcon(""));
    // run synth
    QTreeWidgetItem *synthReportItem = new QTreeWidgetItem(synthItem, QStringList() << "Report");
    synthReportItem->setIcon(0, QIcon(""));
    // ================== imp ==================
    QTreeWidgetItem *impItem = new QTreeWidgetItem(navTree, QStringList() << "Implementation");
    impItem->setIcon(0, QIcon(""));
    // pack_place_route
    QTreeWidgetItem *impAllItem = new QTreeWidgetItem(impItem, QStringList() << "Run Implementation");
    impAllItem->setIcon(0, QIcon(""));
    // 仅pack
    QTreeWidgetItem *impPackItem = new QTreeWidgetItem(impItem, QStringList() << "Only pack");
    impPackItem->setIcon(0, QIcon(""));
    // 仅place
    QTreeWidgetItem *impPlaceItem = new QTreeWidgetItem(impItem, QStringList() << "Only place");
    impPlaceItem->setIcon(0, QIcon(""));
    // 仅route
    QTreeWidgetItem *impRouteItem = new QTreeWidgetItem(impItem, QStringList() << "Only route");
    impRouteItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *impPackReportItem = new QTreeWidgetItem(impItem, QStringList() << "Report");
    // 查看 Pack 日志
    impPackReportItem->setIcon(0, QIcon(""));
    // ================== 码流及可视化 ==================
    QTreeWidgetItem *proItem = new QTreeWidgetItem(navTree, QStringList() << "Program and Debug");
    proItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proNetlistViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate NetlistView");
    proNetlistViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitItem = new QTreeWidgetItem(proItem, QStringList() << "Generate Bitstream");
    proBitItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proBitViewItem = new QTreeWidgetItem(proItem, QStringList() << "Generate GridView");
    proBitViewItem->setIcon(0, QIcon(""));
    QTreeWidgetItem *proDownloadBitItem = new QTreeWidgetItem(proItem, QStringList() << "Download Bit");
    proDownloadBitItem->setIcon(0, QIcon(""));

    QObject::connect(navTree, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column) {
        // 双击触发
        if (item == synthRunItem) {
            // runSynth();
        } else if (item == synthReportItem) {
            // synthReport();
        } else if (item == impAllItem) {
            // pack place route全流程
            // buildImp();
        } else if (item == impPackItem) {
            // buildPack();
        } else if (item == impPackReportItem) {
            // impReport();
        } else if (item == impPlaceItem) {
            // buildPlace(3);
        } else if (item == impRouteItem) {
            // buildRoute();
        } else if (item == proBitItem) {
            // buildBit(2);
        } else if (item == proBitViewItem) {
            // if (frameView) {
            //     delete frameView;  // 删除现存的对象
            //     frameView = nullptr;  // 确保指针不再指向已删除的对象
            // }
            // frameView = new FrameView();
            // frameView->resize(1000, 800);
            // frameView->show();
        } else if (item == proNetlistViewItem) {
            // if (!frameView) {
            //     frameView = new NetlistView();
            // }
            // frameView->resize(1000, 800);
            // frameView->show();
        } else if (item == proDownloadBitItem) {
            // downloadBit();
        }
    });

    navTree->expandAll();

}

TaskView::~TaskView()
{
    qDebug() << "[TaskView] Distructing...";
}

bool TaskView::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == navTree->viewport())
    {
        //点击树的空白,取消选中
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                QModelIndex index = navTree->indexAt(mouseEvent->pos());
                if (!index.isValid())
                {
                    navTree->setCurrentIndex(QModelIndex());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
