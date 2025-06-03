#ifndef SAVECONSTRAINTSDIALOG_H
#define SAVECONSTRAINTSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>

class SaveConstraintsDialog : public QDialog
{
    Q_OBJECT
public:
    SaveConstraintsDialog(QWidget *parent = nullptr);

protected:
    void accept() override;

private:
    QRadioButton *createNewFileRadioButton;
    QComboBox *fileTypeComboBox;
    QLineEdit *fileNameLineEdit;
    // QComboBox *fileLocationComboBox;

    QRadioButton *selectExistFileRadioButton;
    QComboBox *selectTargetFileComboBox;

    QDialogButtonBox* buttonBox;

    void validateInput();
};

#endif // SAVECONSTRAINTSDIALOG_H
