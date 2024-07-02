/**
  ******************************************************************************
  * @file           : Form.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/23
  ******************************************************************************
  */

#include <QListWidget>
#include "Form.h"
#include "mainwindow.h"
#include "CustomListWidget.h"
#include "utils/XmlUtilities.h"
#include "base/InitialConfig.h"
#include "utils/ProjectManager.h"

Form *Form::instance()
{
    static Form *_instance = nullptr;
    if (!_instance) {
        _instance = new Form(nullptr);
    }
    return _instance;
}

void Form::paintEvent(QPaintEvent *event)
{
    // 填充Widget空白处
    QPainter painer(this);
    painer.setPen(Qt::NoPen);
    painer.setBrush(Qt::white);
    painer.drawRect(rect());
}


Form::Form(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[Form] Constructing...";
    QPixmap logoPixmap(":/resource/logo.png");
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setPixmap(logoPixmap.scaled(250, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedHeight(75);
    // 整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(logoLabel, 0, Qt::AlignLeft);
    mainLayout->setMargin(30);

    // 水平布局，存放下方两个区域，左侧区域为功能按钮，右侧区域为recentList
    QHBoxLayout *contentLayout = new QHBoxLayout;
    // ========================== 左侧功能区域 ==========================
    QVBoxLayout *leftLayout = new QVBoxLayout; // 左侧功能布局
    // ========================== Quick Start ==========================
    QWidget *leftWidgetOne = new QWidget();
    leftWidgetOne->setFixedHeight(190); // 设置固定高度
    QVBoxLayout *leftGroupLayoutOne = new QVBoxLayout(leftWidgetOne);
    // 创建标题
    QLabel *titleLabelOne = new QLabel("Quick Start");
    titleLabelOne->setStyleSheet("font-size: 32px;padding-left: 4px;color: white; border: none;");
    // 创建按钮
    QPushButton *leftCreateProjectBtn = new QPushButton("Create Project >");
    connect(leftCreateProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onNewTriggered);
    QPushButton *leftOpenProjectBtn = new QPushButton("Open Project >");
    connect(leftOpenProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onOpenTriggered);
    QPushButton *leftOpenExampleBtn = new QPushButton("Open Example Project >");

    leftCreateProjectBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    leftOpenProjectBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    leftOpenExampleBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // 将标题和按钮添加到布局中
    leftGroupLayoutOne->addWidget(titleLabelOne);
    leftGroupLayoutOne->addWidget(leftCreateProjectBtn);
    leftGroupLayoutOne->addWidget(leftOpenProjectBtn);
    leftGroupLayoutOne->addWidget(leftOpenExampleBtn);
    // 添加弹簧固定下方尺寸
    leftGroupLayoutOne->addSpacerItem(new QSpacerItem(10, 14, QSizePolicy::Expanding, QSizePolicy::Minimum));
    // 设置 leftWidgetOne 的样式表
//    "    background-color: rgb(77, 128, 127);"
//    "    background-image: url(:/resource/form_logo_2.png); "
//    "    background-repeat: no-repeat; "
//    "    background-position: center;"
//    "    background-size: cover;"
//    "    border-image: url(:/resource/form_logo_2_rorate.png);"
    leftWidgetOne->setStyleSheet(
            "QWidget {"
                "background-color: rgb(77, 128, 127);"
//            "    border: 1px solid red;"
//                "border-image: url(:/resource/form_logo_2_rorate.png);"
            "}"
            "QPushButton { "
                "border: none; "
                "text-align: left; "
                "font-size: 15px; "
                "background-color: transparent; " /* 设置背景为透明 */
                "color: white; "
                "padding-left: 10px; "
            "} "
            "QPushButton:hover { "
                "text-decoration: underline; "
            "}"
    );

    // 将 QWidget 添加到左侧布局
    leftLayout->addWidget(leftWidgetOne);
    leftLayout->addSpacing(20); // 间距

    // ========================== Tasks ==========================
    QWidget *leftWidgetTwo = new QWidget();
    leftWidgetTwo->setFixedHeight(190); // 设置固定高度
    QVBoxLayout *leftGroupLayoutTwo = new QVBoxLayout(leftWidgetTwo);
    // 创建标题
    QLabel *titleLabelTwo = new QLabel("Tasks");
    titleLabelTwo->setStyleSheet("font-size: 32px;padding-left: 4px;");
    // 创建按钮
    QPushButton *button4 = new QPushButton("Manage IP >");
    QPushButton *button5 = new QPushButton("Open Hardware Manager >");
    QPushButton *button6 = new QPushButton("Hub Store >");
    button4->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button5->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button6->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    leftGroupLayoutTwo->addWidget(titleLabelTwo);
    leftGroupLayoutTwo->addWidget(button4);
    leftGroupLayoutTwo->addWidget(button5);
    leftGroupLayoutTwo->addWidget(button6);
    leftGroupLayoutTwo->addSpacerItem(new QSpacerItem(10, 14, QSizePolicy::Expanding, QSizePolicy::Minimum));

    leftWidgetTwo->setStyleSheet(
            "QWidget {"
            "    background-color: rgb(156, 201, 223);"
            "    border: none;"
            "}"
            "QPushButton { "
                "padding-left: 10px; "
            "    border: none;"
                "text-align: left; "
                "font-size: 15px; "
                "background-color: transparent; "
                "color: rgb(64, 64, 64); "
            "} "
            "QPushButton:hover { "
                "text-decoration: underline; "
            "}"
    );
    leftLayout->addWidget(leftWidgetTwo);
    leftLayout->addSpacing(20); // 间距

    // ========================== Learning Center ==========================

    QWidget *leftWidgetThree = new QWidget();
    leftWidgetThree->setFixedHeight(190); // 设置固定高度
    QVBoxLayout *leftGroupLayoutThree = new QVBoxLayout(leftWidgetThree);
    // 创建标题
    QLabel *titleLabelThree = new QLabel("Learning Center");
    titleLabelThree->setStyleSheet("font-size: 32px;padding-left: 4px;");
    // 创建按钮
    QPushButton *button7 = new QPushButton("Documention One >");
    QPushButton *button8 = new QPushButton("Documention Two >");
    QPushButton *button9 = new QPushButton("Documention Three >");
    button7->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button8->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button9->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    leftGroupLayoutThree->addWidget(titleLabelThree);
    leftGroupLayoutThree->addWidget(button7);
    leftGroupLayoutThree->addWidget(button8);
    leftGroupLayoutThree->addWidget(button9);
    leftGroupLayoutThree->addSpacerItem(new QSpacerItem(10, 14, QSizePolicy::Expanding, QSizePolicy::Minimum));

    leftWidgetThree->setStyleSheet(
            "QWidget {"
            "    background-color: rgb(156, 201, 223);"
            "    border: none;"
            "}"
            "QPushButton { "
                "padding-left: 10px; "
                "border: none; "
                "text-align: left; "
                "font-size: 15px; "
                "background-color: transparent; "
                "color: rgb(64, 64, 64); "
            "} "
            "QPushButton:hover { "
                "text-decoration:underline;"
            "}"
    );
    leftLayout->addWidget(leftWidgetThree);

    // ========================== 调整左侧部分布局 ==========================
    QWidget *leftWidget = new QWidget;
    leftWidget->setLayout(leftLayout);
//    leftWidget->setFixedHeight(600); // 设置左侧布局的固定高度

    contentLayout->addWidget(leftWidget);
    contentLayout->setAlignment(leftWidget, Qt::AlignTop); // 设置顶部对齐

    // ========================== 右侧功能区域 ==========================
    QVBoxLayout *rightLayout = new QVBoxLayout; // 右侧功能区域
    // 模拟数据
    QStringList titles = {"Project 1", "Project 2", "Project 3"};
    QStringList paths = {"/path/to/project1", "/path/to/project2", "/path/to/project3"};
    // ========================== Recent Project ============================
    QGroupBox *rightGroupBoxOne = new QGroupBox("Recent Projects");
    QVBoxLayout *rightGroupLayoutOne = new QVBoxLayout(rightGroupBoxOne);
    rightGroupBoxOne->setFixedHeight(500);  // 设置右侧区域的固定宽度
    rightGroupBoxOne->setFixedWidth(700);  // 设置右侧区域的固定宽度
    QListWidget *listWidget1 = new QListWidget;
    std::vector<XmlRecent> recentLists = XmlUtilities::instance().getRecentListFromFatherElementName(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS");
    for (int i = 0; i < recentLists.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem;
        listWidgetItem->setSizeHint(QSize(0, 50));  // 设置 item 的大小
        listWidget1->addItem(listWidgetItem);  // 将 item 添加到 listWidget 中
        // 创建 custom list item
        CustomListWidget *customListItem = new CustomListWidget(QString::fromStdString(extractProjectName(recentLists.at(i).getPath())), QString::fromStdString(recentLists.at(i).getPath()), listWidget1);
        listWidget1->setItemWidget(listWidgetItem, customListItem);

        connect(customListItem, &CustomListWidget::getProjectPath, this, [](const QString &projectPath){
            ProjectManager::instance().openProject(projectPath);
        });
    }
    // 设置 QListWidget 的样式表
    rightGroupBoxOne->setStyleSheet(
            "QListWidget { border: none; }"
            "QListWidget::item:hover {"
            "    background-color: #ccdefd;"
            "}"
            "QGroupBox { font-size: 28px; }"
    );
    rightGroupLayoutOne->addWidget(listWidget1);
    // ========================== Recent IP ============================
    QGroupBox *rightGroupBoxTwo = new QGroupBox("Recent IP Locations");
    QVBoxLayout *rightGroupLayoutTwo = new QVBoxLayout(rightGroupBoxTwo);
    rightGroupBoxTwo->setFixedWidth(700);  // 设置右侧区域的固定宽度
    QListWidget *listWidget2 = new QListWidget;

    for (int i = 0; i < titles.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem;
        listWidgetItem->setSizeHint(QSize(0, 50));  // 设置 item 的大小
        listWidget2->addItem(listWidgetItem);  // 将 item 添加到 listWidget 中
        // 创建 custom list item
        CustomListWidget *customListItem = new CustomListWidget(titles[i], paths[i], listWidget2);
        listWidget2->setItemWidget(listWidgetItem, customListItem);

//        connect(customListItem, &CustomListWidget::getProjectPath, this, [](const QString &projectPath){
//            ProjectManager::instance().openProject(projectPath);
//        });
    }
    // 设置 QListWidget 的样式表
    rightGroupBoxTwo->setStyleSheet(
            "QListWidget { border: none; }"
            "QListWidget::item:hover {"
            "    background-color: #ccdefd;"
            "}"
            "QGroupBox { font-size: 28px; }"
    );
    rightGroupLayoutTwo->addWidget(listWidget2);
    // ========================== 绑定右侧布局 ============================
    rightLayout->addWidget(rightGroupBoxOne);
    rightLayout->addWidget(rightGroupBoxTwo);
    contentLayout->addLayout(rightLayout);
    // 将内容布局添加到主布局中
    mainLayout->addLayout(contentLayout);
}

Form::~Form(){
    qDebug() << "[Form] Distructing...";
}


std::string Form::extractProjectName(const std::string& path) {
    // 查找最后一个斜杠
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        // 截取到最后一个斜杠
        std::string parentPath = path.substr(0, lastSlash);
        // 再次查找上一个斜杠
        size_t secondLastSlash = parentPath.find_last_of("/\\");
        if (secondLastSlash != std::string::npos) {
            // 提取倒数第二个目录名
            return parentPath.substr(secondLastSlash + 1, lastSlash - secondLastSlash - 1);
        }
    }
    return "Project Name"; // 如果路径格式不正确
}
