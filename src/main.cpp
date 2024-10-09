#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include "base/Globals.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>
#include <utils/FontsUtilities.h>
#include "utils/LicenseUtilities.h"
#include "utils/ProcessManager.h"
#include "widgets/FrameView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //! ------ QSS ------
    QFile qss(":/resource/Global.qss");
    if (qss.open(QFile::ReadOnly)) {
        a.setStyleSheet(qss.readAll());
    }
    qss.close();

    //! ----- Fonts -----
    FontsUtilities::loadFont();

    //! ----- Config ----
    InitialConfig::instance().initializeApplicationConfig();
    InitialConfig::instance().initializeRoamingPath();

#if defined(ENABLE_MAC_CHECK) || defined(ENABLE_EXPIRATION_CHECK)
    if (!LicenseUtilities::checkLicense()) {
        return -1;  // License check failed or user rejected dialog
    }
#endif

    ProcessManager::instance().initEnvironment();

    // 启动画面
    QPixmap pix(":/resource/start.jpg");
    QSplashScreen splash(pix.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash.show();
    splash.showMessage("Loading...");
    a.processEvents();

#if ONLY_COMPILE_GRIDVIEW
    std::string tileGridPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tilegrid_100t.json";
    std::string tileColorPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + "/chip_view/maps/tile_info_map.json";
    FrameView gridView(tileGridPath,tileColorPath);
    gridView.resize(1600, 1000);
    splash.finish(&gridView);
    gridView.show();
#else
    splash.finish(MainWindow::instance()); // 启动画面结束
    MainWindow::instance()->setCentralWidget(Form::instance());
    MainWindow::instance()->show();
    MainWindow::instance()->setForm(1);
    // 获取命令行参数列表
    QStringList args = a.arguments();
    // 如果有传递文件作为命令行参数
    ProjectManager::instance().openProjectFromArgs(args);
#endif
    return a.exec();
}
