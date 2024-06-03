#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>

class GeneralPage : public QWidget
{
public:
    GeneralPage(QWidget *parent = nullptr);

    void setTopModule();
private:
    QFormLayout *fLayout;

    // QLineEdit *project_device;
    QLineEdit *top_module_name;
};

#endif // GENERALPAGE_H
