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
};

#endif // OTHEROPTIONSWIDGET_H
