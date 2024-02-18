#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "taskview.h"
#include "projectnavigator.h"
#include "chipplanner.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_Open_triggered();

    void on_Save_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_New_triggered();

    void on_Open_Project_triggered();

    void on_Chip_Planner_triggered();

    void on_Cut_triggered();

    void on_Copy_triggered();

    void on_Paste_triggered();

    void on_Undo_triggered();

    void on_Redo_triggered();

// protected:
//     void closeEvent(QCloseEvent *event) override;

private:
    void createEditorTab(QString fileName);

    void refreshActionState();

    TaskView *taskView;
    ProjectNavigator *projectNavigator;
    ChipPlanner chipPlanner;

    Ui::MainWindow *ui;

public slots:
    void receiveFilePath(const QString &path);
};
#endif // MAINWINDOW_H
