#include "mainwindow.h"

#include "editor.h"
#include "wizard/Wizard.h"
#include "navigator.h"
#include "taskmanager.h"
#include "infowidget.h"
#include "utils/ProcessManager.h"

MainWindow *MainWindow::instance()
{
    static MainWindow *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new MainWindow;
    }
    return m_instance;
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

bool MainWindow::cleanEditorTab()
{
    for (int i = 0; i < tabWidget->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(tabWidget->widget(i));
        if (editor->isModified()) {
            QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "There are unsaved files,"
                                                                                     " are you sure you want to close?",
                                                                    QMessageBox::Yes | QMessageBox::No);
            if (btn == QMessageBox::No) {
                return false;
            } else {
                break;
            }
        }
    }

    while (tabWidget->count() > 0) {
        Editor *editor = qobject_cast<Editor*>(tabWidget->widget(0));
        delete editor;
    }
    return true;
}

bool MainWindow::saveAllFile()
{
    for (int i = 0; i < tabWidget->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(tabWidget->widget(i));
        if (editor->isModified()) {
            QMessageBox::StandardButton btn = QMessageBox::question(this, "Warning", "There are unsaved files,"
                                                                                     " are you sure you want to run?",
                                                                    QMessageBox::Yes | QMessageBox::No);
            if (btn == QMessageBox::No) {
                return false;
            } else {
                break;
            }
        }
    }

    for (int i = 0; i < tabWidget->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(tabWidget->widget(i));
        if (editor->isModified()) {
            if(!editor->saveFile()) {
                return false;
            }
        }
    }
    return true;
}

void MainWindow::onNewTriggered()
{
    Wizard wizard(this);
    wizard.exec();
}

void MainWindow::onOpenFileTriggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    createEditorTab(path);
}

void MainWindow::onOpenTriggered()
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
        Project *proj = new Project;
        proj->openProject(path);
        Navigator::instance()->loadFile(proj);
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

void MainWindow::onEditTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action) {
        return;
    }

    Editor *editor = qobject_cast<Editor*>(tabWidget->currentWidget());
    if (!editor) {
        return;
    }

    if (action == cutAction) {
        editor->cut();
    } else if (action == copyAction) {
        editor->copy();
    } else if (action == pasteAction) {
        editor->paste();
    } else if (action == undoAction) {
        editor->undo();
    } else if (action == redoAction) {
        editor->redo();
    }
}

void MainWindow::onChipPlannerTriggered()
{
    chipPlanner.show();
}

