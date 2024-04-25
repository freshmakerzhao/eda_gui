#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
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

    void updateDesignSources(QStringList &list);

    void updateConstraints(QStringList &list);

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
