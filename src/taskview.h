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

    explicit TaskView(QWidget *parent = nullptr);
    ~TaskView();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTreeWidget *navTree;

};

#endif // TASKVIEW_H
