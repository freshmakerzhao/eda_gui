#ifndef PROJECTNAVIGATOR_H
#define PROJECTNAVIGATOR_H

#include <QWidget>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMouseEvent>
#include <QGridLayout>


class ProjectNavigator : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectNavigator(QWidget *parent = nullptr);
    ~ProjectNavigator();

public slots:
    void refreshItems(const QString &path);

    void clickedFile(QTreeWidgetItem *item);
private:
    QTreeWidget *treeWidget;

    QTreeWidgetItem *rootItem;

signals:
    void sendFilePath(const QString &path);


};

#endif // PROJECTNAVIGATOR_H
