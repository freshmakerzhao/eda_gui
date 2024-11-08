/**
  ******************************************************************************
  * @file           : BlockMemoryGenerator.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */
#include "BlockMemoryGenerator.h"
#include "ipmanager/common/SetNameUtils.h"

BlockMemoryGenerator::BlockMemoryGenerator(QWidget *parent) :
    BaseDialog(parent)
{
    displayNameLabel->setText("Block Memory Generator(8.4)");
    QString componentName = QString("blk_mem_gen_%1").arg(SetNameUtils::getComponentNameIndex());
    componentNameLineEdit->setText(componentName);

    basicWidget = new BasicWidget(this);
    connect(basicWidget, &BasicWidget::memoryTypeComboBoxChangeSignal, this, &BlockMemoryGenerator::updateMemoryType);
    portAOptionsWidget = new PortAOptionsWidget(this);
    portBOptionsWidget = new PortBOptionsWidget(this);
    otherOptionsWidget = new OtherOptionsWidget(this);
    summaryWidget = new BlkMemGenSummary(this);

    tabWidget->addTab(basicWidget, "Basic");
    tabWidget->addTab(portAOptionsWidget, "Port A Options");
    tabWidget->addTab(otherOptionsWidget, "Other Options");
    tabWidget->addTab(summaryWidget, "Summary");
}

void BlockMemoryGenerator::updateMemoryType(const QString &option)
{
    //! 更新summary
    qDebug() << "Memory Type : " << option;
    summaryWidget->setMemoryTypeInformation(option);

    //! Simple Dual Port RAM, True Dual Port RAM, Dual Port ROM 显示Port B Options
    if (option == "Simple Dual Port RAM" ||
        option == "True Dual Port RAM" ||
        option == "Dual Port ROM") {
        tabWidget->insertTab(2, portBOptionsWidget, "Port B Options");
        return;
    }
    if (tabWidget->count() == 5) {
        tabWidget->removeTab(2);
    }

}

void BlockMemoryGenerator::onipLocActionTrigger()
{
    // IPlocationDialog dialog(this);
    // QPoint globalPos = this->mapToGlobal(QPoint(0, 0));
    // dialog.move(globalPos.x() + 200, globalPos.y() + 100);
    // dialog.exec();
}
