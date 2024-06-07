#include "IPlocationDialog.h"

IPlocationDialog::IPlocationDialog(QWidget *parent) :
    QDialog(parent)
{
    setFixedHeight(70);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("IP location"));
    locationLineEdit = new QLineEdit(this);
    locationLineEdit->setClearButtonEnabled(true);
    layout->addWidget(locationLineEdit);
    locationButton = new QPushButton("...", this);
    layout->addWidget(locationButton);
    connect(locationButton, &QPushButton::clicked, this, &IPlocationDialog::onLocationButtonClicked);
}

int IPlocationDialog::exec()
{
    return QDialog::exec();
}

void IPlocationDialog::onLocationButtonClicked()
{
    QString location = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
    if (location.isEmpty()) {
        return;
    }
    locationLineEdit->setText(location);
}
