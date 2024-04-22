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

#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QTextCodec>
#include <QCoreApplication>
#include <QSharedPointer>

// #include "grid/ChipPlanner.h"
#include "ads/DockManager.h"
#include "ads/DockWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance(const QString &path = nullptr);

    void updateActionState();                 // 更新编辑器按钮状态
    void createEditorTab(const QString& path); // 创建编辑器Tab
    bool cleanEditorTab();
    bool saveAllFile();

    void showProjectTitle(int mode = 0, const QString &title = "");

private slots:
    void onNewTriggered();
    void onOpenFileTriggered();
    void onOpenTriggered();
    void onSaveTriggered();
    void onSaveAsTriggered();
    void onEditTriggered();


    void onChipPlannerTriggered();

    void onDocumentationTriggered();
    void onAboutTriggered();

    void onTabWidgetCurrentChanged(int index);
    void onTabWidgetTabCloseRequested(int index);

protected:
    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    MainWindow(QWidget *parent = nullptr,const QString &path = nullptr);
    ~MainWindow();

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

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

    QAction *documentation;
    QAction *aboutAction;

    QAction *chipPlannerAction;

    QToolBar *toolbar;
    QTabWidget *tabWidget;

    // ChipPlanner chipPlanner;

    QDockWidget *NavigationBar;
    QDockWidget *BottomDock;
    QDockWidget *ManagerDock;

    ads::CDockManager *DockManager;

    ads::CDockWidget *SourcesWidget;
    ads::CDockWidget *EditWidget;
    // 工程文件路径
    QString hprPath = nullptr;
};

#endif // MAINWINDOW_H
