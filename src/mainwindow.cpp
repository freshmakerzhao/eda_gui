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
#include "wizard/Wizard.h"
#include "widgets/Editor.h"
#include "widgets/InfoWidget.h"
#include "widgets/FlowNavigator.h"
#include "widgets/Form.h"
#include "widgets/FileManager.h"
#include "widgets/EditorManager.h"
#include "dialog/AboutDialog.h"
#include "dialog/CustomMessageBox.h"
#include "utils/ProjectManager.h"
#include "entity/XmlRecent.h"
#include "utils/XmlUtilities.h"
#include "base/InitialConfig.h"
#include "ipmanager/IPManager.h"
#include "widgets/PrjSummary.h"
#include "dialog/AdvancedFileDialog.h"
#include "base/TreeViewBase.h"

MainWindow *MainWindow::instance()
{
    static MainWindow *_instance = nullptr;
    if (!_instance) {
        _instance = new MainWindow(nullptr);
    }
    return _instance;
}

void MainWindow::updateActionState()
{
    Editor *editor = EditorManager::instance()->currentEditor();
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
    EditorManager::instance()->createEditorTab(path);
    EditWidget->toggleView(true);
    setCurrentDock(0);
}

bool MainWindow::cleanEditorTab()
{
    return EditorManager::instance()->cleanEditorTab();
}

bool MainWindow::saveAllFile()
{
    return EditorManager::instance()->saveAllFiles();
}

void MainWindow::showProjectTitle(const int &mode, const QString &title)
{
    /*
     * mode 0 设置工程目录Title
     * mode 1 还原Title
    **/
    if (mode == 0) {
        setWindowTitle("HybrdLink -[" + title + "]");
        return;
    }
    setWindowTitle("HybrdLink");
}

void MainWindow::setForm(const int &mode)
{
    if (mode == 0) {
        Form::instance()->hide();
        toolbar->show();
        ManagerDock->show();
        BottomDock->show();
        NavigationBar->show();
        ManagerDock->toggleViewAction()->setEnabled(true);
        BottomDock->toggleViewAction()->setEnabled(true);
        NavigationBar->toggleViewAction()->setEnabled(true);
        resizeUi();
        return;
    }
    Form::instance()->show();
    toolbar->hide();
    ManagerDock->hide();
    BottomDock->hide();
    NavigationBar->hide();
    ManagerDock->toggleViewAction()->setEnabled(false);
    BottomDock->toggleViewAction()->setEnabled(false);
    NavigationBar->toggleViewAction()->setEnabled(false);
    resizeUi();
}


void MainWindow::setRecentMenu() {
    // 不让 Open Recent 操作影响主进程
    try {
        // 获取 RECENT_PROJECTS 的 recentList
        std::vector<XmlRecent> recentList = XmlUtilities::instance().getRecentListFromFatherElementName(
                InitialConfig::instance().xmlPath.toStdString().c_str(),
                "RECENT_PROJECTS"
        );
        if (recentList.empty()){
            // 当 recentList 空时
            // Open Recent 不允许点击
            recentFilesMenu->setDisabled(true);
        } else {
            recentFilesMenu->setDisabled(false);
            for (const XmlRecent& recent : recentList) {
                QAction *recentFileAction = recentFilesMenu->addAction(QString::fromStdString(recent.getPath()));
                connect(recentFileAction, &QAction::triggered, [this, recent]() {
                    this->onOpenRecentTriggered(recent.getPath());
                });
            }
            // 分割线
            recentFilesMenu->addSeparator();
            clearAction = new QAction("Clear List", recentFilesMenu);
            recentFilesMenu->addAction(clearAction);
            connect(clearAction, &QAction::triggered, this, &MainWindow::onClearTriggered);
        }
    } catch (const std::exception& e) {
        // 异常
        qDebug() << "[MainWindow] An error occurred from MainWindow recentList: " << e.what();
    }
}

void MainWindow::showIPCatalog()
{
    DockManager->addDockWidgetTab(ads::RightDockWidgetArea, IPManagerWidget);
    IPManagerWidget->show();
}

void MainWindow::showPrjSummary()
{
    DockManager->addDockWidgetTab(ads::RightDockWidgetArea, PrjSummaryWidget);
    // PrjSummaryWidget->toggleView(true);
    PrjSummaryWidget->show();
}

void MainWindow::setCurrentDock(const int &type)
{
    switch (type) {
    case 0:
        EditWidget->setAsCurrentTab();
        break;
    default:
        break;
    }
}

