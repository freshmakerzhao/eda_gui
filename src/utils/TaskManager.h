#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QTreeWidget>
#include <QDebug>

class TaskManager : public QObject
{
    Q_OBJECT
public:
    static TaskManager& instance();

    void handleTreeItemActivation(QTreeWidgetItem *item);

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
    // arch name xc7a50t
    QString archName;
    // arch 50t
    QString arch;
    // arch name
    QString GLOBAL_RESOURCE_PATH;

private:
    TaskManager();
    ~TaskManager();

    QTreeWidget *taskTree;

    void runSynth();
    void buildPack();
    void buildPlace(int mode);
    void buildRoute();
    void buildImp();
    void buildBit(int mode);
    void downloadBit();
    QWidget* gridView = nullptr;

};

#endif // TASKMANAGER_H
