#include "mainwindow.h"
#include "utils/ProjectManager.h"
#include "widgets/Form.h"
#include "base/InitialConfig.h"
#include "utils/LicenseUtilities.h"
#include "base/Globals.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QDateTime>
#include <QSslSocket>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <fstream>
#include <iomanip>

const int VALID_DAYS = 7;
static unsigned char key[AES_BLOCK_SIZE*2] = "1wradfr4e3fefefad4545454h6thrsf";   //秘钥

void printHex(const unsigned char* data, int length) {
    QString hexString;
    for (int i = 0; i < length; ++i) {
        hexString.append(QString("%1 ").arg(data[i], 2, 16, QChar('0')).toUpper());
    }
    qDebug() << hexString;
}

/**
 * AES CBC 加密函数
 * @param plain_text 待加密数据
 * @param plain_text_len 待加密数据长度,字符长度
 * @param key_hex char 数组，16个元素，元素宽度是 8位，共128位
 * @param iv  初始向量,与明文块进行异或运算，然后再加密
 * @param encrypt_text 加密后结果
 */
void aesEncrypt(
        const unsigned char* plain_text,
        int plain_text_len,
        const unsigned char* key_hex,
        const unsigned char* iv,
        unsigned char* encrypt_text,
        const std::string& encrypt_file_path){
    AES_KEY en_key;
    // ============= 设置 AES 加密密钥 ==================
    if(AES_set_encrypt_key(key_hex,128,&en_key) < 0){
        // 密钥设置失败
        qDebug("[MAIN] set encrypt key failed ! %d \r\n",__LINE__);
    }

    // 计算填充长度
    int padding_len = AES_BLOCK_SIZE - (plain_text_len % AES_BLOCK_SIZE);
    int total_len = plain_text_len + padding_len;

    unsigned char* padded_plain_text = (unsigned char*)malloc(total_len);
    // 使用 memcpy 复制 plain_text 到 padded_plain_text
    memcpy(padded_plain_text, plain_text, plain_text_len);
    memset(padded_plain_text + plain_text_len, padding_len, padding_len);

    unsigned char iv_tmp[17];
    memcpy(iv_tmp, iv, 17);

    AES_cbc_encrypt(padded_plain_text,encrypt_text,total_len,&en_key,iv_tmp,AES_ENCRYPT);

    free(padded_plain_text);
    // 如果提供了文件路径，则将加密数据写入文件
    if (!encrypt_file_path.empty()) {
        std::ofstream outfile(encrypt_file_path, std::ios::binary);
        if (!outfile) {
            qDebug("[MAIN] encrypt_file_path open failed ! %d \r\n");
        } else {
            outfile.write(reinterpret_cast<char*>(encrypt_text), total_len);
            outfile.close();
        }
    }
}

/**
 * AES CBC 解密函数
 * @param encrypt_text 加密后文本
 * @param plain_text_len 原始数据长度
 * @param key
 * @param iv
 * @param decrypt_text
 */
void aesDecrypt(const unsigned char* encrypt_text, int plain_text_len, const unsigned char* key_hex, const unsigned char* iv, unsigned char* decrypt_text) {
    AES_KEY de_key;
    unsigned char iv_dec[17];
    // ============= 重置初始向量 ==================
    memcpy(iv_dec, iv, 17);
    // ============= 设置 AES 解密密钥 ==================
    if (AES_set_decrypt_key(key_hex, 128, &de_key) < 0) {
        qDebug("[MAIN] set decrypt key fail! %d \r\n",__LINE__);
    }
    // 计算填充长度
    int padding_len = AES_BLOCK_SIZE - (plain_text_len % AES_BLOCK_SIZE);
    int total_len = plain_text_len + padding_len;
    AES_cbc_encrypt(encrypt_text, decrypt_text, total_len, &de_key, iv_dec, AES_DECRYPT);

    // 移除填充
    int padding = decrypt_text[total_len - 1];
    int decrypted_len = total_len - padding;
    decrypt_text[decrypted_len] = '\0'; // 添加 null 终止符
}

