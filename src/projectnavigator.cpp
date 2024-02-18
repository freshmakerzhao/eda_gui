#include "projectnavigator.h"
#include "ui_projectnavigator.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QQueue>
#include <QPair>

ProjectNavigator::ProjectNavigator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectNavigator)
{
    ui->setupUi(this);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &ProjectNavigator::clickedFile);


    QObject::connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, [&](const QPoint &pos){
        QMenu contextMenu;
        QAction addFileAction("Add File");
        QAction deleteFileAction("Delete File");
        QString path = ui->treeWidget->currentItem()->data(0, Qt::UserRole).toString();
        QFile tmp(path);
        if (QFileInfo(tmp).isDir() || ui->treeWidget->currentItem() == rootItem) {
            contextMenu.addAction(&addFileAction);
            QObject::connect(&addFileAction, &QAction::triggered, [&](){
                QString filePath = QFileDialog::getOpenFileName(nullptr, "Add File", "", "All Files (*.*)");
                QFile file(filePath);
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                file.close();

                QTreeWidgetItem *newItem = new QTreeWidgetItem(rootItem);
                newItem->setText(0, QFileInfo(file).fileName());
                newItem->setData(0, Qt::UserRole, file.fileName());
                ui->treeWidget->expandAll();
            });
        } else {
            contextMenu.addAction(&deleteFileAction);
            QObject::connect(&deleteFileAction, &QAction::triggered, [&](){
                if(ui->treeWidget->currentItem() != rootItem){
                    QFile file(ui->treeWidget->currentItem()->text(0));
                    file.remove();
                    delete ui->treeWidget->currentItem();
                }
                ui->treeWidget->expandAll();
            });
        }
        contextMenu.exec(ui->treeWidget->mapToGlobal(pos));
    });

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->expandAll();
}

ProjectNavigator::~ProjectNavigator()
{
    delete ui;
}

void ProjectNavigator::refreshItems(const QString &path)
{
    QFile file(path);
    QQueue<QPair<QString, QTreeWidgetItem*>> queue;
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui->treeWidget);
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

    ui->treeWidget->expandAll();
}

void ProjectNavigator::clickedFile(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    QFile file(path);
    if(QFileInfo(file).isFile()) {
        emit sendFilePath(path);
    }

}


