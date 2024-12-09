#ifndef PORTAOPTIONSWIDGET_H
#define PORTAOPTIONSWIDGET_H

#include "ipmanager/common/BasePage.h"

class PortAOptionsWidget : public BasePage
{
    Q_OBJECT
public:
    PortAOptionsWidget(QWidget *parent = nullptr);

    QLineEdit *portAWidthLineEdit;

    QLineEdit *portADepthLineEdit;

    QLabel *portADepthRangeLabel;

    //! Port A Depth Range动态更新接口
    void updatePortADepthRange();

private:
    int portADepthMax = 1024;	// Port A 的最大位宽

};

#endif // PORTAOPTIONSWIDGET_H
