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
    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;"
                                  "}";
};

#endif // PORTAOPTIONSWIDGET_H
