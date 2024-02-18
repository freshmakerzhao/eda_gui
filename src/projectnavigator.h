#ifndef PROJECTNAVIGATOR_H
#define PROJECTNAVIGATOR_H

#include <QWidget>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMouseEvent>

namespace Ui {
class ProjectNavigator;
}

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

    Ui::ProjectNavigator *ui;

    QTreeWidgetItem *rootItem;

signals:
    void sendFilePath(const QString &path);


};

#endif // PROJECTNAVIGATOR_H
