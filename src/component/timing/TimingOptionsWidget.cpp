#include "TimingOptionsWidget.h"

TimingOptionsWidget::TimingOptionsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    QScrollArea *scrollArea = new QScrollArea;
    vBoxLayout->addWidget(scrollArea);
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);
    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addSpacing(10);

    // ---------------------------- Report --------------------------------
    QLabel *reportLabel = new QLabel("Report");
    reportLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(reportLabel);

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout;
    vBoxLayout1->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(vBoxLayout1);

    QComboBox *pathDelayTypeComboBox = new QComboBox;
    pathDelayTypeComboBox->setFixedWidth(150);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Path delay type:", pathDelayTypeComboBox);
    pathDelayTypeComboBox->addItems(QStringList() << "min" << "max" << "min_max");
    vBoxLayout1->addLayout(formLayout);

    QCheckBox *reportUnconstrainedPathsCheckBox = new QCheckBox("Report unconstrained paths");
    vBoxLayout1->addWidget(reportUnconstrainedPathsCheckBox);
    QCheckBox *reportDatasheetCheckBox = new QCheckBox("Report datasheet");
    vBoxLayout1->addWidget(reportDatasheetCheckBox);

    mainLayout->addSpacing(20);
    // ------------------------- Path Limits ------------------------------
    QLabel *pathLimitsLabel = new QLabel("Report");
    pathLimitsLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(pathLimitsLabel);

    QSpinBox *maximumNumberOfPathsPerClockOrPathGroupSpinBox = new QSpinBox;
    maximumNumberOfPathsPerClockOrPathGroupSpinBox->setValue(10);
    QSpinBox *maximumNumberOfWorstPathsPerEndpoint = new QSpinBox;
    maximumNumberOfWorstPathsPerEndpoint->setValue(10);

    QFormLayout *formLayout1 = new QFormLayout;
    formLayout1->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(formLayout1);
    formLayout1->addRow("Maximum number of paths per clock or path group:", maximumNumberOfPathsPerClockOrPathGroupSpinBox);
    formLayout1->addRow("Maximum number of worst paths per endpoint:", maximumNumberOfWorstPathsPerEndpoint);

    mainLayout->addSpacing(20);
    // -------------------------- Path Display ----------------------------
    QLabel *pathDisplayLabel = new QLabel("Path Display");
    pathDisplayLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    mainLayout->addWidget(pathDisplayLabel);

    QLineEdit *displayPathsWithSlackLessThanLineEdit = new QLineEdit;
    QCheckBox *useDefaultCheckBox = new QCheckBox("Use default (1e+30)");
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setContentsMargins(25, 0, 0, 0);
    mainLayout->addLayout(hBoxLayout);
    hBoxLayout->addWidget(new QLabel("Display paths with slack less than:"));
    hBoxLayout->addWidget(displayPathsWithSlackLessThanLineEdit);
    hBoxLayout->addWidget(useDefaultCheckBox);

}
