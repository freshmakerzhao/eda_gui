#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>

class GeneralPage : public QWidget
{
public:
    GeneralPage(QWidget *parent = nullptr);

    void setTopModule();

    void setDevicePart();

private:
    QFormLayout *fLayout;

    QLineEdit *projectDeviceLineEdit;

    QPushButton *projectDeviceSquareButton;

    QLineEdit *topModuleNameLineEdit;

    QStringList deviceInfo;

public slots:
    void startWizard();
};

#endif // GENERALPAGE_H
