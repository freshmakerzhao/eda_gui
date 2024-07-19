#include "LicenseDialog.h"

LicenseDialog::LicenseDialog(QWidget *parent, const int &mode)
    : QDialog(parent)
{
    // this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowTitle("License");
    QFormLayout *formLayout = new QFormLayout(this);

    QString title;
    if (mode == -1) {
        title = "Please load the license.lic file. "
                "This step is essential to ensure the application can verify and activate the corresponding license, "
                "allowing you to access all features. If you encounter any issues during this process, "
                "please refer to the documentation or contact support for further assistance.";
    } else if (mode == -2) {
        title = "The license. lic file could not be parsed."
                "Please ensure that the file is correctly formatted and valid.  "
                "If the issue persists, refer to the documentation or contact support for further assistance.";
    } else if (mode == -3) {
        title = "License verification failed."
                "If the issue persists, please refer to the documentation or contact support for further assistance.";
    } else if (mode == -4) {
        title = "Your software has expired."
                "Please request the latest license."
                "lic to continue using it. "
                "If you need assistance, please refer to the documentation or contact support.";
    }
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LicenseDialog::reject);
    QPushButton *loadLicenseButton = new QPushButton("Load License");
    connect(loadLicenseButton, &QPushButton::clicked, this, &LicenseDialog::loadLicense);
    buttonBox->addButton(loadLicenseButton, QDialogButtonBox::ActionRole);
    QLabel *label = new QLabel(title, this);
    label->setWordWrap(true);
    formLayout->addWidget(label);
    formLayout->addRow(buttonBox);

}

void LicenseDialog::loadLicense()
{
    const QString licenseFile = QFileDialog::getOpenFileName(nullptr, "Select License", QDir::homePath(), "License File (*.lic)");
    if (licenseFile.isEmpty()) {
        this->accept();
    }
    const QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString licensePath = QFileInfo(appDataPath).path() + "/HybrdChip/Common/license.lic";
    QFile file(licensePath);
    if (file.exists()) {
        // 如果目标文件已经存在，先删除它
        file.remove();
    }

    const bool success = QFile::copy(licenseFile, licensePath);
    if (success) {
        qDebug() << "License copied successfully.";
    } else {
        qDebug() << "License copied failed.";
    }
    this->accept();
}
