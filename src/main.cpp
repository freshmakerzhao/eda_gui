#include "mainwindow.h"

#include "navigator.h"
// #include "taskview.h"
// #include "infowidget.h"
#include "project.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 获取命令行参数列表
    QStringList args = a.arguments();
    MainWindow::instance()->show();
    // 如果有传递文件作为命令行参数
    if(args.size() > 1) {
        // 获取第一个文件路径
        QString path = args.at(1);
        qDebug() << "Loading Project:" << path;
        Project *proj = new Project;
        proj->openProject(path);
        Navigator::instance()->loadFile(proj);
    }

    return a.exec();
}
