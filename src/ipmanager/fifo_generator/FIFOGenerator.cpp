#include "FIFOGenerator.h"
#include "ipmanager/common/SetNameUtils.h"

FIFOGenerator::FIFOGenerator(QWidget *parent) :
    BaseDialog(parent)
{
    displayNameLabel->setText("FIFO Generator(13.2)");
    QString componentName = QString("fifo_generator_%1").arg(SetNameUtils::getComponentNameIndex());
    componentNameLineEdit->setText(componentName);

    basic = new Basic;
    nativePorts = new NativePorts;
    dataCounts = new DataCounts;

    tabWidget->addTab(basic, "Basic");
    tabWidget->addTab(nativePorts, "Native Ports");
    tabWidget->addTab(dataCounts, "Data Counts");
}
