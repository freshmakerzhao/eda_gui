#include "wizard.h"

#include "navigator.h"
#include <yaml-cpp/yaml.h>

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

        // 创建runs文件夹
        if (dir.mkdir("runs")) {
            // cd进入runs文件夹
            dir.cd("runs");
            // 在runs文件夹内创建.works、impl和synth文件夹
            dir.mkdir(".works"); // 记录中间过程，方便后续在此基础上继续执行
            dir.mkdir("impl"); // pack place route
            dir.mkdir("synth"); // synth
            qDebug() << "Folders Created Successfully including runs/.works、runs/impl and runs/synth";
        } else {
            qDebug() << "Failed to create runs folder";
        }

        // 返回到原来的项目文件夹路径下
        dir.cdUp();
    } else {
        qDebug() << "Folders Created Unsuccessfully";
        return;
    }

    QString targetPath =  projectPath + "/" + projectName;
    // 复制文件列表中的文件到项目文件夹sources
    foreach (const QString &file, sourcesFilesList) {
        QFile::copy(file, targetPath + "/sources/" + QFileInfo(file).fileName());
    }
    // 复制文件列表中的文件到项目文件夹constrains
    foreach (const QString &file, constraintFilesList) {
        QFile::copy(file, targetPath + "/constraints/" + QFileInfo(file).fileName());
    }

    // ======================== 记录复制文件后的文件位置 =========================
    QStringList sourcetmp,constrainttmp;
    for (const QString& source : sourcesFilesList) {
        QString newPath =  targetPath + "/sources/" + QFileInfo(source).fileName();
        sourcetmp.append(newPath);
    }
    for (const QString& constraint : constraintFilesList) {
        QString newPath =  targetPath + "/constraints/" + QFileInfo(constraint).fileName();
        constrainttmp.append(newPath);
    }
    // ============================= 生成工程 =================================
    project = new Project(projectName, targetPath, part, arch, archName);
    project->sourceList = sourcetmp;
    project->constraintList = constrainttmp;
    project->makeProject();
    Navigator::instance()->loadFile(project);

    // ============================= 清除缓存 =================================
    QDir dircache("Cache");
    if (!dircache.isEmpty()) {
        dircache.removeRecursively();
    }
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
    connect(browseButton, &QPushButton::clicked, [=]() {
        QString path = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
        if (!path.isEmpty()) {
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

    QPushButton *createFileButton = new QPushButton("Create Files");createFileButton->setFixedSize(160, 45);
    connect(createFileButton, &QPushButton::clicked, this, &AddSourcesPage::onCreateFile);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &AddSourcesPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    btnLayout->addWidget(addFilesButton);
    btnLayout->addWidget(createFileButton);
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

void AddSourcesPage::onCreateFile()
{
    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(320, 180);
    dialog->setWindowTitle("Create Source File");
    QComboBox *comboBox = new QComboBox(dialog);
    comboBox->addItem("Verilog");

    QFormLayout *formLayout = new QFormLayout(dialog);
    formLayout->addRow("File type:", comboBox);

    QLineEdit *lineEdit = new QLineEdit(dialog);
    lineEdit->setClearButtonEnabled(true);
    formLayout->addRow("File name:", lineEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    formLayout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [comboBox, lineEdit, dialog, this](){
        QString fileType = comboBox->currentText();
        QString fileName = lineEdit->text();

        QString extension;
        if (fileType == "Verilog") {
            extension = ".v";
        }
        else {
            // Handle other file types if needed
        }

        QDir directory("Cache");
        if (!directory.exists()) {
            directory.mkpath(".");
        }

        QString newFilePath = "Cache/" + fileName + extension;
        QFile file(newFilePath);
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            // File created successfully, do any additional processing here if needed
            // QTextStream stream(&file);
            file.close();
        }
        else {
            // Failed to create file, handle error appropriately
        }
        QStringList tmp;
        tmp.append(newFilePath);
        emit filesListUpdatedSignal(tmp);
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->sourcesFilesList.append(newFilePath);  // 将新建的文件追加到列表中
        dialog->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    dialog->setLayout(formLayout);
    dialog->show();
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

    tableView = new QTableView(this);
    tableView->verticalHeader()->setVisible(false); // 不显示行号
    QStandardItemModel *model = new QStandardItemModel(0, 4, this);
    tableView->setModel(model);
    QStringList headers = {"part", "device", "package", "speedgrade", "archName" , "arch"};
    model->setHorizontalHeaderLabels(headers);

    QFile file(":/resource/parts.yaml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 从资源读取YAML文件内容
        QTextStream in(&file);
        QString yamlData = in.readAll();

        // 使用yaml-cpp库解析YAML数据
        try {
            YAML::Node config = YAML::Load(yamlData.toStdString());
            // 处理YAML数据
            qDebug() << "YAML loaded successfully";
            int row = 0;
            for (auto it = config.begin(); it != config.end(); ++it) {
                QString part = QString::fromStdString(it->first.as<std::string>());
                QString device = QString::fromStdString(it->second["device"].as<std::string>());
                QString package = QString::fromStdString(it->second["package"].as<std::string>());
                QString speedgrade = QString::fromStdString(it->second["speedgrade"].as<std::string>());
                QString archName = QString::fromStdString(it->second["archName"].as<std::string>());
                QString arch = QString::fromStdString(it->second["arch"].as<std::string>());
                model->setItem(row, 0, new QStandardItem(part));
                model->setItem(row, 1, new QStandardItem(device));
                model->setItem(row, 2, new QStandardItem(package));
                model->setItem(row, 3, new QStandardItem(speedgrade));
                model->setItem(row, 4, new QStandardItem(archName));
                model->setItem(row, 5, new QStandardItem(arch));
                row++;
            }
        } catch (const YAML::ParserException& e) {
            qDebug() << "Error parsing YAML: " << e.what();
        } catch (const YAML::Exception& e) {
            qDebug() << "YAML error: " << e.what();
        }
        file.close();
    } else {
        qDebug() << "Error opening file";
    }

    // 设置整个表格为只读
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置宽度自适应
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 将同一行标记为选中状态
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    // 隐藏第四 第五列 archName/arch 仅供程序使用，用户无需获取
    tableView->setColumnHidden(4, true);
    tableView->setColumnHidden(5, true);
    QObject::connect(tableView, &QTableView::clicked, this, &DefaultPartPage::selectPart);

    // Filter
    // QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    // proxyModel->setSourceModel(model);
    // tableView->setModel(proxyModel);
    // QLineEdit *lineEdit = new QLineEdit(this);
    // connect(lineEdit, &QLineEdit::textChanged, [proxyModel](const QString &text){
    //     proxyModel->setFilterFixedString(text);
    // });

    QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(lineEdit);
    layout->addWidget(tableView);
}

bool DefaultPartPage::isComplete() const
{
    // Part为必选项
    if (tableView->selectionModel()->selectedIndexes().isEmpty()) {
        return false; // 不满足条件，禁止进入下一步
    }
    return true;
}

void DefaultPartPage::selectPart(const QModelIndex &index) {
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tableView->model());
    QStandardItem *partItem = model->item(index.row(), 0); // 获取part
    QStandardItem *archNameItem = model->item(index.row(), 4); // 获取archName
    QStandardItem *archItem = model->item(index.row(), 5); // 获取arch
    if (partItem) {
        Wizard *wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->part = partItem->text();
        wizard->archName = archNameItem->text();
        wizard->arch = archItem->text();
    }
    completeChanged();
}

