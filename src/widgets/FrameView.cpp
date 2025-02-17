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
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include "blocks/Block.h"

FrameView::FrameView(const std::string& tileGridPath, const std::string& tileColorPathLocal, const std::string& pinsInfoPathLocal, QString projectImplPath,QWidget *parent)
        :  QWidget(parent), scene(new QGraphicsScene(this))
{
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    view = new View("view");
    //主分割窗口
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, parent);
    splitterMain->setHandleWidth(15);

    view->view()->setScene(scene);
    splitterMain->addWidget(view);

    //右部分割窗口
    QSplitter *splitterRight =new QSplitter(Qt::Vertical, splitterMain);

    // 设置拉伸因子，优先拉伸右侧窗口
    splitterMain->setStretchFactor(0, 1);
    splitterMain->setStretchFactor(1, 0);

    splitterRight->setStretchFactor(1, 1);


    //搜索框
    QWidget* searchWidget = new QWidget(splitterRight);
    QVBoxLayout* searchVLayout = new QVBoxLayout;
    QHBoxLayout* searchHLayout = new QHBoxLayout;
    searchBox = new SearchBox(&viewer);


    QPushButton* searchButton = new QPushButton("search");
    searchVLayout->addWidget(new QLabel("Search cell: "));

    searchHLayout->addWidget(searchBox);
    searchHLayout->addWidget(searchButton);
    searchVLayout->addLayout(searchHLayout);
    searchWidget->setLayout(searchVLayout);

    connect(searchBox, &QLineEdit::returnPressed, this, &FrameView::showCell);
    connect(searchButton, &QPushButton::clicked, this, &FrameView::showCell);

    // 上部分的按钮
    QWidget *rightTopWidget = new QWidget(splitterRight);
    QVBoxLayout *rightTopLayout = new QVBoxLayout(rightTopWidget);
    // QPushButton* right_top_load_arch = new QPushButton("加载架构信息");
    QPushButton* rightTopBlockName = new QPushButton("Tile Name Off");
    QPushButton* rightTopSites = new QPushButton("Show Module Details");
    QPushButton* rightTopClockRegion = new QPushButton("Show Clock Region");
    QPushButton* rightTopUsage = new QPushButton("Resource Usage(no arch)");
