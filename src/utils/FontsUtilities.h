/**
  ******************************************************************************
  * @file           : FontsUtilities.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/24
  ******************************************************************************
  */
#ifndef FONTSUTILITIES_H
#define FONTSUTILITIES_H

#include <QCoreApplication>
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDir>
#include <QRegularExpression>
#include <QDebug>
#include "base/Globals.h"

namespace FontsUtilities {

    void loadFont() {
        // Text Editor
        QFontDatabase::addApplicationFont(":/resource/fonts/LFTEtica/no-liga-LFTEticaMono-Regular-OK.ttf");// LFT Etica Mono

        // Application
        QString fontDirPath;

#ifdef FONTS_PATH
    // 通过编译选项设置字体路径
    fontDirPath = QString::fromStdString(FONTS_PATH);
#else
    // 使用默认路径
    fontDirPath = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("../Fonts");
#endif
#ifdef PACK_DEPLOYED
    // 打包模式下，字体路径固定为根目录下的 Fonts
    fontDirPath = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("../Fonts");
#endif
        QDir fontDir(fontDirPath);
        if (!fontDir.exists()) {
            qWarning() << "Font directory does not exist:" << fontDirPath;
            return;
        }

        QStringList files = fontDir.entryList(QDir::Files);
        QRegularExpression regex(".*\\.(ttf|otf)", QRegularExpression::CaseInsensitiveOption);
        QStringList fontFiles = files.filter(regex);
        if (fontFiles.isEmpty()) {
            qWarning() << "No font files found in directory:" << fontDirPath;
            return;
        }

        // 只读取一个字体文件
        QString fontFilePath = fontDir.absoluteFilePath(fontFiles.first());
        int id = QFontDatabase::addApplicationFont(fontFilePath);
        if (id == -1) {
            qWarning() << "Failed to load font:" << fontFilePath;
            return;
        }

        QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(id);
        if (loadedFontFamilies.isEmpty()) {
            qWarning() << "No font families found in font:" << fontFilePath;
            return;
        }

        qDebug() << "[FontsUtilities] Loaded font:" << fontFilePath << "with family:" << loadedFontFamilies.first();

        QString fontName = loadedFontFamilies.first();
        if (fontName.isEmpty()) {
            qCritical() << "Failed to load any font. Exiting.";
            fontName = QApplication::font().family();  // 使用 QApplication::instance()->font()
        }

        QFont font(fontName);
        font.setPointSizeF(QApplication::font().pointSizeF());
        font.setHintingPreference(QFont::PreferNoHinting); // 修复中文笔画粘连
        QApplication::setFont(font);  // 设置应用程序字体
    }
}


#endif // FONTSUTILITIES_H
