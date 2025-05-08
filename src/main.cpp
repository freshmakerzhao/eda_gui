#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include "base/Globals.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QComboBox>
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

    //! ----- Config ----
    InitialConfig::instance().initializeApplicationConfig();
    InitialConfig::instance().initializeRoamingPath();

    //! ----- Fonts -----
    FontsUtilities::loadFont();

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
    QDialog dialog;
    dialog.resize(400, 100);
    splash.finish(&dialog);
    QVBoxLayout *vLayout = new QVBoxLayout(&dialog);
    QComboBox *comboBox = new QComboBox;
    QPushButton *button = new QPushButton("Accept");
    comboBox->addItems(QStringList() << "100t" << "160t" << "200t");
    vLayout->addWidget(comboBox);
    vLayout->addWidget(button);
    QObject::connect(button, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Rejected) {
        return 0;
    }

    std::string tileGridPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + QString("/chip_view/maps/tilegrid_%1.json").arg(comboBox->currentText()).toStdString();
    std::string tileColorPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + QString("/chip_view/maps/tile_info_map_%1.json").arg(comboBox->currentText()).toStdString();
    std::string chipPinPath = GlobalConfig::GLOBAL_RESOURCE_PATH.toStdString() + QString("/chip_view/maps/pins_info_%1.json").arg(comboBox->currentText()).toStdString();
    FrameView gridView(tileGridPath, tileColorPath, chipPinPath);
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
