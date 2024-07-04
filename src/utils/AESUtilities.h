/**
  ******************************************************************************
  * @file           : AESUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 3/7/2024
  ******************************************************************************
  */
#ifndef HYBRDLINK_AESUTILITIES_H
#define HYBRDLINK_AESUTILITIES_H


#include <string>

class AESUtilities {

public:
    static void aesEncrypt(
            const std::string& plain_text,
            const std::string& encrypt_file_path);

    static unsigned char* aesDecrypt(const std::string& encrypt_file_path);
};


#endif //HYBRDLINK_AESUTILITIES_H
