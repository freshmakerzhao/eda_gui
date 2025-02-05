#include "TextEditorSettingsPage.h"

TextEditorSettingsPage::TextEditorSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("TextEditorSettingsPage");

    QLabel *label = new QLabel("<b>Text Editor</b><br> You need to restart the Text Editor to apply these settings.", this);
    label->setStyleSheet("border-bottom: 3px dashed #DCDCDC;"
                         "border-radius: 0px;");
    label->setWordWrap(true);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(label);

    encodingComboBox = new QComboBox;
    encodingComboBox->addItem("UTF8");
    encodingComboBox->addItem("GBK");

    QSettings settings("HybrdChip", "HybrdLink");
    QString currentEncoding = settings.value("TextEditor/encoding", "UTF-8").toString();
    encodingComboBox->setCurrentText(currentEncoding);

    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow(hBoxLayout);
    formLayout->addRow("Encoding:", encodingComboBox);
}

void TextEditorSettingsPage::setEncoding()
{
    QSettings settings("HybrdChip", "HybrdLink");
    settings.setValue("TextEditor/encoding", encodingComboBox->currentText());
}
