/**
  ******************************************************************************
  * @file           : BitstreamSettingPage.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/19
  ******************************************************************************
  */
#ifndef HYBRDLINK_BITSTREAMSETTINGPAGE_H
#define HYBRDLINK_BITSTREAMSETTINGPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QAction>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

class BitstreamSettingPage : public QWidget {

public:
    BitstreamSettingPage(QWidget *parent = nullptr);

    // 应用设置
    void applySettings();
    // 初始化复选框状态
    void loadSettings();

private:
    QCheckBox *generate_rbt_file_checkBox;
    QCheckBox *generate_bin_file_checkBox;
    QCheckBox *enable_crc_option;
    QCheckBox *enable_compress_option;
};

#endif //HYBRDLINK_BITSTREAMSETTINGPAGE_H
