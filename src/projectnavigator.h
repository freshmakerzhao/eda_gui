#ifndef PROJECTNAVIGATOR_H
#define PROJECTNAVIGATOR_H

#include <QWidget>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMouseEvent>
#include <QGridLayout>
#include <QSet>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QQueue>
#include <QPair>

#include <mainwindow.h>

class ProjectNavigator : public QWidget
{
    Q_OBJECT

public:
    static ProjectNavigator *instance(QWidget *parent = nullptr);

    explicit ProjectNavigator(QWidget *parent = nullptr);
    ~ProjectNavigator();

public slots:
    void updateItems(const QString &path);

    void clickedFile(QTreeWidgetItem *item);

    void showContextMenu(const QPoint &pos);

    void closeProjectAction();

    void addSourcesAction();

    void addConstraintsAction();

    void deleteFileAction();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTreeWidget *treeWidget;

    QSet<QString> projectNode;

// signals:
//     void sendFilePath(const QString &path);


};

#endif // PROJECTNAVIGATOR_H
