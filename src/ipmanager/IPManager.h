#ifndef IPMANAGER_H
#define IPMANAGER_H

#include <QLineEdit>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QSplitter>
#include <QLabel>
#include "RepositoryTreeView.h"

class IPManager : public QWidget
{
    Q_OBJECT
public:
    static IPManager *instance();

private:
    IPManager(QWidget* parent = nullptr);

    void init();

public slots:
    void doubleClickedIP(const QModelIndex& index);

    void clickedIP(const QModelIndex& index);
private:
    void setDetails(const QString &details = QString());

    QLabel *detailLabel;

    RepositoryTreeView *repositoryTreeView;
};

#endif // IPMANAGER_H
