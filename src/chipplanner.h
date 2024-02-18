#ifndef CHIPPLANNER_H
#define CHIPPLANNER_H

#include <QMainWindow>

namespace Ui {
class ChipPlanner;
}

class ChipPlanner : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChipPlanner(QWidget *parent = nullptr);
    ~ChipPlanner();

private:
    Ui::ChipPlanner *ui;
};

#endif // CHIPPLANNER_H
