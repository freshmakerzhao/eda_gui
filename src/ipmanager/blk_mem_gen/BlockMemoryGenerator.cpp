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

void BlockMemoryGenerator::accept()
{
    QProcess process;
    QString pythonPath = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("ipcore/rom_ip.exe");
    QStringList arguments;
    arguments << "--core_generation_info" << core_generation_info;

    process.setWorkingDirectory(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("ip"));
    // 启动 Python 脚本
    process.start(pythonPath, arguments);

    // 等待脚本启动并输出
    if (!process.waitForStarted()) {
        QMessageBox::warning(this, "Information" ,"The IP Core generation failed. Failed to start the process");
    }

    // 等待脚本完成执行
    process.waitForFinished();

    // 读取输出内容
    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (!output.isEmpty()) {
        qDebug() << output;
    }

    if (!error.isEmpty()) {
        QMessageBox::warning(this, "Information" ,"The IP Core generation failed." + error);
    }

    QDialog::accept();
}