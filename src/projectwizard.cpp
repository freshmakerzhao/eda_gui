#include "projectwizard.h"

ProjectWizard::ProjectWizard(QWidget *parent) : QWizard(parent)
{
    setWizardStyle(QWizard::ModernStyle);
    addPage(new ProjectNamePage);
    addPage(new AddSourcesPage);
    addPage(new AddConstrainsPage);
    addPage(new DefaultPartPage);
    connect(this, &QWizard::accepted, this, &ProjectWizard::onFinish);

    resize(900, 600);

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

    qDebug() << "Device: " << device << ", Package: " << package; // 输出到控制台

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

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &AddSourcesPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &AddSourcesPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    layout1->addWidget(addFilesButton);
    layout1->addWidget(removeButton);
    layout->addLayout(layout1);
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

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &AddConstrainsPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &AddConstrainsPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    layout1->addWidget(addFilesButton);
    layout1->addWidget(removeButton);
    layout->addLayout(layout1);
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

DefaultPartPage::DefaultPartPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Default Part");
    setSubTitle("Choose a default part for your project.");

    // 读取parts.yaml文件
    QFile file(":/resource/parts.yaml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "parts.yaml load fail";
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.close();

    tableView = new QTableView(this);
    QStandardItemModel *model = new QStandardItemModel(0, 4, this);
    tableView->setModel(model);
    QStringList headers = {"part", "device", "package", "speedgrade"};
    model->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines.at(i);
        if (line.startsWith("xc7")) {
            QString part = line.section(':', 0, 0).trimmed();
            model->setItem(row, 0, new QStandardItem(part));

            for (int j = i + 1; j < lines.size(); ++j) {
                QString nextLine = lines.at(j);
                if (nextLine.startsWith("  device:")) {
                    QString device = nextLine.section(':', 1).trimmed();
                    model->setItem(row, 1, new QStandardItem(device));
                } else if (nextLine.startsWith("  package:")) {
                    QString package = nextLine.section(':', 1).trimmed();
                    model->setItem(row, 2, new QStandardItem(package));
                } else if (nextLine.startsWith("  speedgrade:")) {
                    QString speedgrade = nextLine.section(':', 1).trimmed();
                    model->setItem(row, 3, new QStandardItem(speedgrade));
                    break;
                }
            }
            row++;
        }
    }
    // 设置整个表格为只读
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置宽度自适应
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 将同一行标记为选中状态
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QObject::connect(tableView, &QTableView::clicked, this, &DefaultPartPage::selectPart);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
}

void DefaultPartPage::selectPart(const QModelIndex &index) {
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tableView->model());
    QStandardItem *deviceItem = model->item(index.row(), 1); // 获取选中行的device
    QStandardItem *packageItem = model->item(index.row(), 2); // 获取选中行的package
    if (deviceItem && packageItem) {
        ProjectWizard *wizard = qobject_cast<ProjectWizard*>(this->wizard());
        wizard->device = deviceItem->text(); // 将device参数发送给ProjectWizard
        wizard->package = packageItem->text(); // 将package参数发送给ProjectWizard
    }
}

