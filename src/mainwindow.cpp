#include "mainwindow.h"

#include "editor.h"
#include "projectwizard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "[Main Window] Constructing...";
    this->resize(1600, 900);

    menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("File");
    editMenu = menuBar->addMenu("Edit");

    newAction = new QAction("New", this);
    openAction = new QAction("Open", this);
    openProjectAction = new QAction("Open Project", this);
    saveAction = new QAction("Save", this);
    fileMenu->addActions({newAction, openAction, openProjectAction, saveAction});

    connect(newAction, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::onOpenProjectTriggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    cutAction = new QAction("Cut", this);
    copyAction = new QAction("Copy", this);
    pasteAction = new QAction("Paste", this);
    undoAction = new QAction("Undo", this);
    redoAction = new QAction("Redo", this);
    editMenu->addActions({cutAction, copyAction, pasteAction, undoAction, redoAction});

    // 编辑器按钮绑定
    connect(cutAction, &QAction::triggered, this, &MainWindow::onCutTriggered);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopyTriggered);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onPasteTriggered);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndoTriggered);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedoTriggered);

    chipPlannerAction = new QAction("ChipPlanner", this);
    connect(chipPlannerAction, &QAction::triggered, this, &MainWindow::onChipPlannerTriggered);

    toolbar = new QToolBar("Tools", this);
    toolbar->addActions({newAction, openAction, saveAction, cutAction, copyAction, pasteAction, undoAction, redoAction, chipPlannerAction});
    addToolBar(toolbar);

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabWidgetTabCloseRequested); // 关闭编辑器
    this->setCentralWidget(tabWidget);
    refreshActionState();

    projectNavigator = new ProjectNavigator(this);
    QDockWidget *leftDock1 = new QDockWidget(this);
    leftDock1->setWindowTitle("Project Navigator");
    leftDock1->setWidget(projectNavigator);
    addDockWidget(Qt::LeftDockWidgetArea, leftDock1);

    taskView = new TaskView(this);
    QDockWidget *leftDock2 = new QDockWidget(this);
    leftDock2->setWindowTitle("Tasks");
    leftDock2->setWidget(taskView);
    addDockWidget(Qt::LeftDockWidgetArea, leftDock2);

    infoWidget = new InfoWidget(this);
    QDockWidget *bottomDock = new QDockWidget(this);
    bottomDock->setWindowTitle("Information");
    bottomDock->setWidget(infoWidget);
    addDockWidget(Qt::BottomDockWidgetArea, bottomDock);

    // 从projectNavigator获取文件路径，在编辑器打开
    connect(projectNavigator, &ProjectNavigator::sendFilePath, this, &MainWindow::receiveFilePath);
}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Distructing...";
}

void MainWindow::receiveFilePath(const QString &path)
{
    qDebug() << path;
    createEditorTab(path);
    refreshActionState();
}

void MainWindow::onNewTriggered()
{
    ProjectWizard *projectWizard = new ProjectWizard(this);
    projectWizard->show();
    connect(projectWizard, &ProjectWizard::wizardAccepted, projectNavigator, &ProjectNavigator::refreshItems);
}

void MainWindow::onOpenTriggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    createEditorTab(path);
    refreshActionState();
}

void MainWindow::onOpenProjectTriggered()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if(folder.isEmpty()) {// 取消打开文件夹
        return;
    }
    // qDebug() << folder;

    projectNavigator->refreshItems(folder);

}

void MainWindow::onSaveTriggered()
{
    Editor *m_editor = (Editor*) tabWidget->currentWidget();

    if(m_editor) {
        if(m_editor->saveFile()) {
            qDebug() << "Save Success";
            // TODO
        }
    }

    refreshActionState();
}

void MainWindow::onCutTriggered()
{
    Editor *m_editor  = (Editor*) tabWidget->currentWidget();
    if (m_editor) {
        if (m_editor->isReadOnly()) {
            QMessageBox::warning(this, "警告", "该文件处于只读模式");
        } else{
            m_editor->cut();
        }
    }
}

void MainWindow::onCopyTriggered()
{
    Editor *m_editor  = (Editor*) tabWidget->currentWidget();
    if (m_editor) {
        m_editor->copy();
    }
}

void MainWindow::onPasteTriggered()
{
    Editor *m_editor  = (Editor*) tabWidget->currentWidget();
    if (m_editor) {
        if (m_editor->isReadOnly()) {
            QMessageBox::warning(this, "警告", "该文件处于只读模式");
        } else{
            m_editor->paste();
        }
    }
}

void MainWindow::onUndoTriggered()
{
    Editor *m_editor  = (Editor*) tabWidget->currentWidget();
    if (m_editor) {
        if (m_editor->isReadOnly()) {
            QMessageBox::warning(this, "警告", "该文件处于只读模式");
        } else{
            m_editor->undo();
        }
    }
}

void MainWindow::onRedoTriggered()
{
    Editor *m_editor  = (Editor*) tabWidget->currentWidget();
    if (m_editor) {
        if (m_editor->isReadOnly()) {
            QMessageBox::warning(this, "警告", "该文件处于只读模式");
        } else{
            m_editor->redo();
        }
    }
}

void MainWindow::onChipPlannerTriggered()
{
    chipPlanner.show();
}

void MainWindow::onTabWidgetTabCloseRequested(int index)
{
    // qDebug() << "Tab index " << index;
    Editor *m_editor = qobject_cast<Editor*>(tabWidget->widget(index));
    if(!m_editor->checkSaved()) {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "文件未保存！是否保存？",
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
    tabWidget->removeTab(index);
    delete m_editor;

    refreshActionState();
}

void MainWindow::createEditorTab(const QString& path)
{
    if(path.isEmpty()) {// 取消打开文件
        return;
    }

    QFile file(path); // 打开文件
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件，报错信息：\n" + file.errorString());
        return;
    }

    for(int i = 0; i < tabWidget->count(); ++i) {
        if(tabWidget->widget(i)->property("filePath").toString() == path) {
            tabWidget->setCurrentIndex(i);
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
    tabWidget->addTab(m_editor, QFileInfo(path).fileName()); // 添加tab
    tabWidget->setCurrentIndex(tabWidget->count() - 1); // 设置当前文件的索引

    file.close();

    refreshActionState();
}

void MainWindow::refreshActionState()
{
    bool state = tabWidget->count() > 0;
    saveAction->setEnabled(state);
    cutAction->setEnabled(state);
    copyAction->setEnabled(state);
    pasteAction->setEnabled(state);
    undoAction->setEnabled(state);
    redoAction->setEnabled(state);

}
