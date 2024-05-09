#ifndef FORM_H
#define FORM_H

/**
  ******************************************************************************
  * @file           : Form.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/23
  ******************************************************************************
  */

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class Form : public QWidget
{
    Q_OBJECT

public:
    static Form *instance();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Form(QWidget *parent = nullptr);
    ~Form();
    std::string extractProjectName(const std::string& path);
};

#endif // FORM_H
