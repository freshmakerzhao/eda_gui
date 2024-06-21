#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载字体文件
//    QFontDatabase::addApplicationFont(":/resource/fonts/LFTEtica/LFTEticaMono-Regular.TTF");// LFT Etica Mono
    QFontDatabase::addApplicationFont(":/resource/fonts/LFTEtica/no-liga-LFTEticaMono-Regular-OK.ttf");// LFT Etica Mono
    QFontDatabase::addApplicationFont(":/resource/fonts/AlibabaPuHuiTi3.0-55Regular.ttf");

    QString fontName = "Alibaba PuHuiTi 3.0";
    QFontDatabase database;
    QStringList fontFamilies = database.families();
    if (!fontFamilies.contains(fontName)) {
        fontName = "Microsoft YaHei UI";
    }
    QFont font(fontName, 9);
    a.setFont(font);

    InitialConfig::instance().initializeApplicationConfig();
    InitialConfig::instance().initializeRoamingPath();

    // 启动画面
//    QPixmap pix(":/resource/logo.png");
//    QPixmap pix(":/resource/start01.jpg");
    QPixmap pix(":/resource/start03.jpg");

//    QSplashScreen splash(pix.scaled(440, 440, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSplashScreen splash(pix.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash.show();
    a.processEvents();
    splash.finish(MainWindow::instance()); // 启动画面结束
    MainWindow::instance()->setCentralWidget(Form::instance());
    MainWindow::instance()->show();
    MainWindow::instance()->setForm(1);
    // 获取命令行参数列表
    QStringList args = a.arguments();
    // 如果有传递文件作为命令行参数
    ProjectManager::instance().openProjectFromArgs(args);

    return a.exec();
}
