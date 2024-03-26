/**
  ******************************************************************************
  * @File           : FrameView.cpp
  * @Author         : zs
  * @Description    : None
  * @Date           : 24-03-14
  ******************************************************************************
  */
#include "FrameView.h"
#include "blocks/Tiles.h"
#include <QHBoxLayout>
#include <QSplitter>
#include <QComboBox>
#include <QPushButton>
#include "utils/FileHelper.h"
#include <QScrollBar>
#include <QGraphicsView>
#include <QFormLayout>
#include <utility>

FrameView::FrameView(const std::string& tileGridPath, const std::string& tileColorPathLocal,QString projectImplPath,QWidget *parent)
        :  QWidget(parent), scene(new QGraphicsScene(this))
{
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    view = new View("view");
    //主分割窗口
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, parent);

    view->view()->setScene(scene);
    splitterMain->addWidget(view);

    //右部分割窗口
    QSplitter *splitterRight =new QSplitter(Qt::Vertical, splitterMain);
    splitterRight->setOpaqueResize(false);
    QList<int> sizes;
    sizes << 100 << splitterRight->width() - 100; // 左边窗口200像素，右边窗口占据剩余空间
    splitterMain->setSizes(sizes);
    splitterMain->setStretchFactor(0, 0);
    splitterRight->setStretchFactor(1, 1);

    QWidget *rightTopWidget = new QWidget(splitterRight);
    QVBoxLayout *rightTopLayout = new QVBoxLayout(rightTopWidget);



    // 上部分的按钮
    // QPushButton* right_top_load_arch = new QPushButton("加载架构信息");
    QPushButton* rightTopBlockName = new QPushButton("Tile Name Off");
    QPushButton* rightTopSites = new QPushButton("Show Module Details");
    QPushButton* rightTopUsage = new QPushButton("Resource Usage(no arch)");
