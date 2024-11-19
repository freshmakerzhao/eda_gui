/**
  ******************************************************************************
  * @File           : FileHelper.h
  * @Author         : zs
  * @Description    : 与文件相关的QT工具类
  * @Date           : 23-11-9
  ******************************************************************************
  */

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#ifdef WINDOWS_PLATFORM
// windows下校验文件是否存在时使用
#include <windows.h>
#elif defined(LINUX_PLATFORM)
#include <sys/stat.h>
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
    // windows下校验文件是否存在时使用
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
    #elif defined(LINUX_PLATFORM)
    #include <sys/stat.h>
    /**
     * 判断文件是否存在 (POSIX)
     * @param path 文件预期路径
     * @return true 如果文件存在，否则 false
     */
    static bool fileExists(const std::string& path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }
    #else
    std::cout << "Running on an unknown platform" << std::endl;
    #endif

    /**
     * 判断路径是否存在，不存在则创建
     * @param path 路径信息
     */
    static void ensureDirectoryExists(const QString& path) {
        QDir dir(path);
        if (!dir.exists()) {
            if (!dir.mkpath(path)) {
                qDebug() << "[FileHelper] Failed to create directory at:" << path;
            } else {
                qDebug() << "[FileHelper] Directory created or exists at:" << path;
            }
        }
    }

    static QString convertToStandardPath(const QString &filePath) {
        // QFileInfo 确保路径合法
        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists()) {
            return QString();
        }

        // 使用 QDir 的 toNativeSeparators 和 QDir::cleanPath
        QString standardizedPath = QDir::cleanPath(fileInfo.absoluteFilePath());
        return standardizedPath;
    }
};
#endif // FILEHELPER_H



