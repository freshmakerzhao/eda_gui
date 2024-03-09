#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QTreeWidget>
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>


class TaskView : public QWidget
{
    Q_OBJECT

public:
    static TaskView *instance(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    TaskView(QWidget *parent = nullptr);
    ~TaskView();

    QTreeWidget *taskTree;

};

#endif // TASKVIEW_H
