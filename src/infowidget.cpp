#include "infowidget.h"
#include <QTextEdit>

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QTabWidget *tabWidget = new QTabWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(tabWidget);
    tabWidget->addTab(new QTextEdit,"Messages");
    tabWidget->addTab(new QTextEdit,"Log");
    tabWidget->addTab(new QTextEdit,"Reports");
}

InfoWidget::~InfoWidget()
{

}
