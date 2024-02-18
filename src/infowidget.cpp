#include "infowidget.h"
#include "ui_infowidget.h"
#include <QTextEdit>

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new QTextEdit,"Messages");
    ui->tabWidget->addTab(new QTextEdit,"Log");
    ui->tabWidget->addTab(new QTextEdit,"Reports");
}

InfoWidget::~InfoWidget()
{
    delete ui;
}
