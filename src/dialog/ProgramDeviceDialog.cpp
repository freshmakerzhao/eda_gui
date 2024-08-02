#include "ProgramDeviceDialog.h"
#include "utils/HardWareManager.h"
#include "dialog/AdvancedFileDialog.h"

ProgramDeviceDialog::ProgramDeviceDialog(QWidget *parent)
{
    setFixedWidth(650);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QLabel *textlabel = new QLabel("Select a bitstream programming file and download it to your hardware device. "
                               // "You can optionallyselect a debug probes file that corresponds to the debug cores contained in the bitstream programming file."
                               , this);
    textlabel->setWordWrap(true);
    textlabel->setAlignment(Qt::AlignTop);
    // vLayout->addWidget(textlabel);
    hLayout->addWidget(textlabel);
    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/resource/icon.png");
    imageLabel->setPixmap(image.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignRight);
    hLayout->addWidget(imageLabel);
    vLayout->addLayout(hLayout);

    QFormLayout *fLayout = new QFormLayout;
    fLayout->setMargin(20);
    vLayout->addLayout(fLayout);

    QHBoxLayout *layout1 = new QHBoxLayout;
    fLayout->addRow("Bitstream file:", layout1);
    bitStreamFilePathLineEdit = new QLineEdit(this);
    bitStreamFilePathLineEdit->setClearButtonEnabled(true);
    layout1->addWidget(bitStreamFilePathLineEdit);
    bitStreamFilePathBrowseButton = new QPushButton("...", this);
    bitStreamFilePathBrowseButton->setFixedSize(25, 25);
    bitStreamFilePathBrowseButton->setObjectName("squareButton");
    connect(bitStreamFilePathBrowseButton, &QPushButton::clicked, this, &ProgramDeviceDialog::onbitStreamFilePathBrowseButtonClicked);
    layout1->addWidget(bitStreamFilePathBrowseButton);

    // QHBoxLayout *layout2 = new QHBoxLayout;
    // fLayout->addRow("Debug Probes File:", layout2);
    // debugProbesFilePathLineEdit = new QLineEdit(this);
    // debugProbesFilePathLineEdit->setClearButtonEnabled(true);
    // layout2->addWidget(debugProbesFilePathLineEdit);
    // debugProbesFilePathBrowseButton = new QPushButton("...", this);
    // debugProbesFilePathBrowseButton->setFixedSize(25, 25);
    // connect(debugProbesFilePathBrowseButton, &QPushButton::clicked, this, &ProgramDeviceDialog::ondebugProbesFilePathBrowseButtonClicked);
    // layout2->addWidget(debugProbesFilePathBrowseButton);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ProgramDeviceDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ProgramDeviceDialog::reject);
    vLayout->addWidget(buttonBox);
    vLayout->insertSpacing(1, 40);
    vLayout->insertSpacing(3, 80);
}

void ProgramDeviceDialog::accept()
{
    HardWareManager::instance().path = bitStreamFilePathLineEdit->text();
    QDialog::accept();
}

void ProgramDeviceDialog::onbitStreamFilePathBrowseButtonClicked()
{
    // QFileDialog dialog(this);
    AdvancedFileDialog dialog(this);
    // dialog.setOption(QFileDialog::DontUseNativeDialog,true);

    dialog.setWindowTitle("Open Project");
    //! Filters
    QStringList filters;
    filters << "Bitstream Files (*.bit *.bin)"
            << "All Files (*.*)";
    dialog.setNameFilters(filters);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QString path = dialog.selectedFiles().value(0, "");
    bitStreamFilePathLineEdit->setText(path);
}

// void ProgramDeviceDialog::ondebugProbesFilePathBrowseButtonClicked()
// {
//     QFileDialog dialog(this);
//     // dialog.setOption(QFileDialog::DontUseNativeDialog,true);

//     dialog.setWindowTitle("Open Project");
//     // dialog.setNameFilter("Bitstream Files(*.bit)");
//     dialog.setAcceptMode(QFileDialog::AcceptOpen);
//     if (dialog.exec() != QDialog::Accepted) {
//         return; // 用户取消了操作
//     }
//     QString path = dialog.selectedFiles().value(0, "");
//     bitStreamFilePathLineEdit->setText(path);
// }
