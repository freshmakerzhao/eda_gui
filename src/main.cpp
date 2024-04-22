#include "mainwindow.h"

#include "utils/ProjectManager.h"
#include <QApplication>
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
    // 当传入参数大于1，args.at(1)为工程文件路径
    splash.finish(MainWindow::instance( args.size() > 1 ? args.at(1) : nullptr)); // 启动画面结束
    MainWindow::instance()->show();
    // 如果有传递文件作为命令行参数
    ProjectManager::instance().loadProject(args);

    return a.exec();
}
