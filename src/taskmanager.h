#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QTreeWidget>
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>


class TaskManager : public QWidget
{
    Q_OBJECT

public:
    static TaskManager *instance(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    TaskManager(QWidget *parent = nullptr);
    ~TaskManager();

    QTreeWidget *taskTree;

};

#endif // TASKMANAGER_H
