/**
  ******************************************************************************
  * @file           : SourcesPage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#include "SourcePage.h"
#include "dialog/AdvancedFileDialog.h"

SourcesPage::SourcesPage(QWidget *parent, const int mode) : QWizardPage(parent)
{
    _mode = mode;

    setTitle("Add Sources");
    setSubTitle("Specify HDL, netlist, Block Design, and iP files to add to your project.");

    model = new QStandardItemModel(this);
    model->setColumnCount(4); // 增加列数，用于显示索引
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Index")); // 设置标题
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("File Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("File Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Location"));

    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    tableView->verticalHeader()->hide(); //隐藏行号方法
    // tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableView->setColumnWidth(0, 85);
    tableView->setColumnWidth(1, 250);
    tableView->setColumnWidth(3, 340);

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &SourcesPage::onAddFiles);

    QPushButton *createFileButton = new QPushButton("Create Files");createFileButton->setFixedSize(160, 45);
    connect(createFileButton, &QPushButton::clicked, this, &SourcesPage::onCreateFile);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &SourcesPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addWidget(tableView);
    btnLayout->addWidget(addFilesButton);
    btnLayout->addWidget(createFileButton);
    btnLayout->addWidget(removeButton);
    layout->addLayout(btnLayout);
    setLayout(layout);
}

void SourcesPage::onAddFiles()
{
    int currentIndex = model->rowCount() + 1; // 用于记录当前索引
    // QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select Files", "", "Verilog Source Files (*.v)");
    AdvancedFileDialog dialog(this);
    dialog.setWindowTitle("Select Files");
    dialog.setNameFilter("Verilog Source Files (*.v)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QStringList fileNames = dialog.selectedFiles();
    for (const QString &fileName : fileNames) {
        QFileInfo fileInfo(fileName);
        QList<QStandardItem *> items;
        items << new QStandardItem(QString::number(currentIndex++)); // 设置索引
        items << new QStandardItem(fileInfo.fileName());
        auto it = Map.find(fileInfo.suffix());
        if (it != Map.end()){
            items << new QStandardItem(Map[fileInfo.suffix()]);
        }else{
            items << new QStandardItem(fileInfo.suffix());
        }
        items << new QStandardItem(QString(fileInfo.path()));
        model->appendRow(items);
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->sourcesFilesList.append(fileName); // 添加文件路径到列表中

        qDebug() << "-----------------------------------------------------";
        for(auto it : wizard->sourcesFilesList){
            qDebug() << it;
        }
    }
}

void SourcesPage::onCreateFile()
{
    QDialog dialog(this);
    dialog.setFixedSize(320, 180);
    dialog.setWindowTitle("Create Source File");
    QComboBox *comboBox = new QComboBox(&dialog);
    comboBox->addItem("Verilog");

    QFormLayout *formLayout = new QFormLayout(&dialog);
    formLayout->addRow("File type:", comboBox);

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    lineEdit->setClearButtonEnabled(true);
    formLayout->addRow("File name:", lineEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    formLayout->addRow(buttonBox);
    formLayout->setVerticalSpacing(25); // 设置垂直间距

    connect(buttonBox, &QDialogButtonBox::accepted, [comboBox, lineEdit, &dialog, this](){
        QString fileType = comboBox->currentText();
        QString fileName = lineEdit->text();

        QString extension;
        if (fileType == "Verilog") {
            extension = ".v";
        }else {
            // Handle other file types if needed
        }

        QDir directory("Cache");
        if (!directory.exists()) {
            directory.mkpath(".");
        }
        // TODO:文件名不能为空
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

        int currentIndex = model->rowCount() + 1;
        QFileInfo fileInfo(newFilePath);
        QList<QStandardItem *> items;
        items << new QStandardItem(QString::number(currentIndex++)); // 设置索引
        items << new QStandardItem(fileInfo.fileName());
        auto it = Map.find(fileInfo.suffix());
        if (it != Map.end()){
            items << new QStandardItem(Map[fileInfo.suffix()]);
        }else{
            items << new QStandardItem(fileInfo.suffix());
        }
        items << new QStandardItem("<Local to Project>");
        model->appendRow(items);
        Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
        wizard->sourcesFilesList.append(newFilePath); // 添加文件路径到列表中

        qDebug() << "-----------------------------------------------------";
        for(auto it : wizard->sourcesFilesList){
            qDebug() << it;
        }

        dialog.accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void SourcesPage::onRemoveFiles()
{
    int currentIndex = 1;
    Wizard* wizard = qobject_cast<Wizard*>(this->wizard());
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    for (const QModelIndex &index : selectedIndexes) {
        model->removeRow(index.row());
        wizard->sourcesFilesList.removeAt(index.row()); // 从文件路径列表中移除对应的文件路径
    }
    // 重新设置索引
    for (int row = 0; row < model->rowCount(); ++row) {
        model->setData(model->index(row, 0), row + 1);
    }
    currentIndex = model->rowCount() + 1; // 更新当前索引

    qDebug() << "-----------------------------------------------------";
    for(auto it : wizard->sourcesFilesList){
        qDebug() << it;
    }
}

int SourcesPage::nextId() const
{
    // 跳过其他页面
    if (_mode == 1) {
        return -1;
    }
    return QWizardPage::nextId();
}
