/**
  ******************************************************************************
  * @file           : FlowNavigator.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/2
  ******************************************************************************
  */

#ifndef FLOWNAVIGATOR_H
#define FLOWNAVIGATOR_H

#include <QTreeWidget>

class FlowNavigator : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FlowNavigator(QWidget *parent = nullptr);

    void downloadBitstreamMode();

signals:
};

#endif // FLOWNAVIGATOR_H
