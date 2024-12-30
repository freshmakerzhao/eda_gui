/**
  ******************************************************************************
  * @file           : FileManager.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/25
  ******************************************************************************
  */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "qfilesystemwatcher.h"
#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QFileInfo>
#include <QDir>

class FileManager : public QTreeView
{
    Q_OBJECT

public:
    static FileManager *instance(QWidget *parent = nullptr);

protected:
    // void keyPressEvent(QKeyEvent* event) override;

public slots:
    void showContextMenu(const QPoint& pos);

    /**
     * 更新DesignSources
     * @param list DesignSources文件路径列表
     */
    void updateDesignSources(const QStringList &list);

    /**
     * 更新Constraints
     * @param list Constraints文件路径列表
     */
    void updateConstraints(const QStringList &list);

    /**
     * @brief 更新simulation 文件
     * @param list simulation 文件路径列表
     */
    void updateSimSources(const QStringList &list);

    void clickedFile(const QModelIndex& index);

    void openFileAction();

    void addSourcesAction();

    /**
     * 移除选中节点，并从工程文件列表移除该文件
     */
    void removeFileAction();

    /**
     * 清除文件树节点
     */
    void cleanFileItems();

    // void renameSelectedItem();

    void updateIPList();

private:
    FileManager(QWidget* parent = nullptr);

    ~FileManager();

    QStandardItemModel *model;

    QStandardItem *designsources;

    QStandardItem *constraints;

    QStandardItem *simulationSource;

    QStandardItem *ipLists;

    QFileSystemWatcher *fileWatcher;

};

#endif // FILEMANAGER_H
