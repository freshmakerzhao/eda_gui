#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include "ipmanager/common/BasePage.h"

class BasicWidget : public BasePage
{
    Q_OBJECT
public:
    BasicWidget(QWidget *parent = nullptr);

    QComboBox *interfaceTypeComboBox;

    QCheckBox *interfaceTypeCheckBox;

    QComboBox *memoryTypeComboBox;

    QCheckBox *memoryTypeCheckBox;

    QComboBox *eccTypeComboBox;

    QCheckBox *errorInjectionPinsCheckBox;

    QComboBox *errorInjectionPinsComboBox;

public slots:
    // ! Interface Type
    void onInterfaceTypeComboBoxIndexChanged(const int &index);

    // ! Generate address interface with 32 bits
    void onInterfaceTypeCheckBoxStateChanged(const int &state);

    // ! Common Clock
    void memoryTypeCheckBoxStateChanged(const int &state);

signals:
    // ! Memory Type
    void memoryTypeComboBoxChangeSignal(const QString &);

private:
};

#endif // BASICWIDGET_H
