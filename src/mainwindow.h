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

#include "chipplanner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateActionState();                 // 更新编辑器按钮状态
    void createEditorTab(const QString& path); // 创建编辑器Tab

private slots:
    void onNewTriggered();
    void onOpenTriggered();
    void onOpenProjectTriggered();
    void onSaveTriggered();
    void onSaveAsTriggered();

    void onCutTriggered();
    void onCopyTriggered();
    void onPasteTriggered();
    void onUndoTriggered();
    void onRedoTriggered();

    void onChipPlannerTriggered();

    void onTabWidgetCurrentChanged(int index);
    void onTabWidgetTabCloseRequested(int index);

private:

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *openProjectAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *chipPlannerAction;

    QToolBar *toolbar;
    QTabWidget *tabWidget;

    ChipPlanner chipPlanner;

    void streamProcessOutput();

    // 配置输出和完成信号槽
    void configOutputSignals(const QString &phase);

protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // MAINWINDOW_H