void MainWindow::resizeUi()
{
    QFontMetrics fontMetrics(this->font());
    int width = fontMetrics.horizontalAdvance(QChar('A'))*20+80;
    float per = width*1.0/this->width();
    // qDebug() << per;
    // NavigationBar->setFixedWidth(width * 20 + 60);
    // resizeDocks({ManagerDock}, {100}, Qt::Vertical);
    int leftwidth = int(this->width()*per);
    int rightwidth = int(this->width()*((this->width()-width)*1.0/this->width()));
    // qDebug() << leftwidth << " " << rightwidth;
    // int leftwidth = int(this->width() * 0.18);//左边的停靠窗宽是主界面的0.18倍
    // int rightwidth = int(this->width() * 0.82);//右边的停靠窗宽是主界面的0.82倍
    resizeDocks({ManagerDock, BottomDock}, {42, 18}, Qt::Vertical);//右侧上下布局42 : 18
    resizeDocks({NavigationBar, BottomDock, ManagerDock},{leftwidth, rightwidth, rightwidth}, Qt::Horizontal);//左右水平布局0.18 : 0.82
}

void MainWindow::onNewTriggered()
{
    Wizard wizard(this);
    wizard.exec();
}

void MainWindow::onOpenFileTriggered()
{
    QString path = AdvancedFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");
    createEditorTab(path);
    setForm(0);
}