//    QPushButton* right_top_four = new QPushButton("清空视图");
//    right_top_four->setEnabled(false); // 默认禁止
//    QPushButton* right_top_five = new QPushButton("保存视图");
//    right_top_five->setEnabled(false); // 默认禁止

    // 设置按钮大小不变
    QSize buttonSize = QSize(rightTopUsage->sizeHint().width(), rightTopUsage->sizeHint().height());
    rightTopBlockName->setFixedSize(buttonSize);
    rightTopSites->setFixedSize(buttonSize);
    rightTopClockRegion->setFixedSize(buttonSize);
    rightTopUsage->setFixedSize(buttonSize);

    // 默认禁止
    rightTopBlockName->setEnabled(false);
    rightTopSites->setEnabled(false);
    rightTopUsage->setEnabled(false);

    rightTopLayout->addWidget(rightTopBlockName, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopSites, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopClockRegion, 0, Qt::AlignHCenter);
    rightTopLayout->addWidget(rightTopUsage, 0, Qt::AlignHCenter);

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

    // 显示、隐藏 Clock Region
    connect(rightTopClockRegion, &QPushButton::clicked, [this,rightTopClockRegion]() {
        viewer.updateClockRegionVisibleStatus(!showClockRegion);
        if(showClockRegion){
            rightTopClockRegion->setText("Show Clock Region");
        }else{
            rightTopClockRegion->setText("Hide Clock Region");
        }
        showClockRegion = !showClockRegion;
    });

    // 资源占用
    connect(rightTopUsage, &QPushButton::clicked, [this,rightTopSites,rightTopBlockName,projectImplPath]() {
#if ONLY_COMPILE_GRIDVIEW
        // -------------------- 手动选择资源占用文件，方便测试 -----------------------------------
        usageJsonPath = FileHelper::addJsonFile();
#else
    // ----------- 默认使用runs/impl目录下的place.json文件可视化资源使用情况 -----------
    QDir dir(projectImplPath);
    usageJsonPath = dir.filePath("route.json");
#endif
        if (QFile(usageJsonPath).exists()) {
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
        } else {
            qDebug() << usageJsonPath <<  "is not exists";
        }

    });

    // ========================= 下部分的标签 ===========================
    QFormLayout* formLayout = new QFormLayout();

    // 创建标签和值
    QLabel* tileTypeLabel = new QLabel("Type:");
    QLabel* rowNumLabel = new QLabel("Number of row:");
    QLabel* colNumLabel = new QLabel("Number of col:");
    QLabel* siteNameLabel = new QLabel("Site:");
    QLabel* siteTypeLabel = new QLabel("Site type:");
    QLabel* NameLabel = new QLabel("Name:");
    QLabel* TypeLabel = new QLabel("Type:");
    tileTypeValue = new QLabel;
    rowNumValue = new QLabel;
    colNumValue = new QLabel;
    siteNameValue = new QLabel;
    siteTypeValue = new QLabel;
    NameValue = new QLabel;
    TypeValue = new QLabel;

    tileTypeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rowNumLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colNumLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    siteNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    siteTypeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    NameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TypeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    tileTypeValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rowNumValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colNumValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    siteNameValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    siteTypeValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    NameValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TypeValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    formLayout->addRow(tileTypeLabel, tileTypeValue);
    formLayout->addRow(rowNumLabel, rowNumValue);
    formLayout->addRow(colNumLabel, colNumValue);
    formLayout->addRow(siteNameLabel, siteNameValue);
    formLayout->addRow(siteTypeLabel, siteTypeValue);
    formLayout->addRow(NameLabel, NameValue);
    formLayout->addRow(TypeLabel, TypeValue);

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
        viewer.buildTileGridAndCellsMatrix(tileGridPath, tileColorPathLocal, pinsInfoPathLocal);
        if (viewer.showGridView(scene)) {
            rightTopBlockName->setEnabled(true);
            rightTopUsage->setEnabled(true);
            rightTopSites->setEnabled(true);
            rightTopUsage->setText("Resource Usage");
        }

        // 添加搜索框提示词
        searchBox->setWords();
    }

   for (auto& cols : viewer.gridMatrix) {
       for (auto& item : cols) {
           if (item == nullptr) continue;
           // 先断开再链接，避免多次链接
           connect(item, &Tiles::BlockClicked, this, &FrameView::showTileInfo);
           for (SitesBlock* site:item->child_items) {
               connect(site, &SitesBlock::SiteClicked, this, &FrameView::showSiteInfo);
               for (BelsBlock* bel: site->child_bel_items) {
                   connect(bel, &BelsBlock::BelClicked, this, &FrameView::showBelInfo);
               }
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
    NameValue->setText("");
    TypeValue->setText("");
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
    NameValue->setText("");
    TypeValue->setText("");
}

void FrameView::showBelInfo(int col, int row, int site_index, bool bel_visible_status, int index, const std::string &bel_type, const std::string &name) {
    NormalTile one = viewer.getTileInfo(col,row);
    // 更新标签文本
    tileTypeValue->setText(QString::fromStdString(one.types));
    rowNumValue->setText(QString::number(row));
    colNumValue->setText(QString::number(col));
    if (bel_visible_status){
        siteNameValue->setText(QString::fromStdString(one.cur_sites[site_index].name));
        siteTypeValue->setText(QString::fromStdString(one.cur_sites[site_index].type));
        NameValue->setText(QString::fromStdString(name));
        TypeValue->setText(QString::fromStdString(bel_type));
    } else {
        siteNameValue->setText("");
        siteTypeValue->setText("");
        NameValue->setText("");
        TypeValue->setText("");
    }
}

bool FrameView::searchCell(const std::string &cell_name) {
    size_t pos = cell_name.find("_X");
    std::string site_type = cell_name.substr(0, pos);
    qDebug() << "site_type:" <<  QString::fromStdString(site_type);
    auto it = viewer.siteBlockMap.find(site_type);
    if(it == viewer.siteBlockMap.end()) {
        qDebug() << "not find site_type: " << QString::fromStdString(site_type);
        return false;
    }

    pos = cell_name.find('/');
    if(pos != std::string::npos) {
        std::string site_name = cell_name.substr(0, pos);
        auto it = viewer.siteBlockMap[site_type].find(site_name);
        if(it == viewer.siteBlockMap[site_type].end()) {
            qDebug() << "not find site_name: " << QString::fromStdString(site_name);
            return false;
        }
        SitesBlock* site_block = viewer.siteBlockMap[site_type][site_name];
        for(auto bel_block : site_block->child_bel_items) {
            if(cell_name == bel_block->getName()) {
                view->cellLocationShow(bel_block);
                bel_block->launchClicked();
                return true;
            }
        }
    } else {
        auto it = viewer.siteBlockMap[site_type].find(cell_name);
        if(it == viewer.siteBlockMap[site_type].end())
            return false;
        view->cellLocationShow(viewer.siteBlockMap[site_type][cell_name]);
        viewer.siteBlockMap[site_type][cell_name]->launchClicked();
        return true;
    }
    return false;
}

void FrameView::showCell() {
    if(!searchCell(searchBox->text().toStdString())) {
        searchBox->setPlaceholderText("Not found");
        searchBox->clear();
    }
}


SearchBox::SearchBox(ChipGridOperations* view, QWidget *parent):view(view) {
    completer = new QCompleter(this);
    wordListModel = new QStringListModel(this);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    setCompleter(completer);

    connect(completer, QOverload<const QString &>::of(&QCompleter::highlighted), [=](const QString &text) {
        std::string site_type = text.split("_X")[0].toStdString();
        std::string site_name_in = text.split("/")[0].toStdString();
//
        qDebug() << "selected_word:" << text;
//
        if (!text.contains("_X")) { //匹配site类型
            qDebug() << "is site type";
            for (auto site: view->siteBlockMap[site_type]) {
                QString site_name = QString::fromStdString(site.first);
                if (!wordList.contains(site_name)) {
                    wordList << site_name;
                }
            }
//            wordListModel->setStringList(wordList);
        }

    });

    connect(completer, QOverload<const QString &>::of(&QCompleter::activated), [=](const QString &text) {
        wordListModel->setStringList(wordList);
//        qDebug() << "is site type";
//      this->setText(text);
    });

    connect(this, &QLineEdit::textChanged, [=](const QString &text) {
        if(text.isEmpty()) {
            clearWords();
            return;
        }

        int row_count = completer->completionCount();
        qDebug() << "row count:" << row_count;

        if(row_count == 1) { //当提示框只有一个关键词时
            QString current_word = completer->currentCompletion();
            std::string site_type = current_word.split("_X")[0].toStdString();
            std::string site_name_in = current_word.split("/")[0].toStdString();

            qDebug() << "current_word:" << current_word << "from searchUpdate()";

            if (!current_word.contains("_X")) { //匹配site类型
                for (auto site: view->siteBlockMap[site_type]) {
                    QString site_name = QString::fromStdString(site.first);
                    if (!wordList.contains(site_name)) {
                        wordList << site_name;
                    }
                }
                wordListModel->setStringList(wordList);

                if (text.size() > textLast.size())
                    this->setText(current_word + "_X"); //自动补全

                textLast = text;
                return;
            }

            if (text.size() > textLast.size()) { //当用户是在添加字符串时
                this->setText(current_word); //有且只有一个提示词,则自动补全
            }

            if (!current_word.contains('/')) { //匹配site_name
                for (auto bel: view->siteBlockMap[site_type][site_name_in]->child_bel_items) {
                    QString bel_name = QString::fromStdString(bel->getName());
                    if (!wordList.contains(bel_name)) {
                        wordList << bel_name;
                    }
                }
                wordListModel->setStringList(wordList);
            }
        }
        textLast = text;

    });
}

void SearchBox::clearWords() {
    wordList.clear();
    for (auto type_word : view->siteBlockMap) {
        wordList << QString::fromStdString(type_word.first);
    }
    wordListModel->setStringList(wordList);
}

void SearchBox::setWords() {
    for (auto type_word : view->siteBlockMap) {
        wordList << QString::fromStdString(type_word.first);
    }
    completer->setModel(wordListModel);
    wordListModel->setStringList(wordList);
}
