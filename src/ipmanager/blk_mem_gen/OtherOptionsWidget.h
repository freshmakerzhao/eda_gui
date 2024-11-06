#ifndef OTHEROPTIONSWIDGET_H
#define OTHEROPTIONSWIDGET_H

#include <QFileDialog>
#include "ipmanager/common/BasePage.h"

class OtherOptionsWidget : public BasePage
{
    Q_OBJECT
public:
    OtherOptionsWidget(QWidget *parent = nullptr);

public slots:
    void onBrowseButtonClicked();

    void onEditButtonClicked();

private:
    QCheckBox *loadInitFileCheckbox;

    QLineEdit *coeFileLineEdit;

    QPushButton *browseButton;

    QPushButton *editButton;

    const QString titleLabelQss = "QLabel{"
                                          "font-size: 20px;"
                                          "font-weight: bold;"
                                          "border-bottom: 2px solid #DCDCDC;"
                                          "border-radius: 0px;"
                                          "}";
};

#endif // OTHEROPTIONSWIDGET_H
