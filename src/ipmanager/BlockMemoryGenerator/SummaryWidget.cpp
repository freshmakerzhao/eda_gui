#include "SummaryWidget.h"

SummaryWidget::SummaryWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // --------------- Information ------------------
    QLabel *infoLabel = new QLabel("Information", this);
    infoLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(infoLabel);
    QFormLayout *fLayout = new QFormLayout;
    fLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(fLayout);
    memoryTypeLabel = new QLabel("Single Port ROM", this);
    fLayout->addRow("Memory Type:", memoryTypeLabel);
    AddressWidthALabel = new QLabel(this);
}

void SummaryWidget::setMemoryTypeInformation(const QString &option)
{
    memoryTypeLabel->setText(option);
}
