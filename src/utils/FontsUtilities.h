#ifndef FONTSUTILITIES_H
#define FONTSUTILITIES_H

/**
  ******************************************************************************
  * @file           : FontsUtilities.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/24
  ******************************************************************************
  */
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
