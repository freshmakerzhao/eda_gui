#include "RepositoryTreeView.h"
#include "base/Globals.h"

RepositoryTreeView::RepositoryTreeView(QWidget *parent)
{
    model = new QStandardItemModel(this);
    QStringList headers = {"Name",
        "AXI4",
        "Status",
        "License",
        "VLNV"
    };
    model->setHorizontalHeaderLabels(headers);

    setModel(model);
    setColumnWidth(0, 360);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    // setAlternatingRowColors(free); // 交替显示
}

void RepositoryTreeView::initRepository()
{
    QString vv_index = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("ipcore/vv_index.xml");
    QFile file(vv_index);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open " + vv_index);
        return;
    }
    // 解析 XML 文件
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Error", "Failed to parse XML");
        return;
    }
    file.close();

    // 获取根元素并解析
    QStandardItem *rootNode = new QStandardItem("HybrdLink Repository");
    model->appendRow(rootNode);

    QDomElement root = doc.documentElement();
    QDomNodeList ipNodes = root.elementsByTagName("IP");
    if (ipNodes.isEmpty()) {
        QMessageBox::warning(this, "Warning", "IP Repository is Empty");
        return;
    }
    for (int i = 0; i < ipNodes.count(); ++i) {
        QDomElement ipElement = ipNodes.at(i).toElement();
        parseIPNode(ipElement, rootNode);
    }
}

void RepositoryTreeView::createTreePath(const QString &path,
                                        QStandardItem *rootItem,
                                        const QString &displayName,
                                        const QString &vlnv,
                                        const QString &interfaces)
{
    QStringList pathParts = path.split("/", Qt::SkipEmptyParts);
    QStandardItem *currentItem = rootItem;

    // 遍历路径，逐层创建树节点
    for (QString part : pathParts) {
        part.replace('_', ' '); // 需替换文本中的下划线

        bool found = false;
        for (int i = 0; i < currentItem->rowCount(); ++i) {
            QStandardItem *child = currentItem->child(i);
            if (child->text() == part) {
                currentItem = child;
                found = true;
                break;
            }
        }

        if (!found) {
            QStandardItem *newItem = new QStandardItem(part);
            currentItem->appendRow(newItem);
            currentItem = newItem;
        }
    }

    // 在路径末尾添加 DisplayName 作为叶子节点
    QStandardItem *displayNameItem = new QStandardItem(displayName);
    QList<QStandardItem *> rowItems;
    rowItems.append(displayNameItem);
    rowItems.append(new QStandardItem(interfaces));
    rowItems.append(new QStandardItem(QString("Production")));
    rowItems.append(new QStandardItem(QString("Included")));
    rowItems.append(new QStandardItem(vlnv));
    currentItem->appendRow(rowItems);
    displayNameItem->setData(displayName, Qt::UserRole);
}

void RepositoryTreeView::parseIPNode(const QDomElement &ipElement, QStandardItem *rootItem)
{
    // 获取 DisplayName 属性
    QDomElement displayNameElement = ipElement.firstChildElement("DisplayName");
    QString displayName = displayNameElement.attribute("value");

    // 获取 VLNV 属性
    QDomElement vlnvElement = ipElement.firstChildElement("VLNV");
    QString vlnv = vlnvElement.attribute("value");

    // 获取 Interfaces 属性
    QDomElement interfacesElement = ipElement.firstChildElement("Interfaces");
    QString interfaces;
    if (!interfacesElement.isNull()) {
        QDomElement interfaceElement = interfacesElement.firstChildElement("Interface");
        if (!interfaceElement.isNull()) {
            interfaces = interfaceElement.attribute("value");
        }
    }

    // 获取 Taxonomy 路径
    QDomElement taxonomiesElement = ipElement.firstChildElement("Taxonomies");
    QDomElement taxonomyElement = taxonomiesElement.firstChildElement("Taxonomy");
    if (!taxonomyElement.isNull()) {
        QString taxonomyPath = taxonomyElement.attribute("value");
        createTreePath(taxonomyPath, rootItem, displayName, vlnv, interfaces);
    }
}
