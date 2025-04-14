#include "DesignTimingSummaryWidget.h"

DesignTimingSummaryWidget::DesignTimingSummaryWidget(const float &worstNegativeSlack,
                                                     const float &totalNegativeSlack,
                                                     const float &worstHoldSlack,
                                                     const float &totalHoldSlack,
                                                     QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setAlignment(Qt::AlignTop);

    QVBoxLayout *setupVBoxLayout = new QVBoxLayout;
    hBoxLayout->addLayout(setupVBoxLayout);
    QLabel *setupLabel = new QLabel("Setup");
    setupLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    setupVBoxLayout->addWidget(setupLabel);
    QFormLayout *setupFormLayout = new QFormLayout;
    setupVBoxLayout->addLayout(setupFormLayout);
    setupFormLayout->addRow("Worst Negative Slack", new QLabel(QString::number(worstNegativeSlack)));
    setupFormLayout->addRow("Total Negative Slack", new QLabel(QString::number(totalNegativeSlack)));

    QVBoxLayout *holdVBoxLayout = new QVBoxLayout;
    hBoxLayout->addLayout(holdVBoxLayout);
    QLabel *holdLabel = new QLabel("Hold");
    holdLabel->setStyleSheet(TITLE_LABEL_STYLESHEET);
    holdVBoxLayout->addWidget(holdLabel);
    QFormLayout *holdFormLayout = new QFormLayout;
    holdVBoxLayout->addLayout(holdFormLayout);
    holdFormLayout->addRow("Worst Hold Slack", new QLabel(QString::number(worstHoldSlack)));
    holdFormLayout->addRow("Total Hold Slack", new QLabel(QString::number(totalHoldSlack)));
}

