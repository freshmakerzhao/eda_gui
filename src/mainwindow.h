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

#include "chipplanner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();

    void updateActionState();                 // 更新编辑器按钮状态
    void createEditorTab(const QString& path); // 创建编辑器Tab

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

private:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
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

    ChipPlanner chipPlanner;

    void streamProcessOutput();
    // 配置输出和完成信号槽
    void configOutputSignals(const QString &phase);
};

#endif // MAINWINDOW_H
