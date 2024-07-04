#ifndef FONTSUTILITIES_H
#define FONTSUTILITIES_H

#include <QFontDatabase>
#include <QRegularExpression>
#include <QDir>
#include <QDebug>

class FontsUtilities : public QObject
{
   Q_OBJECT
public:
   static FontsUtilities& instance();

    QString loadFont(const QString &appDirPath);

private:
    FontsUtilities();
};

#endif // FONTSUTILITIES_H
