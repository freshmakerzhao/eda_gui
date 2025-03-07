#include "TimingSummaryDialog.h"

TimingSummaryDialog::TimingSummaryDialog(QWidget *parent) :
    QDialog(parent)
{
    resize(1000, 800);
    setWindowTitle("Report Timing Summary");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    setStyleSheet("QScrollArea{"
                  " border: none;"
                  "}");

    resultsNameLineEdit = new QLineEdit;
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Results name:", resultsNameLineEdit);
    vLayout->addLayout(formLayout);

    tabWidget = new QTabWidget;
    vLayout->addWidget(tabWidget);

    timingOptionsWidget = new TimingOptionsWidget;
    tabWidget->addTab(timingOptionsWidget, "Options");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TimingSummaryDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TimingSummaryDialog::reject);
    vLayout->addWidget(buttonBox);
}
