#ifndef OTHEROPTIONSWIDGET_H
#define OTHEROPTIONSWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

class OtherOptionsWidget : public QWidget
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
