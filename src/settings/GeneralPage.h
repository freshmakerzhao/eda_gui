#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class GeneralPage : public QWidget
{
public:
    GeneralPage(QWidget *parent = nullptr);

    void setTopModule();

private:
    QFormLayout *fLayout;

    QPushButton *projectDeviceButton;
    QPushButton *projectDeviceSquareButton;

    QLineEdit *topModuleNameLineEdit;
};

#endif // GENERALPAGE_H
