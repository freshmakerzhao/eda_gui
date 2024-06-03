#ifndef IPMANAGER_H
#define IPMANAGER_H

#include <QLineEdit>
#include <QPixmap>
#include <QHBoxLayout>
#include <QDebug>
#include <QLineEdit>
#include <QFormLayout>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSplitter>

class IPManager : public QWidget
{
    Q_OBJECT
public:
    static IPManager *instance();

private:
    IPManager(QWidget* parent = nullptr);

    void init();

    QTreeView *treeView;

    QStandardItemModel *model;
};

#endif // IPMANAGER_H
