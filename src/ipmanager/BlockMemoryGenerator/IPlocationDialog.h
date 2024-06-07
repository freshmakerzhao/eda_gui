#ifndef IPLOCATIONDIALOG_H
#define IPLOCATIONDIALOG_H

#include <QDebug>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>

class IPlocationDialog : public QDialog
{
    Q_OBJECT
public:
    IPlocationDialog(QWidget *parent = nullptr);

    QLineEdit *locationLineEdit;

    QPushButton *locationButton;

    int exec() override;

public slots:
    void onLocationButtonClicked();
};

#endif // IPLOCATIONDIALOG_H
