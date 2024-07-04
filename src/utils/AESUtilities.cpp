/**
  ******************************************************************************
  * @file           : AESUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 3/7/2024
  ******************************************************************************
  */
#include <QDebug>
#include <fstream>
#include "AESUtilities.h"
#include "aes.h"
#include "sha.h"

const std::string KEY_STR = "zhaoshuaikey0704";  // 16字节的字符串做密钥

// 16 字节的初始向量（IV）
const unsigned char IV_STR[17] = "abcdef9876543210";

// 生成AES密钥
void generateAESKey(const std::string& input_key, unsigned char* aes_key, int key_len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input_key.c_str()), input_key.size(), hash);
    memcpy(aes_key, hash, key_len);
}

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
 * @param iv  初始向量,与明文块进行异或运算，然后再加密
 * @param encrypt_text 加密后结果
 */
void AESUtilities::aesEncrypt(
        const std::string& plain_text,
        const std::string& encrypt_file_path){

    unsigned int plain_text_len = plain_text.size();
    unsigned char key_hex[16];
    generateAESKey(KEY_STR, key_hex, 16);
    AES_KEY en_key;
    // ============= 设置 AES 加密密钥 ==================
    if(AES_set_encrypt_key(key_hex,128,&en_key) < 0){
        // 密钥设置失败
        qDebug("[AESUtilities] set encrypt key failed ! ");
    }

    // 计算填充长度
    int padding_len = AES_BLOCK_SIZE - (plain_text_len % AES_BLOCK_SIZE);
    int total_len = plain_text_len + padding_len;

    unsigned char* padded_plain_text = (unsigned char*)malloc(total_len);
    // 使用 memcpy 复制 plain_text 到 padded_plain_text
    memcpy(padded_plain_text, plain_text.c_str(), plain_text_len);
    memset(padded_plain_text + plain_text_len, padding_len, padding_len);

    unsigned char iv_tmp[17];
    memcpy(iv_tmp, IV_STR, 17);

    unsigned char* encrypt_text = (unsigned char*)malloc(total_len);
    AES_cbc_encrypt(padded_plain_text,encrypt_text,total_len,&en_key,iv_tmp,AES_ENCRYPT);
    free(padded_plain_text);

    //    qDebug() << "加密后的密文: ";
    //    printHex(encrypt_text, total_len);
    // 加密数据写入文件
    if (!encrypt_file_path.empty()) {
        std::ofstream outfile(encrypt_file_path, std::ios::binary);
        if (!outfile) {
            qDebug("[AESUtilities] encrypt_file_path open failed ! ");
        } else {
            outfile.write(reinterpret_cast<char*>(encrypt_text), total_len);
            outfile.close();
        }
    }
    free(encrypt_text);
}

/**
 * AES CBC 解密函数
 * @param key_hex
 * @param iv
 * @param encrypt_file_path
 * @return
 */
unsigned char * AESUtilities::aesDecrypt(
        const std::string &encrypt_file_path) {

    unsigned char key_hex[16];
    generateAESKey(KEY_STR, key_hex, 16);

    AES_KEY de_key;
    // ============= 设置 AES 解密密钥 ==================
    if (AES_set_decrypt_key(key_hex, 128, &de_key) < 0) {
        qDebug("[AESUtilities] set decrypt key failed ! ");
    }
    // 从文件中以二进制模式读取密文
    std::ifstream encrypt_file(encrypt_file_path, std::ios::binary);
    if (!encrypt_file) {
        qDebug("[AESUtilities] encrypt_file_path open failed ! ");
    }

    // 读取文件内容到 vector 中
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(encrypt_file)), std::istreambuf_iterator<char>());
    encrypt_file.close();

    unsigned char iv_tmp[17];
    memcpy(iv_tmp, IV_STR, 17);

    unsigned char* decrypt_text = (unsigned char*)malloc(buffer.size());
    AES_cbc_encrypt(buffer.data(), decrypt_text, buffer.size(), &de_key, iv_tmp, AES_DECRYPT);

    // 获取填充长度
    int padding_len = decrypt_text[buffer.size() - 1];
    int plain_text_len = buffer.size() - padding_len;

    decrypt_text[plain_text_len] = '\0'; // 添加 null 终止符
    return decrypt_text;
}