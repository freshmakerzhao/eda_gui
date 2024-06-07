#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class SummaryWidget : public QWidget
{
    Q_OBJECT
public:
    SummaryWidget(QWidget *parent = nullptr);

    void setMemoryTypeInformation(const QString &option);

private:
    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;"
                                  "}";

    QLabel *memoryTypeLabel;
    QLabel *AddressWidthALabel;
};

#endif // SUMMARYWIDGET_H
