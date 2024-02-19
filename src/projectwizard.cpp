#include "projectwizard.h"

ProjectWizard::ProjectWizard(QWidget *parent) : QWizard(parent)
{
    addPage(new ProjectNamePage);
    addPage(new AddSourcesPage);
    addPage(new AddConstrainsPage);
    connect(this, &QWizard::accepted, this, &ProjectWizard::onFinish);

    resize(800, 600);

    setAttribute(Qt::WA_DeleteOnClose);
}

void ProjectWizard::onFinish()
{
    // 获取项目名称和路径
    QString projectName = field("projectName").toString();
    QString projectPath = field("projectPath").toString();

    // 创建项目文件夹
    QDir dir(projectPath);
    if (dir.mkdir(projectName)) {
        dir.cd(projectName);
        dir.mkdir("doc");
        dir.mkdir("ip");
        dir.mkdir("constrains");
        dir.mkdir("sources");
        qDebug() << "Folders created successfully";
    } else {
        qDebug() << "Failed to create folder";
        return;
    }

    // 复制文件列表中的文件到项目文件夹sources
    foreach (const QString &file, sourcesFilesList)
    {
        QFile::copy(file, projectPath + "/" + projectName + "/sources/" + QFileInfo(file).fileName());
    }

    foreach (const QString &file, constrainsFilesList)
    {
        QFile::copy(file, projectPath + "/" + projectName + "/constrains/" + QFileInfo(file).fileName());
    }

    emit wizardAccepted(projectPath + '/' + projectName);
}

ProjectNamePage::ProjectNamePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Project Name");
    setSubTitle("Enter a name for your project and specify a directory "
                "where the project data files will be stored.");

    QLabel *nameLabel = new QLabel("Project name:");
    QLineEdit *nameLineEdit = new QLineEdit;
    registerField("projectName*", nameLineEdit);

    QLabel *pathLabel = new QLabel("Project path:");
    QLineEdit *pathLineEdit = new QLineEdit;
    QPushButton *browseButton = new QPushButton("Browse");
    connect(browseButton, &QPushButton::clicked, [=](){
        QString path = QFileDialog::getExistingDirectory(this, "Select Directory");
        if (!path.isEmpty())
        {
            pathLineEdit->setText(path);
        }
    });
    registerField("projectPath*", pathLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(pathLabel, 1, 0);
    layout->addWidget(pathLineEdit, 1, 1);
    layout->addWidget(browseButton, 1, 2);
    setLayout(layout);
}

AddSourcesPage::AddSourcesPage(QWidget *parent) : QWizardPage(parent)
{
    connect(this, &AddSourcesPage::filesListUpdated, this, &AddSourcesPage::updateFilesList);

    setTitle("Add Sources");
    setSubTitle("Specify HDL, netlist, Block Design, and iP files, or "
                "directories containing those files, to add to your project.");

    filesListWidget = new QListWidget;
    filesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 将文件列表注册为QStringList以便与QWizard的字段交互
    registerField("addsourcesFilesList", filesListWidget, "selectedItems", SIGNAL(itemSelectionChanged()));

    QPushButton *addFilesButton = new QPushButton("Add Files");
    connect(addFilesButton, &QPushButton::clicked, this, &AddSourcesPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");
    connect(removeButton, &QPushButton::clicked, this, &AddSourcesPage::onRemoveFiles);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(filesListWidget);
    layout->addWidget(addFilesButton);
    layout->addWidget(removeButton);
    setLayout(layout);
}

void AddSourcesPage::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty())
    {
        ProjectWizard* wizard = qobject_cast<ProjectWizard*>(this->wizard());
        wizard->sourcesFilesList.append(files);  // 将选择的文件追加到列表中
        for (int i = 0; i < wizard->sourcesFilesList.size(); ++i) {
            qDebug() << wizard->sourcesFilesList.at(i);
        }
        emit filesListUpdated(files); // 发送信号
    }
}

void AddSourcesPage::onRemoveFiles()
{
    QList<QListWidgetItem*> selectedItems = filesListWidget->selectedItems();
    for (QListWidgetItem* item : selectedItems)
    {
        int row = filesListWidget->row(item);
        filesListWidget->takeItem(row);
        // 从sourcesFilesList中移除对应项
        ProjectWizard* wizard = qobject_cast<ProjectWizard*>(this->wizard());
        wizard->sourcesFilesList.removeAt(row);
    }
}

void AddSourcesPage::updateFilesList(const QStringList &files)
{
    filesListWidget->addItems(files);
}

AddConstrainsPage::AddConstrainsPage(QWidget *parent) : QWizardPage(parent)
{
    connect(this, &AddConstrainsPage::filesListUpdated, this, &AddConstrainsPage::updateFilesList);

    setTitle("Add Constrains");
    setSubTitle("Specify or create constraint files for physical and "
                "timing constrains.");

    filesListWidget = new QListWidget;
    filesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 将文件列表注册为QStringList以便与QWizard的字段交互
    registerField("addconstrainsFilesList", filesListWidget, "selectedItems", SIGNAL(itemSelectionChanged()));

    QPushButton *addFilesButton = new QPushButton("Add Files");
    connect(addFilesButton, &QPushButton::clicked, this, &AddConstrainsPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");
    connect(removeButton, &QPushButton::clicked, this, &AddConstrainsPage::onRemoveFiles);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(filesListWidget);
    layout->addWidget(addFilesButton);
    layout->addWidget(removeButton);
    setLayout(layout);
}

void AddConstrainsPage::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty())
    {
        ProjectWizard* wizard = qobject_cast<ProjectWizard*>(this->wizard());
        wizard->constrainsFilesList.append(files);  // 将选择的文件追加到列表中
        for (int i = 0; i < wizard->constrainsFilesList.size(); ++i) {
            qDebug() << wizard->constrainsFilesList.at(i);
        }
        emit filesListUpdated(files); // 发送信号
    }
}

void AddConstrainsPage::onRemoveFiles()
{
    QList<QListWidgetItem*> selectedItems = filesListWidget->selectedItems();
    for (QListWidgetItem* item : selectedItems)
    {
        int row = filesListWidget->row(item);
        filesListWidget->takeItem(row);
        // 从constrainsFilesList中移除对应项
        ProjectWizard* wizard = qobject_cast<ProjectWizard*>(this->wizard());
        wizard->constrainsFilesList.removeAt(row);
    }
}

void AddConstrainsPage::updateFilesList(const QStringList &files)
{
    filesListWidget->addItems(files);
}
