#ifndef PORTBOPTIONSWIDGET_H
#define PORTBOPTIONSWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class PortBOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    PortBOptionsWidget(QWidget *parent = nullptr);
};

#endif // PORTBOPTIONSWIDGET_H
