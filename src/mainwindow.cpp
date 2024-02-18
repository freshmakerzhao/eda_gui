#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "projectwizard.h"
#include "editor.h"
#include "infowidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "[Main Window] Constructing...";
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget);
    refreshActionState();

    projectNavigator = new ProjectNavigator(this);
    ui->ProjectDockWidget->setWidget(projectNavigator);

    taskView = new TaskView(this);
    ui->TaskDockWidget->setWidget(taskView);

    InfoWidget *infoWidget = new InfoWidget(this);
    ui->InfoDockWidget->setWidget(infoWidget);

    connect(projectNavigator, &ProjectNavigator::sendFilePath, this, &MainWindow::receiveFilePath);

}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Distructing...";
    delete ui;
}

void MainWindow::on_Open_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    createEditorTab(path);

    refreshActionState();
}


void MainWindow::on_Save_triggered()
{
    Editor *m_editor = (Editor*) ui->tabWidget->currentWidget();

    if(m_editor) {
        if(m_editor->saveFile()) {
            qDebug() << "Save Success";
            // TODO
        }
    }

    refreshActionState();
}

void MainWindow::createEditorTab(QString path)
{
    if(path.isEmpty()) {// 取消打开文件
        return;
    }

    QFile file(path); // 打开文件
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件，报错信息：\n" + file.errorString());
        return;
    }

    for(int i = 0; i < ui->tabWidget->count(); ++i) {
        if(ui->tabWidget->widget(i)->property("filePath").toString() == path) {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }

    QTextStream in(&file);
    QString text = in.readAll();

    Editor *m_editor = new Editor(this); // 创建对象
    m_editor->setText(text);// 放置文本
    m_editor->setFilePath(path); // 设置文件路径
    m_editor->initSaveState(); // 初始化保存状态
    m_editor->setProperty("filePath", path);
    ui->tabWidget->addTab(m_editor, QFileInfo(path).fileName()); // 添加tab
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1); // 设置当前文件的索引

    file.close();


}

void MainWindow::refreshActionState()
{
    bool state = ui->tabWidget->count() > 0;
    ui->Undo->setEnabled(state);
    ui->Redo->setEnabled(state);
    ui->Cut->setEnabled(state);
    ui->Copy->setEnabled(state);
    ui->Paste->setEnabled(state);
    ui->Save->setEnabled(state);
}

void MainWindow::receiveFilePath(const QString &path)
{
    qDebug() << path;
    createEditorTab(path);
    refreshActionState();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    qDebug() << "Tab index " << index;
    Editor *m_editor = qobject_cast<Editor*>(ui->tabWidget->widget(index));
    if(!m_editor->checkSaved()) {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "您还没有保存文档！是否保存？",
                                                                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (btn == QMessageBox::Yes) {
            if(m_editor->saveFile()) {
                qDebug() << "Save Success";
                // TODO
            }

        } else if (btn == QMessageBox::Cancel) {
            // 对话框的关闭按钮是与QMessageBox::question里面最后一个值绑定的
            return;
        }
    }
    ui->tabWidget->removeTab(index);
    delete m_editor;
    refreshActionState();
}


void MainWindow::on_New_triggered()
{
    ProjectWizard *projectWizard = new ProjectWizard(this);
    projectWizard->show();

    /*已将创建文件夹功能交给ProjectWizard处理
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select folder path", QDir::homePath());
    qDebug() << folderPath;
    if (!folderPath.isEmpty()) {
        QString folderName = QInputDialog::getText(nullptr, "Create Folder", "Enter folder name");
        QDir dir(folderPath);

        if (dir.mkdir(folderName)) {
            dir.cd(folderName);
            dir.mkdir("doc");
            dir.mkdir("ip");
            dir.mkdir("constrains");
            dir.mkdir("sources");
            qDebug() << "Folders created successfully";
        } else {
            qDebug() << "Failed to create folder";
        }
    }
    */
    connect(projectWizard, &ProjectWizard::wizardAccepted, projectNavigator, &ProjectNavigator::refreshItems);
    // projectNavigator->refreshItems(projectWizard->projectPath);

    refreshActionState();
}


void MainWindow::on_Open_Project_triggered()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if(folder.isEmpty()) {// 取消打开文件夹
        return;
    }
    // qDebug() << folder;

    projectNavigator->refreshItems(folder);

    refreshActionState();
}


void MainWindow::on_Chip_Planner_triggered()
{
    chipPlanner.show();
}


void MainWindow::on_Cut_triggered()
{
    Editor *m_editor  = (Editor*) ui->tabWidget->currentWidget();
    if (m_editor) {
        m_editor->cut();
    }
}


void MainWindow::on_Copy_triggered()
{
    Editor *m_editor  = (Editor*) ui->tabWidget->currentWidget();
    if (m_editor) {
        m_editor->copy();
    }
}


void MainWindow::on_Paste_triggered()
{
    Editor *m_editor  = (Editor*) ui->tabWidget->currentWidget();
    if (m_editor) {
        m_editor->paste();
    }
}


void MainWindow::on_Undo_triggered()
{
    Editor *m_editor  = (Editor*) ui->tabWidget->currentWidget();
    if (m_editor) {
        m_editor->undo();
    }
}


void MainWindow::on_Redo_triggered()
{
    Editor *m_editor  = (Editor*) ui->tabWidget->currentWidget();
    if (m_editor) {
        m_editor->redo();
    }
}


