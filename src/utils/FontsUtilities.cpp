#include "FontsUtilities.h"

FontsUtilities &FontsUtilities::instance()
{
    static FontsUtilities instance;
    return instance;
}

QString FontsUtilities::loadFont(const QString &appDirPath) {
    QString fontDirPath = appDirPath + "/Fonts";
    QDir fontDir(fontDirPath);
    if (!fontDir.exists()) {
        qWarning() << "Font directory does not exist:" << fontDirPath;
        return QString();
    }

    QStringList files = fontDir.entryList(QDir::Files);
    QRegularExpression regex(".*\\.(ttf|otf)", QRegularExpression::CaseInsensitiveOption);
    QStringList fontFiles = files.filter(regex);
    if (fontFiles.isEmpty()) {
        qWarning() << "No font files found in directory:" << fontDirPath;
        return QString();
    }

    // 只读取一个字体文件
    QString fontFilePath = fontDir.absoluteFilePath(fontFiles.first());
    int id = QFontDatabase::addApplicationFont(fontFilePath);
    if (id == -1) {
        qWarning() << "Failed to load font:" << fontFilePath;
        return QString();
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(id);
    if (loadedFontFamilies.isEmpty()) {
        qWarning() << "No font families found in font:" << fontFilePath;
        return QString();
    }

    qDebug() << "Loaded font:" << fontFilePath << "with family:" << loadedFontFamilies.first();
    return loadedFontFamilies.first();
}

FontsUtilities::FontsUtilities() {}
