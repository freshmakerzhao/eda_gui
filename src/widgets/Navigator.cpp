#include "navigator.h"
#include "mainwindow.h"
#include "utils/TaskManager.h"
#include "utils/StringUtilities.h"
#include "utils/ProjectManager.h"
#include "wizard/Wizard.h"

Navigator *Navigator::instance(QWidget *parent)
{
    static Navigator *_instance = nullptr;
    if (!_instance) {
        _instance = new Navigator(parent);
    }
    return _instance;
}

void Navigator::loadFile(Project *proj)
{
    /*如果加载的是同一个工程,刷新工程文件表,否则运行新进程,在新进程加载工程*/
    if (p != nullptr && p != proj) { // 加载的不是同一个工程
        // 运行新进程，在新进程加载工程
        ProjectManager::instance().startProcess(proj);
        return;
    }

    p = proj;

    TaskManager::instance().setParams(proj->getAllParams());
    TaskManager::instance().sourcePathList = proj->sourceList;
    TaskManager::instance().constraintPathList = proj->constraintList;

    qDebug() << "[Navigator] loadFile...";
    qDebug() << "[Navigator] proj->sourceList：" << proj->sourceList;
    qDebug() << "[Navigator] proj->constraintList：" << proj->constraintList;

    QTreeWidgetItem *nameItem = new QTreeWidgetItem(navTree);
    nameItem->setText(0, proj->getParam("name"));
    sourceItem = new QTreeWidgetItem(nameItem);
    sourceItem->setText(0, "sources");
    foreach (const QString &file, proj->sourceList) {
        QTreeWidgetItem *sourcefile = new QTreeWidgetItem(sourceItem);
        sourcefile->setText(0, QFileInfo(file).fileName());
        sourcefile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    QTreeWidgetItem *constraintsItem = new QTreeWidgetItem(nameItem);
    constraintsItem->setText(0, "constraints");
    foreach (const QString &file, proj->constraintList) {
        QTreeWidgetItem *constraintsfile = new QTreeWidgetItem(constraintsItem);
        constraintsfile->setText(0, QFileInfo(file).fileName());
        constraintsfile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    // TODO:Load doc, ip
    // QTreeWidgetItem *docItem = new QTreeWidgetItem(nameItem);
    // docItem->setText(0, "doc");

    // QTreeWidgetItem *ipItem = new QTreeWidgetItem(nameItem);
    // ipItem->setText(0, "ip");
}

void Navigator::clickedFile(QTreeWidgetItem *item)
{ 
    QString path = item->data(0, Qt::UserRole).toString();
    // qDebug() << "Navigator:: open " << path;
    MainWindow::instance()->createEditorTab(path);
    navTree->clearSelection(); // 清除navTree选中状态
}

void Navigator::showContextMenu(const QPoint &pos) {
    QMenu contextMenu;
    QAction closeProject("Close Project");
    QAction addSources("Add/Create...");
    // QAction addConstraints("Add Constraints");
    QAction removeFileAction("Remove File from Project");
    // QAction setAsTopAction("Set As Top");
    QTreeWidgetItem *rightClickedItem = navTree->itemAt(pos); // 右键点击位置
    if (rightClickedItem == nullptr) {
        // qDebug() << "Empty Item";
        navTree->clearSelection(); // 清除navTree选中状态
        return;
    }
    if (rightClickedItem->parent() == nullptr) {
        contextMenu.addAction(&closeProject);
        connect(&closeProject, &QAction::triggered, this, &Navigator::closeProjectAction);
        contextMenu.addAction(&addSources);
        connect(&addSources, &QAction::triggered, this, &Navigator::addSourcesAction);
        // contextMenu.addAction(&addConstraints);
        // connect(&addConstraints, &QAction::triggered, this, &Navigator::addConstraintsAction);
    } else if (QFileInfo(rightClickedItem->data(0, Qt::UserRole).toString()).isFile()) {
        contextMenu.addAction(&removeFileAction);
        connect(&removeFileAction, &QAction::triggered, this, &Navigator::removeFileAction);
    }
    // if (rightClickedItem->parent() == sourceItem) {
    //     contextMenu.addAction(&setAsTopAction);
    //     // TODO 设置顶层模块操作
    // }
    contextMenu.exec(navTree->mapToGlobal(pos));
    navTree->clearSelection(); // 清除navTree选中状态
}

void Navigator::closeProjectAction()
{
    if (!MainWindow::instance()->cleanEditorTab()) {
        return;
    }
    delete navTree->topLevelItem(0);
    delete p;
    p = nullptr;
    TaskManager::instance().cleanParams();
}

void Navigator::addSourcesAction()
{
    Wizard wizard(MainWindow::instance(), 1, p);
    wizard.exec();
    p->makeProject();
    delete navTree->topLevelItem(0);
    loadFile(p);
}

void Navigator::removeFileAction()
{
    QString path = navTree->currentItem()->data(0, Qt::UserRole).toString();
    QFileInfo fileInfo(path);
    QString folderName = fileInfo.dir().dirName();
    qDebug() << folderName;
    if (folderName == "sources") {
        p->sourceList.removeOne(path);
    } else if (folderName == "constraints") {
        p->constraintList.removeOne(path);
    }

    QFile file(navTree->currentItem()->text(0));
    file.remove();
    delete navTree->currentItem();
    p->makeProject();
}

bool Navigator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == navTree->viewport()) {
        //点击树的空白,取消选中
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                QModelIndex index = navTree->indexAt(mouseEvent->pos());
                if (!index.isValid()) {
                    navTree->setCurrentIndex(QModelIndex());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

Navigator::Navigator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[Navigator] Constructing...";
    navTree = new QTreeWidget(this);
    navTree->setStyleSheet("QTreeWidget::item { height: 32px; }");
    navTree->viewport()->installEventFilter(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(navTree);
    layout->setMargin(0);

    navTree->setContextMenuPolicy(Qt::CustomContextMenu);

    // 点击打开文件
    QObject::connect(navTree, &QTreeWidget::itemDoubleClicked, this, &Navigator::clickedFile);
    // 绑定右键菜单
    QObject::connect(navTree, &QTreeWidget::customContextMenuRequested, this, &Navigator::showContextMenu);


    navTree->setColumnCount(1);
    navTree->setHeaderHidden(true);
}

Navigator::~Navigator()
{
    qDebug() << "[Navigator] Distructing...";
}

bool Navigator::canSetTile(Project *proj) {

    if (p == nullptr && proj == nullptr) {
        // 第一次开启项目
        return true;
    }else if (p != nullptr && p != proj) {
        // 加载的不是同一个工程
        return false;
    } else {
        return true;
    }
}

