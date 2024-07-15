#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>

class BasicWidget : public QWidget
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
    void onInterfaceTypeComboBoxIndexChanged(const QString &option);

    // ! Generate address interface with 32 bits
    void onInterfaceTypeCheckBoxStateChanged(const int &state);

    // ! Common Clock
    void memoryTypeCheckBoxStateChanged(const int &state);

signals:
    // ! Memory Type
    void memoryTypeComboBoxChangeSignal(const QString &);

private:
    const QString titleLabelQss = "QLabel{"
                                          "font-size: 20px;"
                                          "font-weight: bold;"
                                          "border-bottom: 2px solid #DCDCDC;"
                                          "border-radius: 0px;"
                                          "}";
};

#endif // BASICWIDGET_H
