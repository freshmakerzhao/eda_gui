#ifndef IOPORTSTREEWIDGET_H
#define IOPORTSTREEWIDGET_H

#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QWidget>

class IOportsTreeWidget : public QWidget {
public:
    IOportsTreeWidget(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QTreeWidget *treeWidget = new QTreeWidget();
        treeWidget->setColumnCount(14);
        treeWidget->setHeaderLabels(QStringList() << "Name" << "Direction" << "Neg Diff Pair" << "Package Pin"
                                                  << "Fixed" << "Bank" << "I/O Std" << "Vcco"
                                                  << "Vref" << "Drive Strength" << "Slew Type"
                                                  << "Pull Type" << "Off-Chip Termination" << "IN_TERM");

        QTreeWidgetItem *item1 = new QTreeWidgetItem(treeWidget, QStringList("All ports"));
        QTreeWidgetItem *item2 = new QTreeWidgetItem(item1, QStringList("Scalar ports"));

        // 添加数据
        for (int i = 0; i < 5; ++i) {
            QTreeWidgetItem *item = new QTreeWidgetItem(item2);
            // item->setText(0, QString("Item %1").arg(i + 1));
            // treeWidget->addTopLevelItem(item);

            // 在第5列插入QCheckBox
            QCheckBox *checkBox = new QCheckBox();
            QWidget *subWidget = new QWidget;
            QHBoxLayout *subLayout = new QHBoxLayout(subWidget);
            subLayout->setAlignment(Qt::AlignCenter);
            subLayout->setMargin(0);
            subLayout->addWidget(checkBox);
            treeWidget->setItemWidget(item, 4, subWidget); // 第5列为索引4

            // 在其它列插入QComboBox
            for (int col : {3}) { // 注意列索引是从0开始的
                QComboBox *comboBox = new QComboBox();
                comboBox->addItems(QStringList() << "K17" << "J17" << "L14");
                treeWidget->setItemWidget(item, col, comboBox);
            }

            for (int col : {6, 9, 10, 11, 12, 13, 14}) { // 注意列索引是从0开始的
                QComboBox *comboBox = new QComboBox();
                comboBox->addItems(QStringList() << "LVCMOS15" << "LVCMOS18" << "LVCMOS25" << "LVCMOS33");
                treeWidget->setItemWidget(item, col, comboBox);
            }
        }

        layout->addWidget(treeWidget);
        setLayout(layout);

        treeWidget->expandAll();
    }
};

#endif // IOPORTSTREEWIDGET_H
