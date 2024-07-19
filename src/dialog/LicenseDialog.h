#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QtWidgets>

class LicenseDialog : public QDialog
{
    Q_OBJECT
public:
    LicenseDialog(QWidget *parent = nullptr, const int &mode = 0);

    QString selectLicense() const;

private slots:
    void loadLicense();

};

#endif // LICENSEDIALOG_H
