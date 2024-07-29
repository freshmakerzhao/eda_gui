/**
  ******************************************************************************
  * @file           : FileManager.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/25
  ******************************************************************************
  */

#include "FileManager.h"
#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "dialog/RemoveFileDialog.h"
#include <QLineEdit>

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

    QAction addSourcesAction(QIcon(":icons/resource/icons/12-1icon_add.png"),"Add Sources...", this);
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

/**
 * 更新DesignSources
 * @param list DesignSources文件路径列表
 */
void FileManager::updateDesignSources(const QStringList &list)
{
    designsources->removeRows(0, designsources->rowCount());
    foreach (const QString &file, list) {
        QStandardItem* node = new QStandardItem(QIcon(":/icons/resource/icons/38-1icon_source_file.png"),QFileInfo(file).fileName());
        node->setData(QFileInfo(file).filePath(), Qt::UserRole); // 设置Qt::UserRole数据
        designsources->appendRow(node);
    }
}

/**
 * 更新Constraints
 * @param list Constraints文件路径列表
 */
void FileManager::updateConstraints(const QStringList &list)
{
    constraints->removeRows(0, constraints->rowCount());
    foreach (const QString &file, list) {
        QStandardItem* node = new QStandardItem(QIcon(":/icons/resource/icons/39-icon_constraints_file.png"),QFileInfo(file).fileName());
        node->setData(QFileInfo(file).filePath(), Qt::UserRole); // 设置Qt::UserRole数据
        constraints->appendRow(node);
    }
}

void FileManager::clickedFile(const QModelIndex& index)
{
    const QString path = index.data(Qt::UserRole).toString();
    MainWindow::instance()->createEditorTab(path);
}

void FileManager::openFileAction()
{
    const QModelIndex &index = selectionModel()->currentIndex();
    const QString path = index.data(Qt::UserRole).toString(); // 获取项的Qt::UserRole数据
    // 在此处使用数据进行打开文件操作
    MainWindow::instance()->createEditorTab(path);
}

void FileManager::addSourcesAction()
{
    ProjectManager::instance().addSourcesAction();
}

/**
 * 移除选中节点，并从工程文件列表移除该文件，(选择)删除该文件
 */
void FileManager::removeFileAction()
{
    // 获取当前右键指向的节点索引
    const QModelIndex &index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }
    // 获取项的Qt::UserRole数据
    const QString path = index.data(Qt::UserRole).toString();

    RemoveFileDialog dialog(MainWindow::instance(), path);
    const int result = dialog.exec();
    bool isRemove = false;
    switch (result) {
    case RemoveFileDialog::AcceptedUnchecked: // 仅移除
        isRemove = ProjectManager::instance().removeFileAction(path, false);
        break;
    case  RemoveFileDialog::AcceptedChecked: // 移除+删除
        isRemove = ProjectManager::instance().removeFileAction(path, true);
        break;
    default: // 忽略
        return;
    }

    if (isRemove) {
        // 获取父节点的模型索引
        QModelIndex parentIndex = index.parent();
        // 从模型中移除选中的子节点
        model->removeRow(index.row(), parentIndex);
    }
}

/**
 * 清除文件树节点
 */
void FileManager::cleanFileItems()
{
    designsources->removeRows(0, designsources->rowCount());
    constraints->removeRows(0, constraints->rowCount());
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
    qDebug() << "[FileManager] Constructing...";
    setStyleSheet("QTreeView::item { height: 32px; }");
    setHeaderHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &FileManager::showContextMenu);
    connect(this, &QTreeView::doubleClicked, this, &FileManager::clickedFile);


    model = new QStandardItemModel(this);
    setModel(model);
    designsources = new QStandardItem("Design Sources");
    constraints = new QStandardItem("Constraints");
    model->setItem(0, 0, designsources);
    model->setItem(1, 0, constraints);
}

FileManager::~FileManager()
{
    qDebug() << "[FileManager] Distructing...";
    model->removeRows(0, model->rowCount());
    // delete designsources;
    // delete constraints;
}

