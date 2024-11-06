#ifndef REPOSITORYTREEVIEW_H
#define REPOSITORYTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QDir>
#include <QMessageBox>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class RepositoryTreeView : public QTreeView
{
    Q_OBJECT
public:
    RepositoryTreeView(QWidget *parent = nullptr);

    void initRepository();
private:
    void createTreePath(const QString &path,
                        QStandardItem *rootItem,
                        const QString &displayName,
                        const QString &vlnv,
                        const QString &interfaces);

    void parseIPNode(const QDomElement &ipElement, QStandardItem *rootItem);

    QStandardItemModel *model;
};

#endif // REPOSITORYTREEVIEW_H
