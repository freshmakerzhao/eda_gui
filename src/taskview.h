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

//    QStringList sourceList;
    // 存储设计与约束文件
    QList<QString> sourcePathList;
    QList<QString> constraintPathList;
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    TaskView(QWidget *parent = nullptr);
    ~TaskView();

    QTreeWidget *taskTree;

    void runSynth();
    void buildPack();
    void buildPlace(int mode);
    void buildRoute();
    void buildBit(int mode);
    void downloadBit();

};

#endif // TASKVIEW_H
