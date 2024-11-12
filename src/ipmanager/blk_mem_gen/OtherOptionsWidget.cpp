#include "OtherOptionsWidget.h"
#include "CoeFileEditor.h"

OtherOptionsWidget::OtherOptionsWidget(QWidget *parent) :
    BasePage(parent)
{
    // ----------------- Memory Initialization ----------------------
    QLabel *memoryInitializationLabel = new QLabel("Memory Initialization", this);
    memoryInitializationLabel->setStyleSheet(BasePage::TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(memoryInitializationLabel);

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(checkBoxLayout);
    loadInitFileCheckbox = new QCheckBox("Load init File", this);
    checkBoxLayout->addWidget(loadInitFileCheckbox);

    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(editLayout);
    editLayout->addWidget(new QLabel("Coe File", this));

    coeFileLineEdit = new QLineEdit(this);
    coeFileLineEdit->setEnabled(loadInitFileCheckbox->isChecked());
    coeFileLineEdit->setStyleSheet("QLineEdit{color: red;}");
    coeFileLineEdit->setText("no_coe_file_loaded");
    editLayout->addWidget(coeFileLineEdit);

    browseButton = new QPushButton("Browse", this);
    browseButton->setEnabled(loadInitFileCheckbox->isChecked());
    editLayout->addWidget(browseButton);
    connect(browseButton, &QPushButton::clicked, this, &OtherOptionsWidget::onBrowseButtonClicked);

    editButton = new QPushButton("Edit", this);
    editButton->setEnabled(loadInitFileCheckbox->isChecked());
    connect(editButton, &QPushButton::clicked, this, &OtherOptionsWidget::onEditButtonClicked);
    editLayout->addWidget(editButton);

    connect(loadInitFileCheckbox, &QCheckBox::stateChanged, [=]() {
        coeFileLineEdit->setEnabled(loadInitFileCheckbox->isChecked());
        browseButton->setEnabled(loadInitFileCheckbox->isChecked());
        editButton->setEnabled(loadInitFileCheckbox->isChecked());
    });

    mainLayout->addSpacing(20);
    QCheckBox *fillRemainingMemoryLocationsCheckBox = new QCheckBox("Fill Remaining Memory Locations");
    QLineEdit *remainingMemoryLocationsLineEdit = new QLineEdit;
    remainingMemoryLocationsLineEdit->setFixedWidth(250);
    remainingMemoryLocationsLineEdit->setClearButtonEnabled(true);
    QFormLayout *fLayout = new QFormLayout;
    fLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(fLayout);
    fLayout->addRow(fillRemainingMemoryLocationsCheckBox);
    fLayout->addRow("Remaining Memory Locations (Hex)", remainingMemoryLocationsLineEdit);
    mainLayout->addSpacing(20);
}

void OtherOptionsWidget::onBrowseButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Location For COE File");
    dialog.setNameFilter("COE Files(*.coe)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return; // 用户取消了操作
    }
    QString path = dialog.selectedFiles().value(0, "");
    // qDebug() << path;
    coeFileLineEdit->setStyleSheet("QLineEdit{color: black;}");
    coeFileLineEdit->setText(path);
}

void OtherOptionsWidget::onEditButtonClicked()
{
    CoeFileEditor editor(this);
    editor.exec();
}
