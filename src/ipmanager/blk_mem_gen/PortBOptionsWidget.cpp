#include "PortBOptionsWidget.h"
#include <cmath>
#include <unordered_map>

PortBOptionsWidget::PortBOptionsWidget(QWidget *parent, PortAOptionsWidget *portA) :
    BasePage(parent)
{
    // ------------------- Memory Size ----------------------
    QLabel *memorySizeLabel = new QLabel("Memory Size", this);
    memorySizeLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(memorySizeLabel);

    QFormLayout *fLayout = new QFormLayout;
    fLayout->setContentsMargins(26, 0, 0, 0);
    mainLayout->addLayout(fLayout);
    portBWidthComboBox = new QComboBox(this);
    portBWidthComboBox->setObjectName("Port B Width");
    portBWidthComboBox->setFixedWidth(221);
    UpdatePortBWidth(portA);
    QHBoxLayout *portBWidthLayout = new QHBoxLayout;
    portBWidthLayout->addWidget(portBWidthComboBox);
    fLayout->addRow("Port B Width", portBWidthLayout);

    mainLayout->addSpacing(11);

    QHBoxLayout *portBDepthLayout = new QHBoxLayout;
    portBDepthLabel = new QLabel(portA->portADepthLineEdit->text(), this);
    portBDepthLayout->setObjectName("Port B Depth");
    portBDepthLayout->addWidget(portBDepthLabel);
    fLayout->addRow("Port B Depth :", portBDepthLayout);
    fLayout->addRow(new QLabel("The Width and Depth values are used for Read Operation in Port B", this));

    connect(portBWidthComboBox, &QComboBox::currentTextChanged, this, &PortBOptionsWidget::UpdatePortDepth);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(new QLabel("Operating Mode"));
    QComboBox *operatingModeComboBox = new QComboBox;
    operatingModeComboBox->addItems(QStringList() << "Write First" << "Read First" << "No Change");
    hLayout->addWidget(operatingModeComboBox);
    hLayout->addItem(new QSpacerItem(16, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout->addWidget(new QLabel("Enable Port Type"));
    QComboBox *enablePortTypeComboBox = new QComboBox;
    enablePortTypeComboBox->addItems(QStringList() << "Always Enabled" << "Use ENA Pin");
    hLayout->addWidget(enablePortTypeComboBox);
    mainLayout->addLayout(hLayout);
    mainLayout->addSpacing(11);

    // ------------------ Port B Optional Output Registers --------------------
    QLabel *portAOptionalOutputRegistersLabel = new QLabel("Port B Optional Output Registers", this);
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
    QCheckBox *regceaPinCheckBox = new QCheckBox("REGCEB Pin");
    gLayout->addWidget(regceaPinCheckBox, 1, 1);
    mainLayout->addLayout(gLayout);
    mainLayout->addSpacing(20);
    // --------------------- Port B Output Reset Options ----------------------
    QLabel *portAOutputResetOptionsLabel = new QLabel("Port B Output Reset Options", this);
    portAOutputResetOptionsLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(portAOutputResetOptionsLabel);
    mainLayout->addSpacing(20);

    // --------------------- READ Address Change B ----------------------
    QLabel *readAddressChangeALabel = new QLabel("READ Address Change B", this);
    readAddressChangeALabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(readAddressChangeALabel);
}

void PortBOptionsWidget::UpdatePortBWidth(PortAOptionsWidget *portA) {
    if(portA->portADepthLineEdit->text().isEmpty() || portA->portAWidthLineEdit->text().isEmpty()) {
        portBWidthComboBox->clear();
        return;
    }
    int portAWidth = portA->portAWidthLineEdit->text().toInt();
    int portADepth = portA->portADepthLineEdit->text().toInt();
    capacity = portAWidth * portADepth;
    std::vector<int> widths;

    int count = 0;

    int width_less = portAWidth;
    widths.insert(widths.begin(), portAWidth);
    while(width_less % 2 == 0 && count < 6) {
        width_less /= 2;
        widths.insert(widths.begin(), width_less);
        count ++;
    }
    count = 0;
    int width_greater = 2 * portAWidth;
    while(width_greater <= 4608 && width_greater <= capacity && count < 5) {
        widths.push_back(width_greater);
        width_greater *= 2;
        count ++;
    }

    portBWidthComboBox->clear();
    for(int width : widths) {
        portBWidthComboBox->addItem(QString::number(width));
    }
    portBWidthComboBox->setCurrentText(QString::number(portAWidth));
}

void PortBOptionsWidget::UpdatePortDepth(const QString &width) {
    if(width.isEmpty())
        return;
    int portBWidth = width.toInt();
    portBDepthLabel->setText(QString::number(capacity/portBWidth));
}

int PortBOptionsWidget::getDepth() {
    if(portBWidthComboBox->currentText().isEmpty())
        return 0;
    else
        return capacity/portBWidthComboBox->currentText().toInt();
}