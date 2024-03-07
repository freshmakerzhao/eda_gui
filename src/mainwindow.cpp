#include "mainwindow.h"

#include "editor.h"
#include "wizard.h"
#include "navigator.h"
#include "taskview.h"
#include "infowidget.h"
#include "processmanager.h"

MainWindow *MainWindow::instance()
{
    static MainWindow *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new MainWindow;
    }
    return m_instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "[Main Window] Constructing...";
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(1600, 900);

    menuBar = new QMenuBar(this), this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("File");
    editMenu = menuBar->addMenu("Edit");

    newAction = new QAction("New", this);
    openAction = new QAction("Open", this);
    openProjectAction = new QAction("Open Project", this);
    saveAction = new QAction("Save", this);
    saveasAction = new QAction("Save As", this);
    fileMenu->addActions({newAction, openAction, openProjectAction}), fileMenu->addSeparator();
    fileMenu->addActions({saveAction, saveasAction});

    connect(newAction, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::onOpenProjectTriggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(saveasAction, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);

    cutAction = new QAction("Cut", this);
    copyAction = new QAction("Copy", this);
    pasteAction = new QAction("Paste", this);
    undoAction = new QAction("Undo", this), redoAction = new QAction("Redo", this);
    editMenu->addActions({cutAction, copyAction, pasteAction}), editMenu->addSeparator();
    editMenu->addActions({undoAction, redoAction});
    // 编辑器按钮绑定
    connect(cutAction, &QAction::triggered, this, &MainWindow::onCutTriggered);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopyTriggered);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onPasteTriggered);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndoTriggered);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedoTriggered);

    chipPlannerAction = new QAction("ChipPlanner", this);
    connect(chipPlannerAction, &QAction::triggered, this, &MainWindow::onChipPlannerTriggered);

    toolbar = new QToolBar("Tools", this);
    toolbar->addActions({newAction, openAction, saveAction}), toolbar->addSeparator();
    toolbar->addActions({cutAction, copyAction, pasteAction}), toolbar->addSeparator();
    toolbar->addActions({undoAction, redoAction}), toolbar->addSeparator();
    toolbar->addActions({chipPlannerAction});
    addToolBar(toolbar);

    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetCurrentChanged);
    tabWidget->setMovable(true), tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabWidgetTabCloseRequested); // 关闭编辑器
    this->setCentralWidget(tabWidget);
    updateActionState();

    // projectNavigator = new ProjectNavigator(this);
    QDockWidget *leftDock1 = new QDockWidget(this);
    leftDock1->setWindowTitle("Project Navigator"), leftDock1->setWidget(Navigator::instance());
    addDockWidget(Qt::LeftDockWidgetArea, leftDock1);

    QDockWidget *leftDock2 = new QDockWidget(this);
    leftDock2->setWindowTitle("Tasks"), leftDock2->setWidget(TaskView::instance(this));
    addDockWidget(Qt::LeftDockWidgetArea, leftDock2);

    QDockWidget *bottomDock = new QDockWidget(this);
    bottomDock->setWindowTitle("Information"), bottomDock->setWidget(InfoWidget::instance());
    addDockWidget(Qt::BottomDockWidgetArea, bottomDock);


}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Distructing...";
}

void MainWindow::updateActionState()
{
    Editor *editor = (Editor*) tabWidget->currentWidget();
    saveAction->setEnabled(editor != nullptr);
    saveasAction->setEnabled(editor != nullptr);
    cutAction->setEnabled(editor != nullptr && !editor->isReadOnly());
    copyAction->setEnabled(editor != nullptr);
    pasteAction->setEnabled(editor != nullptr && !editor->isReadOnly());
    undoAction->setEnabled(editor != nullptr && editor->isUndoAvailable());
    redoAction->setEnabled(editor != nullptr && editor->isRedoAvailable());
}

