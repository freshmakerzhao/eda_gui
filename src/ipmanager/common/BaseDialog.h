/**
  ******************************************************************************
  * @file           : BaseDialog.h
  * @author         : ksy
  * @description    : Base class for all ip configuration dialogs.
  * @attention      : None
  * @date           : 2024/11/7
  ******************************************************************************
  */
#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QGraphicsView>
#include <QToolBar>
#include <QSplitter>
#include <QScrollArea>
#include <QDialogButtonBox>

class BaseDialog : public QDialog
{
public:
    BaseDialog(QWidget *parent = nullptr);

protected:
    QTabWidget *tabWidget;

    // QGraphicsView *viewModule;

    QAction *docAction;
    QAction *ipLocAction;
    QAction *switchToDefaultAction;

    QLineEdit *componentNameLineEdit;

    QLabel *displayNameLabel;
};

#endif // BASEDIALOG_H
