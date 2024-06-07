/**
  ******************************************************************************
  * @file           : BlockMemoryGenerator.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */
#ifndef BLOCKMEMORYGENERATOR_H
#define BLOCKMEMORYGENERATOR_H

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
#include <QDialogButtonBox>
#include "BasicWidget.h"
#include "PortAOptionsWidget.h"
#include "PortBOptionsWidget.h"
#include "OtherOptionsWidget.h"
#include "SummaryWidget.h"
#include "IPlocationDialog.h"

class BlockMemoryGenerator : public QDialog
{
    Q_OBJECT
public:
    BlockMemoryGenerator(QWidget *parent = nullptr);

    int exec() override;

    // ! 响应MemoryType选择
    // ! 更新Summary
    // ！更新Tab
    void updateMemoryType(const QString &option);
private:
    QTabWidget *tabWidget;

    QLineEdit *cmptNameLineEdit;

    BasicWidget *basic;

    PortAOptionsWidget *portAOptions;

    PortBOptionsWidget *portBOptions;

    OtherOptionsWidget *otherOptions;

    SummaryWidget *summary;

    QGraphicsView *viewModule;

    QAction *docAction;
    QAction *ipLocAction;
    QAction *swToDftAction;

private slots:
    void onipLocActionTrigger();
};

#endif // BLOCKMEMORYGENERATOR_H