void aesDecrypt2(const unsigned char* encrypt_text, int encrypt_text_len, const unsigned char* key_hex, const unsigned char* iv, unsigned char* decrypt_text) {
    AES_KEY de_key;
    unsigned char iv_dec[17];
    // ============= 重置初始向量 ==================
    memcpy(iv_dec, iv, 17);
    // ============= 设置 AES 解密密钥 ==================
    if (AES_set_decrypt_key(key_hex, 128, &de_key) < 0) {
        qDebug("[MAIN] set decrypt key fail! %d \r\n",__LINE__);
    }
    AES_cbc_encrypt(encrypt_text, decrypt_text, encrypt_text_len, &de_key, iv_dec, AES_DECRYPT);
    decrypt_text[encrypt_text_len] = '\0'; // 添加 null 终止符
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 16 字节的 AES 密钥。
    const unsigned char key_hex[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    // 16 字节的初始向量（IV）
    unsigned char iv_hex[17] = {'s','i','n','g','u','l','a','r','-','m','e','d','i','c','a','l'};
    const char* plain_text = "zhaoshuai";
    int plain_text_len = strlen(plain_text);
    // 计算填充长度
    int padding_len = AES_BLOCK_SIZE - (plain_text_len % AES_BLOCK_SIZE);
    int total_len = plain_text_len + padding_len;

    unsigned char padded_plain_text[256];
    strcpy((char *)padded_plain_text, plain_text);
    memset(padded_plain_text + plain_text_len, padding_len, padding_len);

    unsigned char* encrypt_text = (unsigned char*)malloc(total_len);
    unsigned char* decrypt_text = (unsigned char*)malloc(total_len);
    aesEncrypt((unsigned char*)plain_text, plain_text_len, key_hex, iv_hex, encrypt_text);

    // 将密文写入文件
    std::ofstream outfile("E:/workspace/eda_gui/src/testAES222.bin", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write((char*)encrypt_text, total_len);
        outfile.close();
    } else {
        qDebug() << "无法打开文件进行写操作";
        return 1;
    }

    // 从文件中以二进制模式读取密文
    QByteArray fileData;
    QFile infile("E:/workspace/eda_gui/src/testAES333.bin");
    if (infile.open(QIODevice::ReadOnly)) {
        fileData = infile.readAll();
        infile.close();
    } else {
        qDebug() << "无法打开文件进行读操作";
        return 1;
    }

    // 打印读取的内容
    qDebug() << "Encrypted data (hex):" << fileData.toHex();
    // 从文件中以二进制模式读取密文
    std::ifstream infilede("E:/workspace/eda_gui/src/testAES333.bin", std::ios::binary);
    if (!infilede) {
        std::cerr << "无法打开文件进行读操作" << std::endl;
        return 1;
    }

    // 读取文件内容到 vector 中
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(infilede)), std::istreambuf_iterator<char>());
    infilede.close();
    // 输出读取的内容的长度
    std::cout << "Read encrypted data length: " << buffer.size() << " bytes" << std::endl;

    // 打印读取的内容（以十六进制表示）
    std::cout << "Encrypted data (hex): ";
    for (unsigned char byte : buffer) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    aesDecrypt(encrypt_text, plain_text_len, key_hex, iv_hex, decrypt_text);
    unsigned char* decrypt_text2 = (unsigned char*)malloc(buffer.size());
    aesDecrypt2(buffer.data(), buffer.size(), key_hex, iv_hex, decrypt_text2);

    // 显示结果
    qDebug() << "原始字符串: " << plain_text;
    qDebug() << "加密后的密文: ";
    printHex(encrypt_text, total_len);
    qDebug() << "解密后的字符串:";
    qDebug() << (char*)decrypt_text;
    qDebug() << (char*)decrypt_text2;

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