void MainWindow::createEditorTab(const QString& path)
{
    if(path.isEmpty()) {// 取消打开文件
        return;
    }

    for (int i = 0; i < tabWidget->count(); ++i) { // 不重复打开文件
        if(tabWidget->widget(i)->property("filePath").toString() == path) {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    Editor *editor = new Editor(this); // 创建对象
    editor->setProperty("filePath", path);
    if (!editor->openFile(path)) {
        delete editor;
        return;
    }
    tabWidget->addTab(editor, QFileInfo(path).fileName()); // 添加tab
    tabWidget->setCurrentIndex(tabWidget->count() - 1); // 设置当前文件的索引

    updateActionState();
}

void MainWindow::onNewTriggered()
{
    Wizard *projectWizard = new Wizard(this);
    projectWizard->show();
}

void MainWindow::onOpenTriggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    createEditorTab(path);
}

void MainWindow::onOpenProjectTriggered()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Project");
    dialog.setNameFilter("HPR Files (*.hpr)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QString path = dialog.selectedFiles().value(0, "");
    if (!path.isEmpty()) {
        // 执行打开.hpr文件的逻辑
        Project *project = new Project;
        project->openProject(path);
        Navigator::instance()->loadFile(project);
    }
}

void MainWindow::onSaveTriggered()
{
    Editor *editor = (Editor*) tabWidget->currentWidget();

    if(editor) {
        if(editor->saveFile()) {
            qDebug() << "Save Success";
            // TODO
        }
    }

    updateActionState();
}

void MainWindow::onSaveAsTriggered()
{
    Editor *editor = (Editor*) tabWidget->currentWidget();
    if(editor) {
        if(editor->saveAsFile()) {
            qDebug() << "Save_As Success";
            // TODO
        }
    }
}

void MainWindow::onCutTriggered()
{
    Editor *editor  = (Editor*) tabWidget->currentWidget();
    if (editor) {
        editor->cut();
    }
}

void MainWindow::onCopyTriggered()
{
    Editor *editor  = (Editor*) tabWidget->currentWidget();
    if (editor) {
        editor->copy();
    }
}

void MainWindow::onPasteTriggered()
{
    Editor *editor  = (Editor*) tabWidget->currentWidget();
    if (editor) {
        editor->paste();
    }
}

void MainWindow::onUndoTriggered()
{
    Editor *editor  = (Editor*) tabWidget->currentWidget();
    if (editor) {
        editor->undo();
    }
}

void MainWindow::onRedoTriggered()
{
    Editor *editor  = (Editor*) tabWidget->currentWidget();
    if (editor) {
        editor->redo();
    }
}

void MainWindow::onChipPlannerTriggered()
{
    chipPlanner.show();
}

void MainWindow::onTabWidgetCurrentChanged(int index)
{
    Q_UNUSED(index);
    updateActionState();
}

void MainWindow::onTabWidgetTabCloseRequested(int index)
{
    // qDebug() << "Tab index " << index;
    Editor *editor = qobject_cast<Editor*>(tabWidget->widget(index));
    if (editor->isModified()) {
        qDebug() << "File" << index << "has been Modified";
        QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "The document has been modified.\n"
                                                                                 "Do you want to save your changes?",
                                                                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (btn == QMessageBox::Yes) {
            if(editor->saveFile()) {
                qDebug() << "Save Success";
                // TODO
            }
        } else if (btn == QMessageBox::Cancel) {
            // 对话框的关闭按钮是与QMessageBox::question里面最后一个值绑定的
            return;
        }
    }
    tabWidget->removeTab(index);
    delete editor;

    updateActionState();
}

void MainWindow::streamProcessOutput()
{
    QProcess* process = ProcessManager::instance().getProcess();
    QByteArray output = process->readAllStandardOutput();
    QByteArray errorOutput = process->readAllStandardError();

    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QString outputStr = tc->toUnicode(output);
    QString errorOutputStr = tc->toUnicode(errorOutput);
    if (process->error() == QProcess::UnknownError) {
        // 没有错误发生，输出 output
        // logTextEdit->appendPlainText(outputStr);
        InfoWidget::instance()->appendMsg(outputStr); // 发送到InfoWidget
    } else {
        // 发生错误，输出 errorOutput
        // logTextEdit->appendPlainText(errorOutputStr);
        InfoWidget::instance()->appendMsg(errorOutputStr);
    }
    QCoreApplication::processEvents(); // 刷新终端
}

void MainWindow::configOutputSignals(const QString &phase)
{
    // 清空已绑定的信号
    ProcessManager::instance().getProcess()->disconnect();
    // 合并channel
    ProcessManager::instance().getProcess()->setProcessChannelMode(QProcess::MergedChannels);
    // 绑定 readyReadStandardOutput 信号，有输出则显示在窗口中
    connect(ProcessManager::instance().getProcess(), &QProcess::readyReadStandardOutput, this, &MainWindow::streamProcessOutput);
    // 绑定 finished 信号，执行结束后触发
    connect(ProcessManager::instance().getProcess(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),[=](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << exitCode;
        qDebug() << exitStatus;
        qDebug() << phase + " over";
        // 显示信息弹窗
        // exitCode 为0表示正常执行并成功退出
        if (exitCode == 0) {
            QMessageBox::information(nullptr, "提示", phase + " 完成！");
        } else {
            QMessageBox::critical(nullptr, "错误", phase + " 执行失败！");
        }
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (int i = 0; i < tabWidget->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(tabWidget->widget(i));
        if (editor->isModified()) {
            QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "There are unsaved files,"
                                                                                     " are you sure you want to close?",
                                                                    QMessageBox::Yes | QMessageBox::No);
            if (btn == QMessageBox::Yes) {
                event->accept();
            } else {
                event->ignore();
            }
            break;
        }
    }
}
