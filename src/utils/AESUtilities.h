/**
  ******************************************************************************
  * @file           : AESUtilities.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/7/3
  ******************************************************************************
  */
#ifndef HYBRDLINK_AESUTILITIES_H
#define HYBRDLINK_AESUTILITIES_H


#include <string>

class AESUtilities {

public:
    static unsigned char *aesDecrypt(const std::string &encrypt_file_path);
};


#endif //HYBRDLINK_AESUTILITIES_H
