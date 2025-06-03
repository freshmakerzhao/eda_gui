#include "SaveConstraintsDialog.h"

SaveConstraintsDialog::SaveConstraintsDialog(QWidget *parent)
{
    setWindowTitle("Save Constraints");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop);
    QLabel *textlabel = new QLabel("Select a target file to write new unsaved constraints to. "
                                   "Choosing anexisting file will update that file with the new constraints."
                                   , this);
    textlabel->setWordWrap(true);
    textlabel->setAlignment(Qt::AlignTop);
    vLayout->addWidget(textlabel);

    createNewFileRadioButton = new QRadioButton("Create a new file");
    selectExistFileRadioButton = new QRadioButton("Select an existing file");

    QButtonGroup *buttonGroup = new QButtonGroup;
    buttonGroup->addButton(createNewFileRadioButton);
    buttonGroup->addButton(selectExistFileRadioButton);
    createNewFileRadioButton->setChecked(true);

    fileTypeComboBox = new QComboBox;
    fileTypeComboBox->addItems(QStringList() << "XDC");
    fileNameLineEdit = new QLineEdit;

    selectTargetFileComboBox = new QComboBox;
    selectTargetFileComboBox->setToolTip("<select a target file>");

    QFormLayout *fLayout1 = new QFormLayout;
    QFormLayout *fLayout2 = new QFormLayout;

    fLayout1->addRow("      File type:", fileTypeComboBox);
    fLayout1->addRow("      File name:", fileNameLineEdit);

    fLayout2->addRow("      ", selectTargetFileComboBox);

    vLayout->addWidget(createNewFileRadioButton);
    vLayout->addLayout(fLayout1);
    vLayout->addWidget(selectExistFileRadioButton);
    vLayout->addLayout(fLayout2);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel| QDialogButtonBox::Ok, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SaveConstraintsDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SaveConstraintsDialog::accept);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    vLayout->addWidget(buttonBox);

    connect(createNewFileRadioButton, &QRadioButton::clicked, this, &SaveConstraintsDialog::validateInput);
    connect(fileNameLineEdit, &QLineEdit::textChanged, this, &SaveConstraintsDialog::validateInput);
}

void SaveConstraintsDialog::accept()
{
    QDialog::accept();
}

void SaveConstraintsDialog::validateInput()
{
    if ((createNewFileRadioButton->isChecked()) && !fileNameLineEdit->text().isEmpty()) {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