void MainWindow::onDocumentationTriggered()
{
    QDialog documentationDialog(this);
    documentationDialog.setFixedSize(640, 480);
    documentationDialog.setWindowFlags(documentationDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    documentationDialog.setWindowTitle("Documentation");
    QLabel *textLabel = new QLabel(&documentationDialog);
    textLabel->setText("Features to be developed");

    // TODO:load documentation

    documentationDialog.exec();
}

void MainWindow::onAboutTriggered()
{
    QDialog aboutDialog(this);
    aboutDialog.setFixedSize(640, 480);
    aboutDialog.setWindowFlags(aboutDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    aboutDialog.setWindowTitle("About Software");
    QLabel *textLabel = new QLabel(&aboutDialog);
    textLabel->setText("<html><h2>About Software</h2"
                   "<p>© 2024 Power by HybrdChip</p>"
                   "<p><a href='https://www.hybrdchip.com/about'>https://www.hybrdchip.com/about</a>"
                   "</p></html>");
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textLabel->setOpenExternalLinks(true);

    QLabel *imageLabel = new QLabel(&aboutDialog);
    QPixmap image(":/resource/logo.png");
    imageLabel->setPixmap(image.scaled(500, 300));
    imageLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout layout(&aboutDialog);

    layout.addWidget(textLabel);
    layout.addWidget(imageLabel);
    aboutDialog.exec();
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
        // qDebug() << "File" << index << "has been Modified";
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "[Main Window] Constructing...";
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    // 设置窗口初始大小
    this->resize(1600, 900);
    // =================== MENUBAR ====================
    menuBar = new QMenuBar(this), this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("File");
    editMenu = menuBar->addMenu("Edit");
    helpMenu = menuBar->addMenu("Help");
    // ===================== FILE ======================
    newAction = new QAction("New", this), newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    openAction = new QAction("Open", this), openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    closeAction = new QAction("Close", this);
    openFileAction = new QAction("Open File", this);
    saveAction = new QAction("Save", this), saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveasAction = new QAction("Save As", this);
    exitAction = new QAction("Exit", this), exitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
    fileMenu->addActions({newAction, openAction, closeAction}), fileMenu->addSeparator();
    fileMenu->addActions({openFileAction}), fileMenu->addSeparator();
    fileMenu->addActions({saveAction, saveasAction}), fileMenu->addSeparator();
    fileMenu->addActions({exitAction});
    // ================= 文件按钮绑定 ====================
    connect(newAction, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(closeAction, &QAction::triggered, Navigator::instance(), &Navigator::closeProjectAction);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::onOpenFileTriggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(saveasAction, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    // ===================== EDIT ======================
    cutAction = new QAction("Cut", this), cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    copyAction = new QAction("Copy", this), copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    pasteAction = new QAction("Paste", this), pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    undoAction = new QAction("Undo", this), undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    redoAction = new QAction("Redo", this), redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    editMenu->addActions({cutAction, copyAction, pasteAction}), editMenu->addSeparator();
    editMenu->addActions({undoAction, redoAction});
    // ================== 编辑器按钮绑定 ==================
    connect(cutAction, &QAction::triggered, this, &MainWindow::onEditTriggered);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onEditTriggered);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onEditTriggered);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onEditTriggered);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onEditTriggered);
    // ===================== HELP ======================
    documentation = new QAction("Documentation", this);
    aboutAction = new QAction("About", this);
    helpMenu->addActions({documentation, aboutAction});
    connect(documentation, &QAction::triggered, this, &MainWindow::onDocumentationTriggered);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);
    // ================= CHIP PLANNER ==================
    chipPlannerAction = new QAction("ChipPlanner", this);
    connect(chipPlannerAction, &QAction::triggered, this, &MainWindow::onChipPlannerTriggered);
    // =================== TOOLBAR =====================
    toolbar = new QToolBar("Tools", this);
    toolbar->addActions({newAction, openAction, saveAction}), toolbar->addSeparator();
    toolbar->addActions({cutAction, copyAction, pasteAction}), toolbar->addSeparator();
    toolbar->addActions({undoAction, redoAction}), toolbar->addSeparator();
    // toolbar->addActions({chipPlannerAction});
    addToolBar(toolbar);
    // ================= EDITOR TAB ====================
    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetCurrentChanged);
    tabWidget->setMovable(true), tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabWidgetTabCloseRequested); // 关闭编辑器
    this->setCentralWidget(tabWidget);
    updateActionState();
    // ===================== DOCK =====================
    QDockWidget *leftDock1 = new QDockWidget(this);
    leftDock1->setWindowTitle("Project Navigator"), leftDock1->setWidget(Navigator::instance());
    addDockWidget(Qt::LeftDockWidgetArea, leftDock1);

    QDockWidget *leftDock2 = new QDockWidget(this);
    leftDock2->setWindowTitle("Tasks"), leftDock2->setWidget(TaskManager::instance());
    addDockWidget(Qt::LeftDockWidgetArea, leftDock2);

    QDockWidget *bottomDock = new QDockWidget(this);
    bottomDock->setWindowTitle("Information"), bottomDock->setWidget(InfoWidget::instance());
    addDockWidget(Qt::BottomDockWidgetArea, bottomDock);


}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Distructing...";
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
