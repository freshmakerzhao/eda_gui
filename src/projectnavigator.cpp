#include "projectnavigator.h"

ProjectNavigator *ProjectNavigator::instance(QWidget *parent)
{
    static ProjectNavigator *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new ProjectNavigator(parent);
    }
    return m_instance;
}

ProjectNavigator::ProjectNavigator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[ProjectNavigator] Constructing...";
    treeWidget = new QTreeWidget(this);
    treeWidget->viewport()->installEventFilter(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(treeWidget);
    layout->setMargin(0);

    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // 点击打开文件
    QObject::connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &ProjectNavigator::clickedFile);
    // 绑定右键菜单
    QObject::connect(treeWidget, &QTreeWidget::customContextMenuRequested, this, &ProjectNavigator::showContextMenu);


    treeWidget->setColumnCount(1);
    treeWidget->setHeaderHidden(true);
    treeWidget->expandAll();
}

ProjectNavigator::~ProjectNavigator()
{
    qDebug() << "[ProjectNavigator] Distructing...";
}

void ProjectNavigator::updateItems(const QString &path)
{
    // 检查路径是否在projectNode集合中
    if (!projectNode.contains(path)) {
        // 如果路径不在projectNode集合中，就插入
        projectNode.insert(path);
    }

    // 刷新ui->treeWidget
    while (treeWidget->topLevelItemCount() > 0) {
        QTreeWidgetItem* item = treeWidget->takeTopLevelItem(treeWidget->topLevelItemCount() - 1);
        // qDebug() << "idx:" << treeWidget->topLevelItemCount();
        delete item; // 清空树形控件中的项目
    }

    for(auto it = projectNode.begin(); it != projectNode.end(); ++it) {
        QString element = *it;
        // qDebug() << "node:" <<element;

        QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(treeWidget);
        topLevelItem->setText(0, QFileInfo(element).fileName());
        topLevelItem->setData(0, Qt::UserRole, element);
        QQueue<QPair<QString, QTreeWidgetItem*>> queue;
        queue.enqueue(QPair<QString, QTreeWidgetItem*>(element, topLevelItem));
        while (!queue.isEmpty()) {
            QPair<QString, QTreeWidgetItem*> current = queue.dequeue();
            QString currentPath = current.first;
            QTreeWidgetItem *parentItem = current.second;
            QDir dir(currentPath);
            QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QFileInfo &fileInfo : fileInfoList) {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                item->setText(0, fileInfo.fileName());
                item->setData(0, Qt::UserRole, fileInfo.filePath());
                if (fileInfo.isDir()) {
                    queue.enqueue(QPair<QString, QTreeWidgetItem*>(fileInfo.filePath(), item));
                }
            }
        }

    }

    treeWidget->expandAll();
}

void ProjectNavigator::clickedFile(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    MainWindow::instance()->createEditorTab(path);
    // QFile file(path);
    // if(QFileInfo(file).isFile()) {
    //     // emit sendFilePath(path);
    // }
}

void ProjectNavigator::showContextMenu(const QPoint &pos) {
    QMenu contextMenu;
    QAction closeProject("Close Project");
    QAction addSources("Add Sources");
    QAction addConstraints("Add Constrains");
    QAction deleteFileAction("Delete File");
    if (treeWidget->currentItem() == nullptr) {
        qDebug() << "No projects";
        return;
    }
    QString path = treeWidget->currentItem()->data(0, Qt::UserRole).toString();
    // qDebug() << "path: " << path;
    QFile file(path);
    if (projectNode.contains(path)) {
        contextMenu.addAction(&closeProject);
        connect(&closeProject, &QAction::triggered, this, &ProjectNavigator::closeProjectAction);
        contextMenu.addAction(&addSources);
        connect(&addSources, &QAction::triggered, this, &ProjectNavigator::addSourcesAction);
        contextMenu.addAction(&addConstraints);
        connect(&addConstraints, &QAction::triggered, this, &ProjectNavigator::addConstraintsAction);
    } else if (QFileInfo(file).isFile()) {
        contextMenu.addAction(&deleteFileAction);
        connect(&deleteFileAction, &QAction::triggered, this, &ProjectNavigator::deleteFileAction);

    }
    contextMenu.exec(treeWidget->mapToGlobal(pos));
}

void ProjectNavigator::closeProjectAction()
{
    projectNode.remove(treeWidget->currentItem()->data(0, Qt::UserRole).toString());
    delete treeWidget->currentItem();
}

void ProjectNavigator::addSourcesAction()
{
    QString path = treeWidget->currentItem()->data(0, Qt::UserRole).toString();
    QString addSourcesPath = path + "/sources/";
    qDebug() << "addSources path:" << addSourcesPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, addSourcesPath + QFileInfo(file).fileName());
        }
    }
    updateItems(path);
}

void ProjectNavigator::addConstraintsAction()
{
    QString path = treeWidget->currentItem()->data(0, Qt::UserRole).toString();
    QString constrainsPath = path + "/constrains/";
    qDebug() << "constrains path:" << constrainsPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, constrainsPath + QFileInfo(file).fileName());
        }
    }
    updateItems(path);
}

void ProjectNavigator::deleteFileAction()
{
    QFile file(treeWidget->currentItem()->text(0));
    file.remove();
    delete treeWidget->currentItem();
    treeWidget->expandAll();
}

bool ProjectNavigator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == treeWidget->viewport())
    {
        //点击树的空白,取消选中
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                QModelIndex index = treeWidget->indexAt(mouseEvent->pos());
                if (!index.isValid())
                {
                    treeWidget->setCurrentIndex(QModelIndex());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}


