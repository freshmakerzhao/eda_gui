#ifndef PROJECTNAVIGATOR_H
#define PROJECTNAVIGATOR_H

#include <QWidget>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMouseEvent>
#include <QGridLayout>
#include <QSet>
#include <QVector>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QQueue>
#include <QPair>
#include <QProcess>

#include "utils/Project.h"

class Navigator : public QWidget
{
    Q_OBJECT

public:
    static Navigator *instance(QWidget *parent = nullptr);

    void loadFile(Project *proj);

public slots:

    void clickedFile(QTreeWidgetItem *item);

    void showContextMenu(const QPoint &pos);

    void closeProjectAction();

    void addSourcesAction();

    void addConstraintsAction();

    void removeFileAction();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Navigator(QWidget *parent = nullptr);
    ~Navigator();

    QTreeWidget *navTree;

    QTreeWidgetItem *sourceItem;
    QTreeWidgetItem *constraintsItem;

    Project *p = nullptr;
};

#endif // PROJECTNAVIGATOR_H
