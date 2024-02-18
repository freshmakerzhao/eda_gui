#include "projectnavigator.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QQueue>
#include <QPair>

ProjectNavigator::ProjectNavigator(QWidget *parent)
    : QWidget(parent)
{
    treeWidget = new QTreeWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(treeWidget);
    layout->setMargin(0);

    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &ProjectNavigator::clickedFile);


    QObject::connect(treeWidget, &QTreeWidget::customContextMenuRequested, [&](const QPoint &pos){
        QMenu contextMenu;
        QAction addFileAction("Add File");
        QAction deleteFileAction("Delete File");
        if (treeWidget->currentItem() != nullptr) {
            QString path = treeWidget->currentItem()->data(0, Qt::UserRole).toString();
            QFile tmp(path);
            if (QFileInfo(tmp).isDir() || treeWidget->currentItem() == rootItem) {
                contextMenu.addAction(&addFileAction);
                QObject::connect(&addFileAction, &QAction::triggered, [&](){
                    QString filePath = QFileDialog::getOpenFileName(nullptr, "Add File", "", "All Files (*.*)");
                    QFile file(filePath);
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    file.close();

                    QTreeWidgetItem *newItem = new QTreeWidgetItem(rootItem);
                    newItem->setText(0, QFileInfo(file).fileName());
                    newItem->setData(0, Qt::UserRole, file.fileName());
                });
            } else {
                contextMenu.addAction(&deleteFileAction);
                QObject::connect(&deleteFileAction, &QAction::triggered, [&](){
                    if(treeWidget->currentItem() != rootItem){
                        QFile file(treeWidget->currentItem()->text(0));
                        file.remove();
                        delete treeWidget->currentItem();
                    }

                });
            }
        } else {
            qDebug() << "Empty";
        }

        treeWidget->expandAll();
        contextMenu.exec(treeWidget->mapToGlobal(pos));
    });

    treeWidget->setColumnCount(1);
    treeWidget->setHeaderHidden(true);
    treeWidget->expandAll();
}

ProjectNavigator::~ProjectNavigator()
{

}

void ProjectNavigator::refreshItems(const QString &path)
{
    QFile file(path);
    QQueue<QPair<QString, QTreeWidgetItem*>> queue;
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(treeWidget);
    rootItem = topLevelItem;
    topLevelItem->setText(0, QFileInfo(file).fileName());
    queue.enqueue(QPair<QString, QTreeWidgetItem*>(path, topLevelItem));

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

    treeWidget->expandAll();
}

void ProjectNavigator::clickedFile(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    QFile file(path);
    if(QFileInfo(file).isFile()) {
        emit sendFilePath(path);
    }

}


