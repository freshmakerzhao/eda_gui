#include "mainwindow.h"

#include "navigator.h"
#include "project.h"
#include <QApplication>
#include <QDebug>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 启动画面
    QPixmap pix(":/resource/logo.png");
    QSplashScreen splash(pix.scaled(500, 300));
    splash.show();
    a.processEvents();

    // 获取命令行参数列表
    QStringList args = a.arguments();
    splash.finish(MainWindow::instance()); // 启动画面结束
    MainWindow::instance()->show();
    MainWindow::instance()->setWindowTitle("HybrdLink");
    // 如果有传递文件作为命令行参数
    if(args.size() > 1) {
        // 获取第一个文件路径
        QString hprfile = args.at(1);
        qDebug() << "Loading Project:" << hprfile;
        Project *proj = new Project;
        proj->openProject(hprfile);
        Navigator::instance()->loadFile(proj);
    }

    return a.exec();
}
