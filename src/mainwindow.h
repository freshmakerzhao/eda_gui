/**
  ******************************************************************************
  * @file           : mainwindow.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/9
  ******************************************************************************
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QTextCodec>
#include <QCoreApplication>
#include <QToolButton>
#include <QMovie>
#include <QLabel>
#include <QDesktopServices>
// #include "grid/ChipPlanner.h"
#include "ads/DockManager.h"
#include "ads/DockWidget.h"
#include "entity/XmlRecent.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();

    void updateActionState();                 // 更新编辑器按钮状态
    void createEditorTab(const QString& path); // 创建编辑器Tab
    bool cleanEditorTab();
    bool saveAllFile();

    void showProjectTitle(const int &mode = 0, const QString &title = "");
    void setForm(const int &mode = 0);
    void setRecentMenu();
    void showIPCatalog();
    void showPrjSummary();
    void showProperties();
    void setCurrentDock(const int &type); // 跳转到对应的DockWidget

    void resizeUi();

    void setRunState(const QString &phase, const bool &flag);
    void resetRunState();

    void resetUi();

public slots:
    void onNewTriggered();
    void onOpenFileTriggered();
    void onOpenTriggered();
    void onOpenRecentTriggered(std::string path);
    void onClearTriggered();
    void onSaveTriggered();
    void onSaveAsTriggered();
    void onEditTriggered();

    void onChipPlannerTriggered();

    void onDocumentationTriggered();
    void onAboutTriggered();

protected:
    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMenuStateBar();

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // Open Recent 子菜单
    QMenu *recentFilesMenu;
    QAction *clearAction;

    QAction *newAction;
    QAction *openAction;
    QAction *closeAction;
    QAction *openFileAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *projectSummaryAction;

    QAction *documentation;
    QAction *aboutAction;

    QAction *chipPlannerAction;

    QToolBar *toolbar;

    // ChipPlanner chipPlanner;

    QDockWidget *NavigationBar;
    QDockWidget *BottomDock;
    QDockWidget *ManagerDock;

    ads::CDockManager *DockManager;

    ads::CDockWidget *SourcesWidget;
    ads::CDockWidget *EditWidget;
    ads::CDockWidget *PropertiesWidget;
    ads::CDockWidget *IPManagerWidget;
    ads::CDockWidget *PrjSummaryWidget;

    QLabel *phaseLabel;
    QMovie *movie;
    QLabel *movieLabel;
    QImage *completeImage;
    QImage *errorImage;
};

#endif // MAINWINDOW_H
