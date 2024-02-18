#include "projectwizard.h"
#include "ui_projectwizard.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>

ProjectWizard::ProjectWizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::ProjectWizard)
{
    ui->setupUi(this);

    setPage(Page_Init, new InitPage);
    setPage(Page_AddSources, new AddSourcesPage);
    setPage(Page_AddConstraints, new AddConstraintsPage);
    setPage(Page_Summary, new SummaryPage);

    setAttribute(Qt::WA_DeleteOnClose);   // 关闭时释放内存
}

ProjectWizard::~ProjectWizard()
{
    delete ui;
}

void ProjectWizard::accept()   // 重写accept(),完成创建工程文件夹功能
{
    projectName = field("ProjectName").toString();
    projectPath = field("ProjectPath").toString();

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

    emit wizardAccepted(projectPath + '/' + projectName);
    QDialog::accept();
}

InitPage::InitPage(QWidget *parent) // 项目名称，路径填写
    : QWizardPage(parent)
{
    setTitle("Project Name");
    setSubTitle("Enter a name for your project and specify a directory "
                "where the project data files will be stored.");
    QLabel *label = new QLabel("Project Name:");
    QLineEdit *lineEdit = new QLineEdit();
    registerField("ProjectName*", lineEdit);
    QLabel *label2 = new QLabel("Project Path:");
    QLineEdit *lineEdit2 = new QLineEdit();
    lineEdit2->setText(QDir::homePath());    // 设置默认路径为home
    registerField("ProjectPath", lineEdit2);
    // Add the label and line edit to the page layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0);
    layout->addWidget(lineEdit, 0, 1);
    layout->addWidget(label2, 1, 0);
    layout->addWidget(lineEdit2, 1, 1);
    setLayout(layout);
}

AddSourcesPage::AddSourcesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Add Sources");
    setSubTitle("Specify HDL, netlist, Block Design, and iP files, or "
                "directories containing those files, to add to your project.");

    QTableView *tableView = new QTableView(this);
    // tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
    model->setHorizontalHeaderItem(0, new QStandardItem("Type"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Name"));
    tableView->setModel(model);


    QPushButton *addFilesButton = new QPushButton("Add Files", this);

    connect(addFilesButton, &QPushButton::clicked, [model]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Select file", QDir::homePath());

        if (!fileName.isEmpty()) {
            QFileInfo fileInfo(fileName);
            QString name = fileInfo.fileName();
            QString extension = fileInfo.suffix();
            QString type;
            if(extension == "v") {
                type = "verilog";
            } else {
                type = extension;
            }
            int rowCount = model->rowCount();
            model->insertRow(rowCount);
            model->setItem(rowCount, 0, new QStandardItem(type));
            model->setItem(rowCount, 1, new QStandardItem(name));
        }
    });

    QHBoxLayout *layout2 = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(tableView);
    layout2->addWidget(addFilesButton);
    layout->addLayout(layout2);
    setLayout(layout);
}

AddConstraintsPage::AddConstraintsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Add Constraints");
    setSubTitle("Specify or create constraint files for physical and "
                "timing constraints");

    QTableView *tableView = new QTableView(this);
    QStandardItemModel *model = new QStandardItemModel(0, 2, this);

    tableView->setModel(model);

    QPushButton *addFilesButton = new QPushButton("Add Files", this);

    connect(addFilesButton, &QPushButton::clicked, [model]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Select file", QDir::homePath());

        if (!fileName.isEmpty()) {
            QFileInfo fileInfo(fileName);
            QString name = fileInfo.fileName();
            QString extension = fileInfo.suffix();
            int rowCount = model->rowCount();
            model->insertRow(rowCount);
            model->setItem(rowCount, 0, new QStandardItem(extension));
            model->setItem(rowCount, 1, new QStandardItem(name));
        }
    });

    QHBoxLayout *layout2 = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(tableView);
    layout2->addWidget(addFilesButton);
    layout->addLayout(layout2);

    setLayout(layout);
}

SummaryPage::SummaryPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("New Project Summary");
    namelabel = new QLabel("Project Name");
    prjnamelabel = new QLabel();
    pathlabel = new QLabel("Project Path");
    prjpathlabel = new QLabel();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(namelabel, 0, 0);
    layout->addWidget(prjnamelabel, 0, 1);
    layout->addWidget(pathlabel, 1, 0);
    layout->addWidget(prjpathlabel, 1, 1);
    setLayout(layout);
}

void SummaryPage::initializePage() // 获取填写的项目信息显示在SummaryPage
{
    QString projectName = field("ProjectName").toString();
    QString projectPath = field("ProjectPath").toString();

    prjnamelabel->setText(projectName);
    prjpathlabel->setText(projectPath);
}
