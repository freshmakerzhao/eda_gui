#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QFormLayout>
#include <QDebug>

class LicenseDialog : public QDialog
{
    Q_OBJECT
public:
    LicenseDialog(QWidget *parent = nullptr, const int &mode = 0);

    QString selectLicense() const;

private slots:
    void copyLicense();

};

#endif // LICENSEDIALOG_H
