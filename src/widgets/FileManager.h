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

#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QItemSelectionModel>
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

    void updateDesignSources(const QStringList &list);

    void updateConstraints(const QStringList &list);

    void clickedFile(const QModelIndex& index);

    void openFileAction();

    void addSourcesAction();

    void removeFileAction();

    void closeProject();

    // void renameSelectedItem();

private:
    FileManager(QWidget* parent = nullptr);

    QStandardItem *designsources;

    QStandardItem *constraints;

};

#endif // FILEMANAGER_H
