#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include "utils/LicenseUtilities.h"
#include "utils/AESUtilities.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QDateTime>

const int VALID_DAYS = 7;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const std::string plain_text = "z22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222hz22222222222222222222222h";

    AESUtilities::aesEncrypt(plain_text, "E:/workspace/eda_gui/src/zhaoshuai.bin");

    unsigned char* decrypt_text = AESUtilities::aesDecrypt("E:/workspace/eda_gui/src/zhaoshuai.bin");

    // 显示结果
    qDebug() << "原始字符串: " << QString::fromStdString(plain_text);
    qDebug() << "解密后的字符串:";
    qDebug() << (char*)decrypt_text;

#ifdef ENABLE_MAC_CHECK
    qDebug() << "[MAIN] MAC address check is enabled.";
    // 比对MAC地址
    if (!LicenseUtilities::checkMacAddress(EXPECTED_MAC_ADDRESS)) {
        return -1;
    }
#else
    qDebug() << "[MAIN] MAC address check is disabled.";
#endif


#ifdef ENABLE_EXPIRATION_CHECK
    qDebug() << "[MAIN] Software expiration check is enabled.";
    QString buildDateTime = QString("%1 %2").arg(__DATE__, __TIME__); // Jul  xx 20xx xx:xx:xx

    qDebug() << "[MAIN] Current build data is ." << buildDateTime;
    // 检查软件的剩余有效天数
    int remainingDays = LicenseUtilities::isWithinValidPeriod(buildDateTime, VALID_DAYS);

    if (remainingDays == 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Error: Software has expired.");
        msgBox.setInformativeText("This software is no longer valid. Please contact support.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return -1;
    } else {
        qDebug() << "[MAIN] Software is valid for" << remainingDays << "more days.";
        // 计算有效期的截止时间并写入文件
        QDateTime compileDateTime = QLocale::c().toDateTime(buildDateTime, "MMM  d yyyy HH:mm:ss");
        QDateTime expiryDateTime = compileDateTime.addDays(VALID_DAYS);
        LicenseUtilities::writeExpiryDateToFile(expiryDateTime.toString());
    }
#else
    qDebug() << "[MAIN] Software expiration check is disabled.";
#endif

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
    QPixmap pix(":/resource/start05.jpg");

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
