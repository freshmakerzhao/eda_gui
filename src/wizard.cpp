#include "wizard.h"

#include "navigator.h"

Wizard::Wizard(QWidget *parent) : QWizard(parent)
{
    qDebug() << "[Wizard] Constructing...";
    setWindowTitle("Wizard");
    setWizardStyle(QWizard::ModernStyle);
    addPage(new ProjectNamePage);
    addPage(new AddSourcesPage);
    addPage(new AddConstraintPage);
    addPage(new DefaultPartPage);
    connect(this, &QWizard::accepted, this, &Wizard::onFinish);

    resize(900, 600);

    setAttribute(Qt::WA_DeleteOnClose);
}

Wizard::~Wizard()
{
    qDebug() << "[Wizard] Distructing...";
}

void Wizard::onFinish()
{
    // 获取项目名称和路径
    QString projectName = field("projectName").toString();
    QString projectPath = field("projectPath").toString();

    // 创建项目文件夹
    QDir dir(projectPath);
    if (dir.mkdir(projectName)) {
        dir.cd(projectName);
        dir.mkdir("sources");
        dir.mkdir("constraints");
        dir.mkdir("doc");
        dir.mkdir("ip");   
        qDebug() << "Folders Created Successfully";
    } else {
        qDebug() << "Folders Created Unsuccessfully";
        return;
    }

    // 复制文件列表中的文件到项目文件夹sources
    foreach (const QString &file, sourcesFilesList) {
        QFile::copy(file, projectPath + "/" + projectName + "/sources/" + QFileInfo(file).fileName());
    }
    // 复制文件列表中的文件到项目文件夹constrains
    foreach (const QString &file, constraintFilesList) {
        QFile::copy(file, projectPath + "/" + projectName + "/constraints/" + QFileInfo(file).fileName());
    }

    project = new Project(projectName, projectPath + "/" + projectName, part);
    project->sourceList = sourcesFilesList;
    project->constraintList = constraintFilesList;
    project->makeProject();
    Navigator::instance()->loadFile(project);
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
    connect(this, &AddSourcesPage::filesListUpdatedSignal, this, &AddSourcesPage::updateFilesList);

    setTitle("Add Sources");
    setSubTitle("Specify HDL, netlist, Block Design, and iP files to add to your project.");

    filesListWidget = new QListWidget;
    filesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 将文件列表注册为QStringList以便与QWizard的字段交互
    // registerField("addsourcesFilesList", filesListWidget, "selectedItems", SIGNAL(itemSelectionChanged()));

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &AddSourcesPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &AddSourcesPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    btnLayout->addWidget(addFilesButton);
    btnLayout->addWidget(removeButton);
    layout->addLayout(btnLayout);
    setLayout(layout);
}

void AddSourcesPage::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "Verilog Source Files (*.v)");
    if (!files.isEmpty())
    {
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->sourcesFilesList.append(files);  // 将选择的文件追加到列表中
        for (int i = 0; i < wizard->sourcesFilesList.size(); ++i) {
            qDebug() << wizard->sourcesFilesList.at(i);
        }
        emit filesListUpdatedSignal(files); // 发送信号
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
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->sourcesFilesList.removeAt(row);
    }
}

void AddSourcesPage::updateFilesList(const QStringList &files)
{
    filesListWidget->addItems(files);
}

AddConstraintPage::AddConstraintPage(QWidget *parent) : QWizardPage(parent)
{
    connect(this, &AddConstraintPage::filesListUpdatedSignal, this, &AddConstraintPage::updateFilesList);

    setTitle("Add Constrains");
    setSubTitle("Specify or create constraint files for physical and "
                "timing constraints.");

    filesListWidget = new QListWidget;
    filesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 将文件列表注册为QStringList以便与QWizard的字段交互
    // registerField("addconstrainsFilesList", filesListWidget, "selectedItems", SIGNAL(itemSelectionChanged()));

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &AddConstraintPage::onAddFiles);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &AddConstraintPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    btnLayout->addWidget(addFilesButton);
    btnLayout->addWidget(removeButton);
    layout->addLayout(btnLayout);
    setLayout(layout);
}

void AddConstraintPage::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "Verilog Source Files (*.xdc)");
    if (!files.isEmpty())
    {
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->constraintFilesList.append(files);  // 将选择的文件追加到列表中
        for (int i = 0; i < wizard->constraintFilesList.size(); ++i) {
            qDebug() << wizard->constraintFilesList.at(i);
        }
        emit filesListUpdatedSignal(files); // 发送信号
    }
}

void AddConstraintPage::onRemoveFiles()
{
    QList<QListWidgetItem*> selectedItems = filesListWidget->selectedItems();
    for (QListWidgetItem* item : selectedItems)
    {
        int row = filesListWidget->row(item);
        filesListWidget->takeItem(row);
        // 从constrainsFilesList中移除对应项
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->constraintFilesList.removeAt(row);
    }
}

void AddConstraintPage::updateFilesList(const QStringList &files)
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

bool DefaultPartPage::isComplete() const
{
    if (tableView->selectionModel()->selectedIndexes().isEmpty()) {
        return false; // 不满足条件，禁止进入下一步
    }
    return true;
}

void DefaultPartPage::selectPart(const QModelIndex &index) {
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tableView->model());
    QStandardItem *partItem = model->item(index.row(), 0); // 获取part
    if (partItem) {
        Wizard *wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->part = partItem->text();
    }
    completeChanged();
}

