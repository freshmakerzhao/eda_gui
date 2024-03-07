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

#include "project.h"

class Navigator : public QWidget
{
    Q_OBJECT

public:
    static Navigator *instance(QWidget *parent = nullptr);

    explicit Navigator(QWidget *parent = nullptr);
    ~Navigator();

    void loadFile(Project *project);

public slots:
    // void updateItems(const QString &path);


    void clickedFile(QTreeWidgetItem *item);

    void showContextMenu(const QPoint &pos);

    void closeProjectAction();

    void setActiveProjectAction();

    void addSourcesAction();

    void addConstraintsAction();

    void deleteFileAction();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTreeWidget *navTree;

    // QSet<QString> projectNode;

    QVector<QString> pn;
};

#endif // PROJECTNAVIGATOR_H
