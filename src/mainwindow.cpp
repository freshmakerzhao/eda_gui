/**
  ******************************************************************************
  * @file           : mainwindow.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/9
  ******************************************************************************
  */
#include "mainwindow.h"

#include "widgets/Editor.h"
#include "wizard/Wizard.h"
#include "widgets/Navigator.h"
#include "widgets/Infowidget.h"
#include "widgets/FlowNavigator.h"
#include "dialog/AboutDialog.h"

MainWindow *MainWindow::instance()
{
    static MainWindow *_instance = nullptr;
    if (!_instance) {
        _instance = new MainWindow;
    }
    return _instance;
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
    dialog.setNameFilter("HybrdLink Project File (*.hpr)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QString path = dialog.selectedFiles().value(0, "");
    if (!path.isEmpty()) {
        // 执行打开.hpr文件的逻辑
        Project *proj = new Project;
        proj->parseProject(path);
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
    // chipPlanner.show();
}

void MainWindow::onDocumentationTriggered()
{
    // TODO:load documentation
}

void MainWindow::onAboutTriggered()
{
    AboutDialog aboutDialog(this);
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int leftwidth = int(this->width() * 0.18);//左边的停靠窗宽是主界面的0.18倍
    int rightwidth = int(this->width() * 0.82);//右边的停靠窗宽是主界面的0.82倍
    resizeDocks({ManagerDock, BottomDock}, {42, 18}, Qt::Vertical);//右侧上下布局42 : 18
    resizeDocks({NavigationBar, BottomDock, ManagerDock},{leftwidth, rightwidth, rightwidth}, Qt::Horizontal);//左右水平布局0.18 : 0.82
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "[Main Window] Constructing...";
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("HybrdLink");
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    // 设置窗口初始大小
    this->resize(1700, 1000);
    // =================== MENUBAR ====================
    menuBar = new QMenuBar(this), this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("File");
    editMenu = menuBar->addMenu("Edit");
    viewMenu = menuBar->addMenu("View");
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
    documentation->setDisabled(true);
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
    // this->setCentralWidget(tabWidget);
    updateActionState();
    // ===================== DOCK =====================
    NavigationBar = new QDockWidget(this);
    NavigationBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    NavigationBar->setFeatures(QDockWidget::DockWidgetClosable);
    NavigationBar->setWindowTitle("FLOW NAVIGATOR");
    FlowNavigator *flowNavigator = new FlowNavigator(this);
    NavigationBar->setWidget(flowNavigator);
    addDockWidget(Qt::LeftDockWidgetArea, NavigationBar, Qt::Vertical);

    BottomDock = new QDockWidget(this);
    BottomDock->setWindowTitle("INFOMATION");
    BottomDock->setWidget(InfoWidget::instance());
    addDockWidget(Qt::BottomDockWidgetArea, BottomDock);

    ManagerDock = new QDockWidget(this);
    ManagerDock->setWindowTitle("PROJECT MANAGER");
    // ManagerDock->setWidget(tabWidget);
    ManagerDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    ManagerDock->setFeatures(QDockWidget::DockWidgetClosable);
    addDockWidget(Qt::TopDockWidgetArea, ManagerDock);

    splitDockWidget(NavigationBar, BottomDock, Qt::Horizontal);
    splitDockWidget(NavigationBar, ManagerDock, Qt::Horizontal);
    splitDockWidget(ManagerDock, BottomDock, Qt::Vertical);

    DockManager = new ads::CDockManager(ManagerDock);
    ManagerDock->setWidget(DockManager);

    SourcesWidget = new ads::CDockWidget("Sources", DockManager);
    // SourcesWidget->setFeature(ads::CDockWidget::NoTab, true);
    DockManager->addDockWidget(ads::LeftDockWidgetArea, SourcesWidget);
    SourcesWidget->setWidget(Navigator::instance());

    // ads::CDockWidget *PropertiesWidget = new ads::CDockWidget("Properties", DockManager);
    // DockManager->addDockWidget(ads::BottomDockWidgetArea, PropertiesWidget);

    EditWidget = new ads::CDockWidget("Editor", DockManager);
    DockManager->addDockWidget(ads::RightDockWidgetArea, EditWidget);
    EditWidget->setWidget(tabWidget);
    EditWidget->setMinimumSize(450, 10);
    // EditWidget->setMinimumSize(1300, 10);

    // ===================== VIEW ======================
    viewMenu->addAction(NavigationBar->toggleViewAction());
    viewMenu->addAction(BottomDock->toggleViewAction());
    viewMenu->addAction(ManagerDock->toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction(SourcesWidget->toggleViewAction());
    viewMenu->addAction(EditWidget->toggleViewAction());
}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Distructing...";
}

