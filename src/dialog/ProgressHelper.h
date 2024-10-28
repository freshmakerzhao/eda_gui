/**
  ******************************************************************************
  * @file           : ProgressHelper.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/28
  ******************************************************************************
  */
#ifndef PROGRESSHELPER_H
#define PROGRESSHELPER_H

#include <QProgressDialog>
#include <QTimer>

class ProgressHelper : public QObject  {
    Q_OBJECT
public:
    static bool showProgressDialog(QWidget* parent = nullptr,
                                   const QString& title = "Loading...",
                                   const QString& labelText = "Processing..."
                                   );
};

#endif // PROGRESSHELPER_H
