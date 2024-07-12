/**
  ******************************************************************************
  * @file           : AboutDialog.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/21
  ******************************************************************************
  */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = nullptr);

};

#endif // ABOUTDIALOG_H
