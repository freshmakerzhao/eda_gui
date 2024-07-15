#include "PortAOptionsWidget.h"

PortAOptionsWidget::PortAOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // ------------------- Memory Size ----------------------
    QLabel *memorySizeLabel = new QLabel("Memory Size", this);
    memorySizeLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(memorySizeLabel);

    QFormLayout *fLayout = new QFormLayout;
    fLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(fLayout);
    portAWidthLineEdit = new QLineEdit(this);
    portAWidthLineEdit->setClearButtonEnabled(true);
    portAWidthLineEdit->setFixedWidth(220);
    QHBoxLayout *portAWidthLayout = new QHBoxLayout;
    portAWidthLayout->addWidget(portAWidthLineEdit);
    portAWidthLayout->addWidget(new QLabel("Range: 1 to 4608 (bits)"));
    fLayout->addRow("Port A Width", portAWidthLayout);

    mainLayout->addSpacing(20);

    portADepthLineEdit = new QLineEdit(this);
    portADepthLineEdit->setClearButtonEnabled(true);
    portADepthLineEdit->setFixedWidth(220);
    QHBoxLayout *portADepthLayout = new QHBoxLayout;
    portADepthLayout->addWidget(portADepthLineEdit);
    portADepthRangeLabel = new QLabel("Range: -- to --", this);
    portADepthLayout->addWidget(portADepthRangeLabel);
    fLayout->addRow("Port A Depth", portADepthLayout);
    fLayout->addRow(new QLabel("The Width and Depth values are used for Read Operation in Port A", this));

    connect(portAWidthLineEdit, &QLineEdit::textChanged, this, &PortAOptionsWidget::updatePortADepthRange);

    // ------------------ Port A Optional Output Registers --------------------
    QLabel *portAOptionalOutputRegistersLabel = new QLabel("Port A Optional Output Registers", this);
    portAOptionalOutputRegistersLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(portAOptionalOutputRegistersLabel);
    mainLayout->addSpacing(20);
    // --------------------- Port A Output Reset Options ----------------------
    QLabel *portAOutputResetOptionsLabel = new QLabel("Port A Output Reset Options", this);
    portAOutputResetOptionsLabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(portAOutputResetOptionsLabel);
    mainLayout->addSpacing(20);

    // --------------------- READ Address Change A ----------------------
    QLabel *readAddressChangeALabel = new QLabel("READ Address Change A", this);
    readAddressChangeALabel->setStyleSheet(titleLabelQss);
    mainLayout->addWidget(readAddressChangeALabel);
}

void PortAOptionsWidget::updatePortADepthRange()
{
    // 在此处编写计算Port A Depth Range公式
    int portAWidth = portAWidthLineEdit->text().toInt();
    if (portAWidth == 0) return;
    portAWidth = 65536 / portAWidth;
    QString val = QString("Range: %1 to %2").arg(QString::number(2), QString::number(portAWidth));
    portADepthRangeLabel->setText(val);
}
