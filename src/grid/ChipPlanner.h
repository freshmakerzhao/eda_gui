#ifndef CHIPPLANNER_H
#define CHIPPLANNER_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>

class ChipPlanner : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChipPlanner(QWidget *parent = nullptr);
    ~ChipPlanner();

private:
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;

    QToolBar *toolbar;
};

#endif // CHIPPLANNER_H
