#include "SettingsDialog.h"
#include "utils/ProjectManager.h"
#include "base/Globals.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    treeWidget(new QTreeWidget),
    stackedWidget(new QStackedWidget)
{
    setWindowTitle("Settings");
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    // setWindowFlags(topWindow->windowFlags() | Qt::WindowStaysOnTopHint); // 将窗口置顶
    resize(640 * GlobalConfig::SCALE_FACTOR, 520 * GlobalConfig::SCALE_FACTOR);
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeWidget->setStyleSheet(".QTreeWidget { border:4px solid #DCDCDC; }");
    treeWidget->setHeaderLabel("Project Settings");
    QTreeWidgetItem *generalItem = new QTreeWidgetItem(QStringList() << "General");
    QTreeWidgetItem *textEditorItem = new QTreeWidgetItem(QStringList() << "Text Editor");
    QTreeWidgetItem *bitstreamItem = new QTreeWidgetItem(QStringList() << "Bitstream");
    QList<QTreeWidgetItem *> settingsItemslist;
    settingsItemslist.append(generalItem);
    settingsItemslist.append(textEditorItem);
    settingsItemslist.append(bitstreamItem);
    treeWidget->addTopLevelItems(settingsItemslist);

    //将控件添加到堆栈窗口中
    generalSettingsPage =  new GeneralSettingsPage;
    bitstreamSettingsPage =  new BitstreamSettingsPage;
    textEditorSettingsPage = new TextEditorSettingsPage;
    stackedWidget->addWidget(generalSettingsPage);
    stackedWidget->addWidget(textEditorSettingsPage);
    stackedWidget->addWidget(bitstreamSettingsPage);
    bitstreamSettingsPage->loadSettings(); // 初始化复选框状态

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(splitter);
    splitter->addWidget(treeWidget);
    splitter->addWidget(stackedWidget);

    hBoxLayout->setMargin(5);
    hBoxLayout->setSpacing(15);
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
    vLayout->addLayout(hBoxLayout);
    vLayout->addWidget(buttonBox);

    splitter->setStretchFactor(0, 9);
    splitter->setStretchFactor(1, 16);
}

void SettingsDialog::accept()
{
    generalSettingsPage->setDevicePart();
    generalSettingsPage->setTopModule();
    textEditorSettingsPage->setEncoding();
    bitstreamSettingsPage->applySettings();
    ProjectManager::instance().writeAndLoadProject();
    QDialog::accept();
}