//    QPushButton* right_top_four = new QPushButton("清空视图");
//    right_top_four->setEnabled(false); // 默认禁止
//    QPushButton* right_top_five = new QPushButton("保存视图");
//    right_top_five->setEnabled(false); // 默认禁止

    // 设置按钮大小不变
    QSize buttonSize = QSize(200, rightTopBlockName->sizeHint().height());
    // right_top_load_arch->setFixedSize(buttonSize);
    rightTopBlockName->setFixedSize(buttonSize);
    rightTopSites->setFixedSize(buttonSize);
    rightTopUsage->setFixedSize(buttonSize);

    // 默认禁止
    rightTopBlockName->setEnabled(false);
    rightTopSites->setEnabled(false);
    rightTopUsage->setEnabled(false);

    // right_top_layout->addWidget(right_top_load_arch, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopBlockName, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopSites, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopUsage, 0, Qt::AlignHCenter);
    // right_top_layout->addWidget(right_top_four, 0, Qt::AlignHCenter);

    QWidget *rightBottomWidget = new QWidget(splitterRight);

    // 显示、隐藏模块名称
    connect(rightTopBlockName, &QPushButton::clicked, [this,rightTopSites,rightTopBlockName]() {
        viewer.updateTilesNameVisibleStatus(!showTilesName);

        // 如果此时显示了site，则隐藏
        if(showSites) {
            viewer.updateSitesVisibleStatus(false);
            rightTopSites->setText("Show Module Details");
        }

        if(showTilesName){
            // 触发信号后，设置tile_name隐藏
            rightTopBlockName->setText("Tile Name On");
        }else{
            // 触发信号后，设置tile_name显示
            rightTopBlockName->setText("Tile Name Off");
        }
        showSites = false;
        showTilesName = !showTilesName;
    });

    // 显示、隐藏内部模块
    connect(rightTopSites, &QPushButton::clicked, [this,rightTopSites,rightTopBlockName]() {
        viewer.updateSitesVisibleStatus(!showSites);
        // 如果此时显示了tile_name，则隐藏
        if(showTilesName) {
            viewer.updateTilesNameVisibleStatus(false);
            rightTopBlockName->setText("Tile Name On");
        }

        if(showSites){
            rightTopSites->setText("Show Module Details");
        }else{
            rightTopSites->setText("Hide Module Details");
        }
        showTilesName = false;
        showSites = !showSites;
    });

    // 资源占用
    connect(rightTopUsage, &QPushButton::clicked, [this,rightTopSites,rightTopBlockName,projectImplPath]() {
        // ==================== 默认使用runs/impl目录下的end_placement.json文件可视化资源使用情况 =========================
        usageJsonPath = projectImplPath + "/end_placement.json";
        if (projectImplPath != "") {
            viewer.setAllTileWhite(scene);
            viewer.updateSitesVisibleStatus(true);
            viewer.updateTilesNameVisibleStatus(false);
            showTilesName = false;
            showSites = true;
            rightTopSites->setText("Hide Module Details");
            rightTopBlockName->setText("Tile Name On");
            viewer.buildPlaceUsageGrid(usageJsonPath.toStdString());
            if (viewer.showPlaceUsageGrid(scene)){
            } else {
            }
        }
        // ==================== 手动选择资源占用文件，方便测试 =========================
//        usageJsonPath = FileHelper::addJsonFile();
//        if (!usageJsonPath.isEmpty()) {
//            viewer.setAllTileWhite(scene);
//            viewer.updateSitesVisibleStatus(true);
//            viewer.updateTilesNameVisibleStatus(false);
//            showTilesName = false;
//            showSites = true;
//            rightTopSites->setText("Hide Module Details");
//            rightTopBlockName->setText("Tile Name On");
//            viewer.buildPlaceUsageGrid(usageJsonPath.toStdString());
//            if (viewer.showPlaceUsageGrid(scene)){
//            } else {
//            }
//        }
    });

    // ========================= 下部分的标签 ===========================
    QFormLayout* formLayout = new QFormLayout();

    // 创建标签和值
    QLabel* tileTypeLabel = new QLabel("Type:");
    QLabel* rowNumLabel = new QLabel("Number of row:");
    QLabel* colNumLabel = new QLabel("Number of col:");
    QLabel* siteNameLabel = new QLabel("Site:");
    QLabel* siteTypeLabel = new QLabel("Site type:");
    tileTypeValue = new QLabel;
    rowNumValue = new QLabel;
    colNumValue = new QLabel;
    siteNameValue = new QLabel;
    siteTypeValue = new QLabel;
    // 设置每个 QLabel 的最小宽度
    const int labelMinimumWidth = 150;
    tileTypeLabel->setMinimumWidth(labelMinimumWidth);
    rowNumLabel->setMinimumWidth(labelMinimumWidth);
    colNumLabel->setMinimumWidth(labelMinimumWidth);
    siteNameLabel->setMinimumWidth(labelMinimumWidth);
    siteTypeLabel->setMinimumWidth(labelMinimumWidth);

    formLayout->addRow(tileTypeLabel, tileTypeValue);
    formLayout->addRow(rowNumLabel, rowNumValue);
    formLayout->addRow(colNumLabel, colNumValue);
    formLayout->addRow(siteNameLabel, siteNameValue);
    formLayout->addRow(siteTypeLabel, siteTypeValue);

    // 创建一个包含表单的widget
    QWidget *formWidget = new QWidget();
    formWidget->setLayout(formLayout);

    // 创建水平和垂直布局来居中formWidget
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QVBoxLayout *vBoxLayout = new QVBoxLayout();

    // 把formWidget添加到水平布局的中间
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(formWidget);
    hBoxLayout->addStretch();

    // 把上面的水平布局添加到垂直布局的中间
    vBoxLayout->addStretch();
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addStretch();

    // 设置right_bottom_widget的布局为上面创建的垂直布局
    rightBottomWidget->setLayout(vBoxLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(splitterMain);
    setLayout(mainLayout);
    setWindowTitle(tr("GridView"));

    // 显示用户当前选择架构
    if (!tileGridPath.empty()){
        viewer.buildTileGridAndCellsMatrix(tileGridPath, tileColorPathLocal);
        if (viewer.showGridView(scene)) {
            rightTopBlockName->setEnabled(true);
            rightTopUsage->setEnabled(true);
            rightTopSites->setEnabled(true);
            rightTopUsage->setText("Resource Usage");
        }
    }

   for (auto& cols : viewer.gridMatrix) {
       for (auto& item : cols) {
           if (item == nullptr) continue;
           // 先断开再链接，避免多次链接
           connect(item, &Tiles::BlockClicked, this, &FrameView::showTileInfo);
           for (SitesBlock* site:item->child_items) {
               connect(site, &SitesSliceL::SiteClicked, this, &FrameView::showSiteInfo);
           }
       }
   }
}


void FrameView::showTileInfo(int col, int row) {
    NormalTile one = viewer.getTileInfo(col,row);
    // 更新标签文本
    tileTypeValue->setText(QString::fromStdString(one.types));
    rowNumValue->setText(QString::number(row));
    colNumValue->setText(QString::number(col));
    siteNameValue->setText("");
    siteTypeValue->setText("");
}

void FrameView::showSiteInfo(int col, int row,bool sites_visible_status,int index){
    NormalTile one = viewer.getTileInfo(col,row);
    // 更新标签文本
    tileTypeValue->setText(QString::fromStdString(one.types));
    rowNumValue->setText(QString::number(row));
    colNumValue->setText(QString::number(col));
    if (sites_visible_status){
        siteNameValue->setText(QString::fromStdString(one.cur_sites[index].name));
        siteTypeValue->setText(QString::fromStdString(one.cur_sites[index].type));
    } else {
        siteNameValue->setText("");
        siteTypeValue->setText("");
    }
}
