/**
  ******************************************************************************
  * @file           : BitstreamSettingsPage.cpp
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/12/19
  ******************************************************************************
  */
#include <QLabel>
#include "BitstreamSettingsPage.h"
#include "utils/ProjectManager.h"
#include "wizard/Wizard.h"

BitstreamSettingsPage::BitstreamSettingsPage(QWidget *parent)
        : QWidget(parent)
{
    setObjectName("BitstreamSettingsPage");

    // 顶部说明文字
    QLabel *header_label = new QLabel("<b>Write Bitstream</b><br> Specify various settings related to writing Bitstream. ");
    header_label->setWordWrap(true);

    header_label->setObjectName("SettingsPageTitleLabel");

    // 创建一个表单布局，用于组织字段
    QFormLayout *formLayout = new QFormLayout;

    // 添加可选项的复选框
    generate_rbt_file_checkBox = new QCheckBox("Generate rbt file");
    generate_rbt_file_checkBox->setStyleSheet("padding-left: 30px;"); // 增加缩进
    formLayout->addRow(generate_rbt_file_checkBox);

    generate_bin_file_checkBox = new QCheckBox("Generate bin file");
    generate_bin_file_checkBox->setStyleSheet("padding-left: 30px;"); // 增加缩进
    formLayout->addRow(generate_bin_file_checkBox);

    enable_crc_option = new QCheckBox("Enable crc option");
    enable_crc_option->setStyleSheet("padding-left: 30px;"); // 增加缩进
    formLayout->addRow(enable_crc_option);

    enable_compress_option = new QCheckBox("Enable compress option");
    enable_compress_option->setStyleSheet("padding-left: 30px;"); // 增加缩进
    formLayout->addRow(enable_compress_option);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(header_label);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();

    // 设置布局
    setLayout(mainLayout);
}

void BitstreamSettingsPage::applySettings() {
    // 更新 Project 参数
    ProjectManager::instance().updateBinFileOption(generate_bin_file_checkBox->isChecked() ? "enable" : "disable");
    ProjectManager::instance().updateRbtFileOption(generate_rbt_file_checkBox->isChecked() ? "enable" : "disable");
    ProjectManager::instance().updateCRCOption(enable_crc_option->isChecked() ? "enable" : "disable");
    ProjectManager::instance().updateCompressOption(enable_compress_option->isChecked() ? "enable" : "disable");
}

void BitstreamSettingsPage::loadSettings()
{
    // 设置复选框状态，根据参数值是否为 "enable"
    generate_rbt_file_checkBox->setChecked(ProjectManager::instance().getParameter(Project::RbtFile) == "enable");
    generate_bin_file_checkBox->setChecked(ProjectManager::instance().getParameter(Project::BinFile) == "enable");
    enable_crc_option->setChecked(ProjectManager::instance().getParameter(Project::CRCOption) == "enable");
    enable_compress_option->setChecked(ProjectManager::instance().getParameter(Project::CompressOption) == "enable");
}