void MainWindow::onOpenTriggered()
{
    AdvancedFileDialog dialog(this);
    dialog.setWindowTitle("Open Project");
    dialog.setNameFilter("HybrdLink Project File (*.hpr)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QString hprPath = dialog.selectedFiles().value(0, "");
    ProjectManager::instance().openProject(hprPath);
}

void MainWindow::onSaveTriggered()
{
    EditorManager::instance()->editorSave();
}

void MainWindow::onSaveAsTriggered()
{
    EditorManager::instance()->editorSaveAs();
}

void MainWindow::onEditTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action) {
        return;
    }

    int op = -1;
    if (action == cutAction) {
        op = 0;
    } else if (action == copyAction) {
        op = 1;
    } else if (action == pasteAction) {
        op = 2;
    } else if (action == undoAction) {
        op = 3;
    } else if (action == redoAction) {
        op = 4;
    }

    EditorManager::instance()->editorEdit(op);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (EditorManager::instance()->isModified()) {
        CustomMessageBox::StandardButton btn = CustomMessageBox::showQuestion(this,
                                                                              "Warning", "There are unsaved files, are you sure you want to close?",
                                                                              QMessageBox::Yes | QMessageBox::No);
        if (btn == QMessageBox::Yes) {
            ProjectManager::instance().closeProject();
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    resizeUi();
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasTabsMenuButton, false);
    qDebug() << "[MainWindow] Constructing...";
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("HybrdLink");
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    // 设置窗口初始大小
    this->resize(1700, 960);
    // =================== MENUBAR ====================
    menuBar = new QMenuBar(this), this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("&File");
    editMenu = menuBar->addMenu("&Edit");
    viewMenu = menuBar->addMenu("&View");
    windowMenu = menuBar->addMenu("&Window");
    helpMenu = menuBar->addMenu("&Help");
    // ===================== FILE ======================
    newAction = new QAction(QIcon(":icons/resource/icons/35-icon_new_project_2.png"),"New", this), newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    openAction = new QAction("Open Project", this), openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    closeAction = new QAction("Close Project", this);
    openFileAction = new QAction("Open File", this);
    saveAction = new QAction(QIcon(":icons/resource/icons/30-icon_save.png"), "Save", this), saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveasAction = new QAction("Save As", this);
    exitAction = new QAction("Exit", this), exitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
    fileMenu->addActions({newAction, openAction, closeAction}), fileMenu->addSeparator();
    fileMenu->addActions({openFileAction});
    recentFilesMenu = fileMenu->addMenu("Open Recent");
    fileMenu->addSeparator();
    fileMenu->addActions({saveAction, saveasAction}), fileMenu->addSeparator();
    fileMenu->addActions({exitAction});
    // ===================== Open Recent ======================
    setRecentMenu();
    // ================= 文件按钮绑定 ====================
    connect(newAction, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(closeAction, &QAction::triggered, &ProjectManager::instance(), &ProjectManager::closeProject);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::onOpenFileTriggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(saveasAction, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    // ===================== EDIT ======================
    cutAction = new QAction(QIcon(":icons/resource/icons/31-icon_cut.png"), "Cut", this), cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    copyAction = new QAction(QIcon(":icons/resource/icons/14-icon_copy_2.png"),"Copy", this), copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    pasteAction = new QAction(QIcon(":icons/resource/icons/32-icon_paste_2.png"), "Paste", this), pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    undoAction = new QAction(QIcon(":icons/resource/icons/29-1icon_undo.png"), "Undo", this), undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    redoAction = new QAction(QIcon(":icons/resource/icons/29-2icon_redo.png"), "Redo", this), redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
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
    QMenu *menu = new QMenu(this);
    QToolButton *toolButton = new QToolButton;
    toolButton->setText("Actions");
    toolButton->setIcon(QIcon(":icons/resource/icons/21-icon_open_extend.png"));
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    toolButton->setStyleSheet("QToolButton::menu-indicator { image: none; }");
    toolbar->addWidget(toolButton);

    menu->addAction(openAction);
    menu->addSeparator();
    menu->addAction(openFileAction);

    // toolbar->addActions({newAction, saveAction}), toolbar->addSeparator();
    toolbar->addActions({cutAction, copyAction, pasteAction}), toolbar->addSeparator();
    toolbar->addActions({undoAction, redoAction}), toolbar->addSeparator();
    // 设置工具栏图标的大小
    toolbar->setIconSize(QSize(20, 20));
    // toolbar->addActions({chipPlannerAction});
    addToolBar(toolbar);
    // ================= EDITOR TAB ====================
    updateActionState();
    // ===================== DOCK =====================
    NavigationBar = new QDockWidget(this);
    NavigationBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    NavigationBar->setFeatures(QDockWidget::DockWidgetClosable);
    NavigationBar->setWindowTitle("FLOW NAVIGATOR");
    // FlowNavigator *flowNavigator = new FlowNavigator(this);
    NavigationBar->setWidget(FlowNavigator::instance());
    addDockWidget(Qt::LeftDockWidgetArea, NavigationBar, Qt::Vertical);

    BottomDock = new QDockWidget(this);
    BottomDock->setWindowTitle("INFORMATION");
    BottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    BottomDock->setFeatures(QDockWidget::DockWidgetClosable);
    BottomDock->setFeatures(QDockWidget::DockWidgetFloatable);
    BottomDock->setWidget(InfoWidget::instance());

    // 隐藏TitleBar
    // QWidget* lTitleBar = BottomDock->titleBarWidget();
    // QWidget* lEmptyWidget = new QWidget();
    // BottomDock->setTitleBarWidget(lEmptyWidget);
    // delete lTitleBar;
    // 垂直TitleBar
    // BottomDock->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);

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
    // SourcesWidget->setWidget(FileManager::instance());
    SourcesWidget->setWidget(new TreeViewBase(FileManager::instance()));

//    PropertiesWidget = new ads::CDockWidget("Properties", DockManager);
//    DockManager->addDockWidget(ads::BottomDockWidgetArea, PropertiesWidget,SourcesWidget->dockAreaWidget());
//    PropertiesWidget->setWidget(new QLabel("This is a test"));

    EditWidget = new ads::CDockWidget("Text Editor", DockManager);
    DockManager->addDockWidget(ads::RightDockWidgetArea, EditWidget);
    EditWidget->setWidget(EditorManager::instance());
    EditWidget->setMinimumSize(450, 10);
    // EditWidget->setMinimumSize(1300, 10);

    // ==================== Window =====================
    projectSummaryAction = new QAction(QIcon(":/icons/resource/icons/20-icon_summary_2.png") ,"Project Summary", this);
    windowMenu->addAction(projectSummaryAction);
    connect(projectSummaryAction, &QAction::triggered, this, &MainWindow::showPrjSummary);
    // ===================== VIEW ======================
    viewMenu->addAction(NavigationBar->toggleViewAction());
    viewMenu->addAction(BottomDock->toggleViewAction());
    viewMenu->addAction(ManagerDock->toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction(SourcesWidget->toggleViewAction());
//    viewMenu->addAction(PropertiesWidget->toggleViewAction());
    viewMenu->addAction(EditWidget->toggleViewAction());

    IPManagerWidget = new ads::CDockWidget("IP Catalog", DockManager);
    IPManagerWidget->hide();
    // DockManager->addDockWidgetTab(ads::RightDockWidgetArea, IPManagerWidget);
    IPManagerWidget->setWidget(IPManager::instance());

    PrjSummaryWidget = new ads::CDockWidget("Project Summary", DockManager);
    // PrjSummaryWidget->hide();
    DockManager->addDockWidgetTab(ads::RightDockWidgetArea, PrjSummaryWidget);
    PrjSummaryWidget->setWidget(PrjSummary::instance());

    PrjSummaryWidget->setMinimumSize(770, 10);
    SourcesWidget->setMinimumSize(40, 10);
}

MainWindow::~MainWindow()
{
    qDebug() << "[MainWindow] Distructing...";
}

void MainWindow::onClearTriggered() {
    // 清空 RECENT_PROJECTS 下的 recent
    XmlUtilities::instance().clearNodesFromFatherElementName(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS");
    // Open Recent 置灰
    recentFilesMenu->clear();
    recentFilesMenu->setDisabled(true);
}

void MainWindow::onOpenRecentTriggered(std::string path) {
    ProjectManager::instance().openProject(QString::fromStdString(path));
}
