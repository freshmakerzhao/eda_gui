#include "PortAOptionsWidget.h"

PortAOptionsWidget::PortAOptionsWidget(QWidget *parent) :
    BasePage(parent)
{
    // ------------------- Memory Size ----------------------
    QLabel *memorySizeLabel = new QLabel("Memory Size", this);
    memorySizeLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
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

    mainLayout->addSpacing(10);

    portADepthLineEdit = new QLineEdit(this);
    portADepthLineEdit->setClearButtonEnabled(true);
    portADepthLineEdit->setFixedWidth(220);
    portADepthLineEdit->setEnabled(false);
    QHBoxLayout *portADepthLayout = new QHBoxLayout;
    portADepthLayout->addWidget(portADepthLineEdit);
    portADepthRangeLabel = new QLabel("Range: -- to --", this);
    portADepthLayout->addWidget(portADepthRangeLabel);
    fLayout->addRow("Port A Depth", portADepthLayout);
    fLayout->addRow(new QLabel("The Width and Depth values are used for Read Operation in Port A", this));

    connect(portAWidthLineEdit, &QLineEdit::textChanged, this, &PortAOptionsWidget::updatePortADepthRange);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(new QLabel("Operating Mode"));
    QComboBox *operatingModeComboBox = new QComboBox;
    operatingModeComboBox->addItems(QStringList() << "Write First" << "Read First" << "No Change");
    hLayout->addWidget(operatingModeComboBox);
    hLayout->addItem(new QSpacerItem(15, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout->addWidget(new QLabel("Enable Port Type"));
    QComboBox *enablePortTypeComboBox = new QComboBox;
    enablePortTypeComboBox->addItems(QStringList() << "Always Enabled" << "Use ENA Pin");
    hLayout->addWidget(enablePortTypeComboBox);
    mainLayout->addLayout(hLayout);
    mainLayout->addSpacing(10);

    connect(portADepthLineEdit, &QLineEdit::textChanged, this, [this, portADepthLayout](){
        if(portADepthLineEdit->text().toInt() > portADepthMax)
            portADepthLineEdit->setText(QString::number(portADepthMax));
    } );

    // ------------------ Port A Optional Output Registers --------------------
    QLabel *portAOptionalOutputRegistersLabel = new QLabel("Port A Optional Output Registers", this);
    portAOptionalOutputRegistersLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(portAOptionalOutputRegistersLabel);
    QGridLayout *gLayout = new QGridLayout;
    gLayout->setContentsMargins(25, 0, 0, 0);
    gLayout->setAlignment(Qt::AlignLeft);
    QCheckBox *primitivesOutputRegisterCheckBox = new QCheckBox("Primitives Output Register");
    gLayout->addWidget(primitivesOutputRegisterCheckBox, 0, 0);
    QCheckBox *coreOutputRegisterCheckBox = new QCheckBox("Core Output Register");
    gLayout->addWidget(coreOutputRegisterCheckBox, 0, 1);
    QCheckBox *softECCInputRegisterCheckBox = new QCheckBox("SoftECC Input Register");
    gLayout->addWidget(softECCInputRegisterCheckBox, 1, 0);
    QCheckBox *regceaPinCheckBox = new QCheckBox("REGCEA Pin");
    gLayout->addWidget(regceaPinCheckBox, 1, 1);
    mainLayout->addLayout(gLayout);
    mainLayout->addSpacing(20);
    // --------------------- Port A Output Reset Options ----------------------
    QLabel *portAOutputResetOptionsLabel = new QLabel("Port A Output Reset Options", this);
    portAOutputResetOptionsLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(portAOutputResetOptionsLabel);
    mainLayout->addSpacing(20);

    // --------------------- READ Address Change A ----------------------
    QLabel *readAddressChangeALabel = new QLabel("READ Address Change A", this);
    readAddressChangeALabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(readAddressChangeALabel);
}

void PortAOptionsWidget::updatePortADepthRange()
{
    // 在此处编写计算Port A Depth Range公式
    if(portAWidthLineEdit->text().isEmpty()) {
        portADepthLineEdit->clear();
        portADepthLineEdit->setEnabled(false);
        return;
    } else {
        portADepthLineEdit->setEnabled(true);
    }

    int portAWidth = portAWidthLineEdit->text().toInt();
    if (portAWidth <= 0) return;
    else if (portAWidth == 1) portADepthMax = 32768;
    else if (portAWidth == 2) portADepthMax = 16385;
    else if (portAWidth > 2 && portAWidth <= 4) portADepthMax = 8192;
    else if (portAWidth > 4 && portAWidth <= 9) portADepthMax = 4096;
    else if (portAWidth > 9 && portAWidth <= 18) portADepthMax = 2048;
    else portADepthMax = 1024;

    if(portAWidth > 4608)
        portAWidthLineEdit->setText("4608");

    QString val = QString("Range: %1 to %2").arg(QString::number(2), QString::number(portADepthMax));
    portADepthRangeLabel->setText(val);
    if(portADepthLineEdit->text().toInt() > portADepthMax)
        portADepthLineEdit->setText(QString::number(portADepthMax));

}
