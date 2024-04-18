#include "SourcePage.h"

SourcesPage::SourcesPage(QWidget *parent) : QWizardPage(parent)
{
    connect(this, &SourcesPage::filesListUpdatedSignal, this, &SourcesPage::updateFilesList);

    setTitle("Add Sources");
    setSubTitle("Specify HDL, netlist, Block Design, and iP files to add to your project.");

    filesListWidget = new QListWidget;
    filesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    QPushButton *addFilesButton = new QPushButton("Add Files");addFilesButton->setFixedSize(160, 45);
    connect(addFilesButton, &QPushButton::clicked, this, &SourcesPage::onAddFiles);

    QPushButton *createFileButton = new QPushButton("Create Files");createFileButton->setFixedSize(160, 45);
    connect(createFileButton, &QPushButton::clicked, this, &SourcesPage::onCreateFile);

    QPushButton *removeButton = new QPushButton("Remove Files");removeButton->setFixedSize(160, 45);
    connect(removeButton, &QPushButton::clicked, this, &SourcesPage::onRemoveFiles);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addWidget(filesListWidget);
    btnLayout->addWidget(addFilesButton);
    btnLayout->addWidget(createFileButton);
    btnLayout->addWidget(removeButton);
    layout->addLayout(btnLayout);
    setLayout(layout);
}

void SourcesPage::onAddFiles()
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
        dialog.accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void SourcesPage::onRemoveFiles()
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

void SourcesPage::updateFilesList(const QStringList &files)
{
    filesListWidget->addItems(files);
}
