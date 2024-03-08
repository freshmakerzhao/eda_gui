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

    QStringList sourceList;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTreeWidget *taskTree;

    void runSynth();
    void buildPack();
    void buildPlace(int mode);
    void buildRoute();
    void buildBit(int mode);
    void downloadBit();

};

#endif // TASKVIEW_H
