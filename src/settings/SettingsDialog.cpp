#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Settings");
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    setFixedSize(1000, 800);
    // 创建一个QTreeWidget控件 并向其中添加列表项
    treeWidget = new QTreeWidget(this);
    treeWidget->setFixedWidth(300);
    treeWidget->setHeaderLabel("Project Settings");
    QTreeWidgetItem *generalitem = new QTreeWidgetItem;
    generalitem->setText(0, "General");
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

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4f7cce; "   // 背景色
        "   color: white; "                // 白色文字
        "   font-size: 10pt;"              // 字号
        "   border-width: 2px; "           // 边框宽度
        "   border-color: #4f7cce; "       // 边框颜色
        "   border-style: solid; "         // 边框样式
        "   min-width: 60px; "             // 最小宽度
        "   padding: 6px; "                // 内边距
        "   margin-right: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3a5b98; "   // 背景色
        "   color: white; "                // 白色文字
        "   font-size: 10pt;"              // 字号
        "   border-width: 2px; "           // 边框宽度
        "   border-color: #4f7cce; "       // 边框颜色
        "   border-style: solid; "         // 边框样式
        "   min-width: 60px; "             // 最小宽度
        "   padding: 4px;"                // 内边距
        "   margin: 2px;"
        "   margin-right: 10px;"
        "}");
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff; "   // 背景色
        "   color: black; "                // 白色文字
        "   font-size: 10pt;"              // 字号
        "   border-width: 1px; "           // 边框宽度
        "   border-color: black; "       // 边框颜色
        "   border-style: solid; "         // 边框样式
        "   width: 60px; "                 // 宽度
        "   height: 20px; "                 // 高度
        "   padding: 6px; "                // 内边距
        "   margin-right: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ededed; "   // 背景色
        "   color: black; "                // 白色文字
        "   font-size: 10pt;"              // 字号
        "   border-width: 1px; "           // 边框宽度
        "   border-color: black; "       // 边框颜色
        "   border-style: solid; "         // 边框样式
        "   width: 60px; "                 // 宽度
        "   height: 20px; "                 // 高度
        "   padding: 4px;"                // 内边距
        "   margin: 2px;"
        "   margin-right: 10px;"
        "}");

    connect(buttonBox, &QDialogButtonBox::accepted, [this](){
        generalPage->setTopModule();
        this->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, [this](){
        this->accept();
    });

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(buttonBox);

}

// int SettingsDialog::exec()
// {
//     QDialog::exec();
//     return 0;
// }
