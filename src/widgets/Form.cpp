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
    logoLabel->setPixmap(logoPixmap.scaled(350, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedHeight(75);
    // 整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(logoLabel, 0, Qt::AlignLeft);
    mainLayout->setMargin(30);

    // 水平布局，存放下方两个区域，左侧区域为功能按钮，右侧区域为recentList
    QHBoxLayout *contentLayout = new QHBoxLayout;
    // ========================== 左侧功能区域 ==========================
    // ========================== Quick Start ==========================
    QVBoxLayout *leftLayout = new QVBoxLayout; // 左侧功能布局
    QGroupBox *leftGroupBoxOne = new QGroupBox("Quick Start");
    QVBoxLayout *leftGroupLayoutOne = new QVBoxLayout(leftGroupBoxOne);
    QPushButton *leftCreateProjectBtn = new QPushButton("Create Project >");
    leftCreateProjectBtn->setFixedWidth(300);
    connect(leftCreateProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onNewTriggered);
    QPushButton *leftOpenProjectBtn = new QPushButton("Open Project >");
    leftOpenProjectBtn->setFixedWidth(300);
    connect(leftOpenProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onOpenTriggered);
    QPushButton *leftOpenExampleBtn = new QPushButton("Open Example Project >");
    leftOpenExampleBtn->setEnabled(false);
    leftGroupLayoutOne->addWidget(leftCreateProjectBtn);
    leftGroupLayoutOne->addWidget(leftOpenProjectBtn);
    leftGroupLayoutOne->addWidget(leftOpenExampleBtn);
    leftGroupBoxOne->setStyleSheet(
           "QWidget { background-image: url(:/resource/white.png); }"
           "QGroupBox { font-family: Console; font-size: 30px; }"
           "QPushButton { border: none; text-align: left; font-size: 22px; }"
           "QPushButton:hover { color: #4f7cce; text-decoration:underline;}"
    );
    leftLayout->addWidget(leftGroupBoxOne);
    // ========================== Tasks ==========================
    QGroupBox *leftGroupBoxTwo = new QGroupBox("Tasks");
    QVBoxLayout *leftGroupLayoutTwo = new QVBoxLayout(leftGroupBoxTwo);
    QPushButton *button4 = new QPushButton("Manage IP >");
    QPushButton *button5 = new QPushButton("Open Hardware Manager >");
    QPushButton *button6 = new QPushButton("XHub Store >");
    button4->setEnabled(false);
    button5->setEnabled(false);
    button6->setEnabled(false);
    leftGroupLayoutTwo->addWidget(button4);
    leftGroupLayoutTwo->addWidget(button5);
    leftGroupLayoutTwo->addWidget(button6);
    leftGroupBoxTwo->setStyleSheet(
            "QWidget { background-image: url(:/resource/white.png); }"
            "QGroupBox { font-family: Console; font-size: 30px; }"
            "QPushButton { border: none; text-align: left; font-size: 22px; }"
            "QPushButton:hover { color: #4f7cce; text-decoration:underline;}"
    );
    leftLayout->addWidget(leftGroupBoxTwo);
    // ========================== Learning Center ==========================
    QGroupBox *leftGroupBoxThree = new QGroupBox("Learning Center");
    QVBoxLayout *leftGroupLayoutThree = new QVBoxLayout(leftGroupBoxThree);
    QPushButton *button7 = new QPushButton("Documention One >");
    QPushButton *button8 = new QPushButton("Documention Two >");
    QPushButton *button9 = new QPushButton("Documention Three >");
    button7->setEnabled(false);
    button8->setEnabled(false);
    button9->setEnabled(false);
    leftGroupLayoutThree->addWidget(button7);
    leftGroupLayoutThree->addWidget(button8);
    leftGroupLayoutThree->addWidget(button9);
    leftGroupBoxThree->setStyleSheet(
            "QWidget { background-image: url(:/resource/white.png); }"
            "QGroupBox { font-family: Console; font-size: 30px; }"
            "QPushButton { border: none; text-align: left; font-size: 22px; }"
            "QPushButton:hover { color: #4f7cce; text-decoration:underline;}"
    );
    leftLayout->addWidget(leftGroupBoxThree);
    contentLayout->addLayout(leftLayout);


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
            "QGroupBox { font-family: Console; font-size: 30px; }"
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
            "QGroupBox { font-family: Console; font-size: 30px; }"
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
