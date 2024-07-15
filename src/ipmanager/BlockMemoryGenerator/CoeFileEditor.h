/**
  ******************************************************************************
  * @file           : CoeFileEditor.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/6
  ******************************************************************************
  */
#ifndef COEFILEEDITOR_H
#define COEFILEEDITOR_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>

class CoeFileEditor : public QDialog
{
    Q_OBJECT
public:
    CoeFileEditor(QWidget *parent = nullptr);

    int exec() override;

public slots:
    void onSaveTriggered();

    void onSaveAsTriggered();

private:
    QTableView *tableView;

    QStandardItemModel *model;
};

#endif // COEFILEEDITOR_H
