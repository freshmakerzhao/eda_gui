#include "mainwindow.h"

#include "utils/ProjectManager.h"
#include <QApplication>
#include <QSplashScreen>
#include "widgets/Form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 启动画面
    QPixmap pix(":/resource/logo.png");
    QSplashScreen splash(pix.scaled(700, 496, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    splash.show();
    a.processEvents();
    splash.finish(MainWindow::instance()); // 启动画面结束
    MainWindow::instance()->setCentralWidget(Form::instance());
    MainWindow::instance()->show();
    MainWindow::instance()->setForm(1);
    // 获取命令行参数列表
    QStringList args = a.arguments();
    // 如果有传递文件作为命令行参数
    ProjectManager::instance().loadProject(args);

    return a.exec();
}
