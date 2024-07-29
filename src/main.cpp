#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>
#include <utils/FontsUtilities.h>
#include "utils/LicenseUtilities.h"

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
    QString appDirPath = QCoreApplication::applicationDirPath();
    // 加载字体并获取字体名
    QString fontName = FontsUtilities::instance().loadFont(appDirPath);
    if (fontName.isEmpty()) {
        qCritical() << "Failed to load any font. Exiting.";
        fontName = a.font().family();
    }
    QFont font(fontName, 9);
    a.setFont(font);
    QFontDatabase::addApplicationFont(":/resource/fonts/LFTEtica/no-liga-LFTEticaMono-Regular-OK.ttf");// LFT Etica Mono

#if defined(ENABLE_MAC_CHECK) || defined(ENABLE_EXPIRATION_CHECK)
    if (!LicenseUtilities::instance()->checkLicense()) {
        return -1;  // License check failed or user rejected dialog
    }
#endif
    InitialConfig::instance().initializeApplicationConfig();
    InitialConfig::instance().initializeRoamingPath();

    // 启动画面
    QPixmap pix(":/resource/start.jpg");

    QSplashScreen splash(pix.scaled(640, 640, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash.show();
    splash.showMessage("Loading...");
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
