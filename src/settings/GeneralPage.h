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

    QPushButton *project_device;
    QPushButton *project_device_square_button;

    QLineEdit *top_module_name;
};

#endif // GENERALPAGE_H
