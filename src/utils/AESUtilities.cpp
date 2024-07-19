/**
  ******************************************************************************
  * @file           : AESUtilities.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/7/3
  ******************************************************************************
  */
#include <QDebug>
#include <fstream>
#include <iostream>
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

/**
 * AES CBC 解密函数
 * @param encrypt_file_path 待解密文件路径
 * @return
 */
unsigned char *AESUtilities::aesDecrypt(const std::string &encrypt_file_path)
{
    unsigned char key_hex[16];
    generateAESKey(KEY_STR, key_hex, 16);

    AES_KEY de_key;
    // ============= 设置 AES 解密密钥 ==================
    if (AES_set_decrypt_key(key_hex, 128, &de_key) < 0) {
        //        qDebug("[AESUtilities] set decrypt key failed ! ");
        std::cerr << "[AESUtilities] set decrypt key failed!" << std::endl;
        return nullptr;
    }
    // 从文件中以二进制模式读取密文
    std::ifstream encrypt_file(encrypt_file_path, std::ios::binary);
    if (!encrypt_file) {
        //        qDebug("[AESUtilities] encrypt_file_path open failed ! ");
        std::cerr << "[AESUtilities] encrypt_file_path open failed ! " << std::endl;
        return nullptr;
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
