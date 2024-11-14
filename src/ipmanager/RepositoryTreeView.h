#ifndef REPOSITORYTREEVIEW_H
#define REPOSITORYTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QDir>
#include <QMessageBox>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

struct IPInfo {
    QString displayName;
    QString vlnv;
    QString interfaces;
    QString description;
};

class RepositoryTreeView : public QTreeView
{
    Q_OBJECT
public:
    RepositoryTreeView(QWidget *parent = nullptr);

    void initRepository();
private:
    void createTreePath(const QString &path,
                        QStandardItem *rootItem,
                        const IPInfo &ipInfo);

    void parseIPNode(const QDomElement &ipElement, QStandardItem *rootItem);

    QStandardItemModel *model;
};

#endif // REPOSITORYTREEVIEW_H
