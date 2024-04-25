#include "FileManager.h"
#include "mainwindow.h"
#include "utils/ProjectManager.h"

FileManager *FileManager::instance(QWidget *parent)
{
    static FileManager *_instance = nullptr;
    if (!_instance) {
        _instance = new FileManager(parent);
    }
    return _instance;
}

void FileManager::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(this);
    // 获取选中项的索引
    QModelIndex currentIndex = indexAt(pos);

    QAction addSourcesAction("Add Sources...", this);
    // addSourcesAction.setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    QAction openFileAction("Open File", this);
    // openFileAction.setShortcut(QKeySequence(Qt::ALT + Qt::Key_O));
    QAction removeFromAction("Remove File form Project...", this);
    // removeFromAction.setShortcut(Qt::Key_Delete);
    // QAction renameAction("Rename...", this);

    // 判断是否为1级文件夹
    if (currentIndex.isValid() && currentIndex.parent().isValid()) {
        // 不是1级文件夹，添加动作
        // connect(&renameAction, &QAction::triggered, this, &FileManager::renameSelectedItem);
        connect(&openFileAction, &QAction::triggered, this, &FileManager::openFileAction);
        contextMenu.addAction(&openFileAction);
        // contextMenu.addAction(&renameAction);
        connect(&removeFromAction, &QAction::triggered, this, &FileManager::removeFileAction);
        contextMenu.addAction(&removeFromAction);

    } else {
        connect(&addSourcesAction, &QAction::triggered, this, &FileManager::addSourcesAction);
        contextMenu.addAction(&addSourcesAction);
    }

    contextMenu.exec(mapToGlobal(pos));
}

void FileManager::updateDesignSources(QStringList &list)
{
    designsources->removeRows(0, designsources->rowCount());
    foreach (const QString &file, list) {
        QStandardItem* node = new QStandardItem(QFileInfo(file).fileName());
        node->setData(QFileInfo(file).filePath(), Qt::UserRole); // 设置Qt::UserRole数据
        designsources->appendRow(node);
    }
}

void FileManager::updateConstraints(QStringList &list)
{
    constraints->removeRows(0, constraints->rowCount());
    foreach (const QString &file, list) {
        QStandardItem* node = new QStandardItem(QFileInfo(file).fileName());
        node->setData(QFileInfo(file).filePath(), Qt::UserRole); // 设置Qt::UserRole数据
        constraints->appendRow(node);
    }
}

void FileManager::clickedFile(const QModelIndex& index)
{
    QString path = index.data(Qt::UserRole).toString();
    MainWindow::instance()->createEditorTab(path);
}

void FileManager::openFileAction()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    foreach (const QModelIndex& index, selectedIndexes) {
        QString path = index.data(Qt::UserRole).toString(); // 获取项的Qt::UserRole数据
        // 在此处使用数据进行打开文件操作
        MainWindow::instance()->createEditorTab(path);
    }
}

void FileManager::addSourcesAction()
{
    ProjectManager::instance().addSourcesAction();
}

void FileManager::removeFileAction()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    foreach (const QModelIndex& index, selectedIndexes) {
        if (index.isValid()) {
            QString path = index.data(Qt::UserRole).toString(); // 获取项的Qt::UserRole数据
            if (ProjectManager::instance().removeFileAction(path)) {
                // 获取父节点的模型索引
                QModelIndex parentIndex = index.parent();
                // 从模型中移除选中的子节点
                model()->removeRow(index.row(), parentIndex);
            }
        }
    }
}

void FileManager::closeProject()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    foreach (const QModelIndex& index, selectedIndexes) {
        if (index.isValid()) {
            QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
            if (model) {
                QStandardItem* item = model->itemFromIndex(index);
                if (item) {
                    // 删除所有子节点
                    item->removeRows(0, item->rowCount());
                }
            }
        }
    }
}

// void FileManager::keyPressEvent(QKeyEvent *event)
// {
//     if (event->key() == Qt::Key_F2 && state() != QTreeView::EditingState) {
//         QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
//         if (model)
//         {
//             QItemSelectionModel* selectionModel = this->selectionModel();
//             QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
//             if (!selectedIndexes.isEmpty())
//             {
//                 edit(selectedIndexes.first());
//             }
//         }
//     } else {
//         QTreeView::keyPressEvent(event);
//     }
// }

// void FileManager::renameSelectedItem()
// {
//     QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
//     if (model)
//     {
//         QItemSelectionModel* selectionModel = this->selectionModel();
//         QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
//         if (!selectedIndexes.isEmpty())
//         {
//             edit(selectedIndexes.first());
//         }
//     }
// }


FileManager::FileManager(QWidget *parent) : QTreeView(parent)
{
    setStyleSheet("QTreeView::item { height: 32px; }");
    setHeaderHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &FileManager::showContextMenu);
    connect(this, &QTreeView::doubleClicked, this, &FileManager::clickedFile);


    QStandardItemModel* model = new QStandardItemModel(this);
    setModel(model);
    designsources = new QStandardItem("Design Sources");
    constraints = new QStandardItem("Constraints");
    model->setItem(0, 0, designsources);
    model->setItem(1, 0, constraints);

    expandAll();
}
