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
    // 项目路径
    QString projectPath;
    // 综合路径
    QString projectSynthPath;
    // implement路径
    QString projectImplPath;
    // part name
    QString partName;
    // arch name
    QString archName;
    // arch name
    QString GLOBAL_RESOURCE_PATH;
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
    void buildImp();
    void buildBit(int mode);
    void downloadBit();

};

#endif // TASKVIEW_H
