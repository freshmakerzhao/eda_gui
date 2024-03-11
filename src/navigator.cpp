#include "navigator.h"
#include "mainwindow.h"
#include "taskview.h"

Navigator *Navigator::instance(QWidget *parent)
{
    static Navigator *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new Navigator(parent);
    }
    return m_instance;
}

Navigator::Navigator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[Navigator] Constructing...";
    navTree = new QTreeWidget(this);
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
    // navTree->expandAll();
}

Navigator::~Navigator()
{
    qDebug() << "[Navigator] Distructing...";
}

void Navigator::loadFile(Project *project)
{
    // 检查工程是否已打开
    if (pn.contains(project->path)) {
        QMessageBox::question(this, "Warning", "This Project is already open!",
                                                                QMessageBox::Yes);
        return;
    }
    pn.push_back(project->path);
    TaskView::instance()->sourceList = project->sourcePathList;
    QTreeWidgetItem *nameItem = new QTreeWidgetItem(navTree);
    nameItem->setText(0, project->name);
    QTreeWidgetItem *sourceItem = new QTreeWidgetItem(nameItem);
    sourceItem->setText(0, "sources");
    foreach (const QString &file, project->sourcePathList) {
        QTreeWidgetItem *sourcefile = new QTreeWidgetItem(sourceItem);
        sourcefile->setText(0, QFileInfo(file).fileName());
        sourcefile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    QTreeWidgetItem *constrainsItem = new QTreeWidgetItem(nameItem);
    constrainsItem->setText(0, "constrains");
    foreach (const QString &file, project->constraintPathList) {
        QTreeWidgetItem *constrainsfile = new QTreeWidgetItem(constrainsItem);
        constrainsfile->setText(0, QFileInfo(file).fileName());
        constrainsfile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    // TODO:Load doc, ip
    QTreeWidgetItem *docItem = new QTreeWidgetItem(nameItem);
    docItem->setText(0, "doc");

    QTreeWidgetItem *ipItem = new QTreeWidgetItem(nameItem);
    ipItem->setText(0, "ip");
}

void Navigator::clickedFile(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    MainWindow::instance()->createEditorTab(path);
}

void Navigator::showContextMenu(const QPoint &pos) {
    QMenu contextMenu;
    QAction closeProject("Close Project");
    QAction addSources("Add Sources");
    QAction addConstraints("Add Constrains");
    QAction deleteFileAction("Delete File");
    QAction setActiveProject("Set As Active Project");
    if (navTree->currentItem() == nullptr) {
        qDebug() << "Empty Item";
        return;
    }
    if (navTree->currentItem()->parent() == nullptr) {
        contextMenu.addAction(&closeProject);
        connect(&closeProject, &QAction::triggered, this, &Navigator::closeProjectAction);
        contextMenu.addAction(&setActiveProject);
        connect(&setActiveProject, &QAction::triggered, this, &Navigator::setActiveProjectAction);
        contextMenu.addAction(&addSources);
        connect(&addSources, &QAction::triggered, this, &Navigator::addSourcesAction);
        contextMenu.addAction(&addConstraints);
        connect(&addConstraints, &QAction::triggered, this, &Navigator::addConstraintsAction);
    } else if (QFileInfo(navTree->currentItem()->data(0, Qt::UserRole).toString()).isFile()) {
        contextMenu.addAction(&deleteFileAction);
        connect(&deleteFileAction, &QAction::triggered, this, &Navigator::deleteFileAction);

    }
    contextMenu.exec(navTree->mapToGlobal(pos));
}

void Navigator::closeProjectAction()
{
    auto it = std::find(pn.begin(), pn.end(), navTree->currentItem()->data(0, Qt::UserRole).toString());
    if (it != pn.end()) {
        pn.erase(it);
    }
    delete navTree->currentItem();
}

void Navigator::setActiveProjectAction()
{
    // 取消之前已加粗的项目
    for (int i = 0; i < navTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = navTree->topLevelItem(i);
        QFont font = item->font(0);
        font.setBold(false);
        item->setFont(0, font);
    }

    QTreeWidgetItem *item = navTree->currentItem();
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);
}

void Navigator::addSourcesAction()
{
    QString path = navTree->currentItem()->data(0, Qt::UserRole).toString();
    qDebug() << "path:" << path;
    QString addSourcesPath = path + "/sources/";
    qDebug() << "addSources path:" << addSourcesPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, addSourcesPath + QFileInfo(file).fileName());
        }
    }
    // updateItems(path);
}

void Navigator::addConstraintsAction()
{
    QString path = navTree->currentItem()->data(0, Qt::UserRole).toString();
    QString constrainsPath = path + "/constrains/";
    qDebug() << "constrains path:" << constrainsPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, constrainsPath + QFileInfo(file).fileName());
        }
    }
    // updateItems(path);
}

void Navigator::deleteFileAction()
{
    QFile file(navTree->currentItem()->text(0));
    file.remove();
    delete navTree->currentItem();
    // navTree->expandAll();
}

bool Navigator::eventFilter(QObject *obj, QEvent *event)
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


