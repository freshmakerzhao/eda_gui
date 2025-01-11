#ifndef CONFIGURATIONPAGE_H
#define CONFIGURATIONPAGE_H

#include "Wizard.h"
#include <QCheckBox>

class ConfigurationPage :  public QWizardPage
{
    Q_OBJECT
public:
    ConfigurationPage(QWidget *parent = nullptr);

private slots:
    void onCompatibilityModeCheckBoxClicked(bool checked);
private:
    QCheckBox *compatibilityModeCheckBox;
    QLabel *compatibilityModeLabel;
};

#endif // CONFIGURATIONPAGE_H
