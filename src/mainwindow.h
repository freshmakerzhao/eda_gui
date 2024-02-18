#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>

#include "projectnavigator.h"
#include "taskview.h"
#include "infowidget.h"
#include "chipplanner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void receiveFilePath(const QString &path);

private slots:
    void onNewTriggered();
    void onOpenTriggered();
    void onOpenProjectTriggered();
    void onSaveTriggered();

    void onCutTriggered();
    void onCopyTriggered();
    void onPasteTriggered();
    void onUndoTriggered();
    void onRedoTriggered();

    void onChipPlannerTriggered();

    void onTabWidgetTabCloseRequested(int index);

private:
    void createEditorTab(const QString& path);
    void refreshActionState();

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *openProjectAction;
    QAction *saveAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *chipPlannerAction;

    QToolBar *toolbar;
    QTabWidget *tabWidget;

    TaskView *taskView;
    ProjectNavigator *projectNavigator;
    InfoWidget *infoWidget;
    ChipPlanner chipPlanner;
};
#endif // MAINWINDOW_H
