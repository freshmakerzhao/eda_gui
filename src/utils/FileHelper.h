/**
  ******************************************************************************
  * @File           : FileHelper.h
  * @Author         : zs
  * @Description    : 与文件相关的QT工具类
  * @Date           : 23-11-9
  ******************************************************************************
  */

#ifndef GRID_VIEW_FILEHELPER_H
#define GRID_VIEW_FILEHELPER_H

#include <QString>
#include <QWidget>
#include <QFileDialog>

class FileHelper {
public:
    static QString addJsonFile(QWidget* parent = nullptr,const QString& GLOBAL_RESOURCE_PATH = "") {
        QFileDialog fileDialog;
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        fileDialog.setNameFilter("JSON Files (*.json)");
        QDir defaultDir(GLOBAL_RESOURCE_PATH + "/chip_view/maps");
        fileDialog.setDirectory(defaultDir);

        QString fileName = "";
        // 显示文件对话框
        if (fileDialog.exec()) {
            fileName = fileDialog.selectedFiles().first();
        } else {
        }
        return fileName;
    }
};
#endif //GRID_VIEW_FILEHELPER_H
