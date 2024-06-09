#include "IPlocationDialog.h"

IPlocationDialog::IPlocationDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // setFixedHeight(70);
    setFixedSize(700, 50);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(9, 0, 9, 0);
    layout->addWidget(new QLabel("IP location"));
    locationLineEdit = new QLineEdit(this);
    locationLineEdit->setClearButtonEnabled(true);
    layout->addWidget(locationLineEdit);
    locationButton = new QPushButton("...", this);
    locationButton->setFixedWidth(40);
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
