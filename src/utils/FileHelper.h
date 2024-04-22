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

#ifdef WINDOWS_PLATFORM
// windows下校验文件是否存在时使用
#include <windows.h>
#endif

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
    #ifdef WINDOWS_PLATFORM
    /**
     * 判断文件是否存在
     * @param path 文件预期路径
     * @return
     */
    static bool fileExists(const std::string& path) {
        DWORD fileAttr = GetFileAttributesA(path.c_str());
        if (fileAttr == INVALID_FILE_ATTRIBUTES)
            return false;
        return true;
    }
    #endif
};
#endif //GRID_VIEW_FILEHELPER_H



