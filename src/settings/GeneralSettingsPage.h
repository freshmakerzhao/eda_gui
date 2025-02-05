
#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>

class GeneralSettingsPage : public QWidget
{
public:
    GeneralSettingsPage(QWidget *parent = nullptr);

    void setTopModule();

    void setDevicePart();

private:
    QString tempDeviceName;

    QFormLayout *fLayout;

    QLineEdit *projectDeviceLineEdit;

    QPushButton *projectDeviceSquareButton;

    QLineEdit *topModuleNameLineEdit;

    QStringList deviceInfo;

public slots:
    void startWizard();
};

#endif // GENERALSETTINGSPAGE_H
