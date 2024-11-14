#include "IPManager.h"
#include "clk_wiz/ClockingWizard.h"
#include "blk_mem_gen/BlockMemoryGenerator.h"
#include "fifo_generator/FIFOGenerator.h"
#include "widgets/FileManager.h"
#include "base/TreeViewBase.h"

IPManager *IPManager::instance()
{
    static IPManager *_instance = nullptr;
    if (!_instance) {
        _instance = new IPManager(nullptr);
    }
    return _instance;
}

IPManager::IPManager(QWidget* parent)
    : QWidget(parent)
{
    resize(800, 500);
    init();
}

void IPManager::init()
{
    QVBoxLayout* vlayout = new QVBoxLayout;

    repositoryTreeView = new RepositoryTreeView;
    repositoryTreeView->initRepository();

    vlayout->addWidget(new TreeViewBase(repositoryTreeView, false));
    vlayout->setMargin(0);

    detailLabel = new QLabel;
    detailLabel->setWordWrap(true);
    detailLabel->setMargin(15);
    detailLabel->setStyleSheet("background-color: rgb(255, 255, 255);");
    setDetails();

    QWidget *detailWidget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout(detailWidget);
    vboxLayout->setMargin(0);
    QLabel *titleLabel = new QLabel("Details");
    titleLabel->setContentsMargins(9, 0, 0, 0);
    titleLabel->setStyleSheet("font-weight: bold;");
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(detailLabel);
    vboxLayout->addWidget(titleLabel);
    vboxLayout->addWidget(scrollArea);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    QWidget *baseWidget = new QWidget(this);
    baseWidget->setLayout(vlayout);
    splitter->addWidget(baseWidget);
    splitter->addWidget(detailWidget);
    splitter->setSizes(QList<int>() << 400 << 200);

    QHBoxLayout *vlayout2 = new QHBoxLayout(this);
    vlayout2->setMargin(0);
    vlayout2->addWidget(splitter);

    connect(repositoryTreeView, &QTreeView::doubleClicked, this, &IPManager::doubleClickedIP);

    connect(repositoryTreeView, &QTreeView::clicked, this, &IPManager::clickedIP);

    repositoryTreeView->expandAll();
}

void IPManager::doubleClickedIP(const QModelIndex &index)
{
    //取选中的这行的第一个元素的index
    const QModelIndex &idx = index.sibling(index.row(),0);
    if (!idx.isValid()) {
        return;
    }
    const QString ipName = idx.data(Qt::UserRole).toString();
    // qDebug() << idx;
    if (ipName == "Block Memory Generator") {
        BlockMemoryGenerator blk_mem_gen;
        if (blk_mem_gen.exec() == QDialog::Accepted) {
            FileManager::instance()->updateIPList();
        }
    } else if (ipName == "Clocking Wizard"){
        ClockingWizard clk_wiz;
        if (clk_wiz.exec() == QDialog::Accepted) {
            FileManager::instance()->updateIPList();
        }
    } else if(ipName == "FIFO Generator") {
        FIFOGenerator fifo_gen;
        if (fifo_gen.exec() == QDialog::Accepted) {
            FileManager::instance()->updateIPList();
        }
    }

}

void IPManager::clickedIP(const QModelIndex &index)
{
    //取选中的这行的第一个元素的index
    const QModelIndex &idx = index.sibling(index.row(),0);
    if (!idx.isValid()) {
        return;
    }
    const QString ipName = idx.data(Qt::UserRole).toString();
    if (ipName == "Block Memory Generator") {
        setDetails(idx.data(Qt::UserRole+1).toString());
    } else if (ipName == "Clocking Wizard"){
        setDetails(idx.data(Qt::UserRole+1).toString());
    } else if(ipName == "FIFO Generator") {
        setDetails(idx.data(Qt::UserRole+1).toString());
    } else {
        setDetails();
    }
}

void IPManager::setDetails(const QString &details)
{
    QPalette palette;
    if (details.isEmpty()) {
        palette.setColor(QPalette::WindowText, QColor(128, 128, 128));
        detailLabel->setAlignment(Qt::AlignCenter);
        detailLabel->setPalette(palette);
        detailLabel->setText("Select an lP to see details");
        return;
    }

    palette.setColor(QPalette::WindowText, Qt::black);
    detailLabel->setAlignment(Qt::AlignLeft);
    detailLabel->setPalette(palette);
    detailLabel->setText(details);
}
