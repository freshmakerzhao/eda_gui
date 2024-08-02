#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Settings");
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    // setWindowFlags(topWindow->windowFlags() | Qt::WindowStaysOnTopHint); // 将窗口置顶
    setFixedSize(1000, 800);
    // 创建一个QTreeWidget控件 并向其中添加列表项
    treeWidget = new QTreeWidget(this);
    treeWidget->setFixedWidth(300);
    treeWidget->setHeaderLabel("Project Settings");
    QTreeWidgetItem *generalitem = new QTreeWidgetItem(QStringList() << "General");
    QList<QTreeWidgetItem *> settingsItemslist;
    settingsItemslist.append(generalitem);
    treeWidget->addTopLevelItems(settingsItemslist);

    //创建QStackedwidget控件
    stackedWidget = new QStackedWidget(this);
    //将控件添加到堆栈窗口中
    generalPage =  new GeneralPage(this);
    stackedWidget->addWidget(generalPage);

    // 使用一个水平布局管理器对对话框进行布局
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(treeWidget);
    hLayout->addWidget(stackedWidget);
    // 设置mainLayout的边框与对话框边缘的距离
    hLayout->setMargin(5);
    hLayout->setSpacing(15);
    // 信号与槽的连接，实现按选择显示窗体
    connect(treeWidget, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item) {
        int index = treeWidget->indexOfTopLevelItem(item);
        if (index >= 0) {
            stackedWidget->setCurrentIndex(index);
        }
    });

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(buttonBox);

}

void SettingsDialog::accept()
{
    generalPage->setDevicePart();
    generalPage->setTopModule();
    QDialog::accept();
}
