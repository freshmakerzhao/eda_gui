#ifndef DESIGNTIMINGSUMMARYWIDGET_H
#define DESIGNTIMINGSUMMARYWIDGET_H

#include <QtWidgets>

class DesignTimingSummaryWidget : public QWidget
{
public:
    DesignTimingSummaryWidget(const int &worstNegativeSlack,
                              const int &totalNegativeSlack,
                              const int &worstHoldSlack,
                              const int &totalHoldSlack,
                              QWidget *parent = nullptr);

private:
    const QString TITLE_LABEL_STYLESHEET = "QLabel{"
                                           "font-size: 20px;"
                                           "font-weight: bold;"
                                           "border-bottom: 2px solid #DCDCDC;"
                                           "border-radius: 0px;"
                                           "}";
};

#endif // DESIGNTIMINGSUMMARYWIDGET_H
