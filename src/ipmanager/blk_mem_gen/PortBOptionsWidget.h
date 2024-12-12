#ifndef PORTBOPTIONSWIDGET_H
#define PORTBOPTIONSWIDGET_H

#include "ipmanager/common/BasePage.h"
#include "PortAOptionsWidget.h"
#include <vector>

class PortBOptionsWidget : public BasePage
{
    Q_OBJECT
public:
    PortBOptionsWidget(QWidget *parent = nullptr, PortAOptionsWidget *portA = nullptr);
    void UpdatePortBWidth(PortAOptionsWidget *portA = nullptr);

    QComboBox *portBWidthComboBox;
    QLabel *portBDepthLabel;

private:
    void UpdatePortDepth(const QString &width);
    int capacity;
};

#endif // PORTBOPTIONSWIDGET_H
