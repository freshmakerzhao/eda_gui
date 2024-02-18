#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QWidget>
#include <QGridLayout>


class TaskView : public QWidget
{
    Q_OBJECT

public:
    explicit TaskView(QWidget *parent = nullptr);
    ~TaskView();

private:

};

#endif // TASKVIEW_H
