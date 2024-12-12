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
#include "utils/ProjectManager.h"
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
    portBOptionsWidget = new PortBOptionsWidget(this, portAOptionsWidget);
    otherOptionsWidget = new OtherOptionsWidget(this);
    summaryWidget = new BlkMemGenSummary(this);

    tabWidget->addTab(basicWidget, "Basic");
    tabWidget->addTab(portAOptionsWidget, "Port A Options");
    tabWidget->addTab(otherOptionsWidget, "Other Options");
    tabWidget->addTab(summaryWidget, "Summary");

    setup_core_generation_info();

    updateCoreGenerationInfoJson();

    setup_port_info();

    updatePortInfoJson();

}

void BlockMemoryGenerator::updateMemoryType(const QString &option)
{
    //! 更新summary
    qDebug() << "Memory Type : " << option;
    summaryWidget->setMemoryTypeInformation(option);
    QCheckBox *memoryTypeCheckBox = basicWidget->findChild<QCheckBox *>("Common Clock");
    //! Simple Dual Port RAM, True Dual Port RAM, Dual Port ROM 显示Port B Options
    if (option == "Single Port RAM" || option == "Single Port ROM") {
        memoryTypeCheckBox->setChecked(false);
        memoryTypeCheckBox->setEnabled(false);
    }
    else {
        tabWidget->insertTab(2, portBOptionsWidget, "Port B Options");
        memoryTypeCheckBox->setEnabled(true);
        updatePortInfoJson();
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

void BlockMemoryGenerator::updateCoreGenerationInfoJson()
{
    QJsonObject jsonObject;
    jsonObject.insert("version", "blk_mem");
    jsonObject.insert("component_name", componentNameLineEdit->text());
    jsonObject.insert("file_path", componentNameLineEdit->text() + ".json");

    QCheckBox *memoryTypeCheckBox = basicWidget->findChild<QCheckBox *>("Common Clock");
    if (memoryTypeCheckBox && memoryTypeCheckBox->isChecked()) {
        jsonObject.insert("common_clock", true);
    } else {
        jsonObject.insert("common_clock", false);
    }

    QComboBox *memoryTypeComboBox = basicWidget->findChild<QComboBox *>("memoryType");
    if (memoryTypeComboBox) {
        QString memoryType = memoryTypeComboBox->currentText();
        if (memoryType == "Single Port ROM") {
            jsonObject.insert("memory_type", "single_port_rom");
        } else if(memoryType == "Dual Port ROM") {
            jsonObject.insert("memory_type", "dual_port_rom");
        } else if(memoryType == "True Dual Port RAM") {
            jsonObject.insert("memory_type", "true_dual_port_ram");
        } else if(memoryType == "Simple Dual Port RAM") {
            jsonObject.insert("memory_type", "simple_dual_port_ram");
        } else if(memoryType == "Single Port RAM") {
            jsonObject.insert("memory_type", "single_port_ram");
        }
    } else {
        jsonObject.insert("memoryType", "single_port_rom");
    }

    QLineEdit *coeFileLineEdit = otherOptionsWidget->findChild<QLineEdit *>("coeFilePath");
    if (coeFileLineEdit && !coeFileLineEdit->text().isEmpty())
        jsonObject.insert("coe_path", coeFileLineEdit->text());

    QJsonDocument doc(jsonObject);
    core_generation_info = doc.toJson(QJsonDocument::Indented);
    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/core_generation_info.json"));
    if(file.isOpen()) {
        QTextStream in(&file);
        in << core_generation_info;//向文件写入数据
        file.close();
    }
    qDebug() << core_generation_info;
}

void BlockMemoryGenerator::setup_core_generation_info()
{
    QObject::connect(componentNameLineEdit, &QLineEdit::textChanged, [=]() {
        updateCoreGenerationInfoJson();
    });

    QList<QCheckBox *> checkBoxs = basicWidget->findChildren<QCheckBox *>();
    for (const QCheckBox *checkBox : checkBoxs) {
        QObject::connect(checkBox, &QCheckBox::toggled, [=]() {
            updateCoreGenerationInfoJson();
        });
    }

    QList<QComboBox *> comboboxs = basicWidget->findChildren<QComboBox *>();
    for (const QComboBox *combobox : comboboxs) {
        QObject::connect(combobox, &QComboBox::currentTextChanged, [=]() {
            updateCoreGenerationInfoJson();
        });
    }

    QLineEdit *coeFileLineEdit = otherOptionsWidget->findChild<QLineEdit *>("coeFilePath");
    QObject::connect(coeFileLineEdit, &QLineEdit::textChanged, [=]() {
        updateCoreGenerationInfoJson();
    });
}

void BlockMemoryGenerator::updatePortInfoJson()
{
    QJsonObject portA;

    QLineEdit *portAWidthLineEdit = portAOptionsWidget->findChild<QLineEdit *>("Port A Width");
    if (portAWidthLineEdit && !portAWidthLineEdit->text().isEmpty())
        portA.insert("data_width", portAWidthLineEdit->text().toInt());

    QLineEdit *portADepthLineEdit = portAOptionsWidget->findChild<QLineEdit *>("Port A Depth");
    if (portADepthLineEdit && !portADepthLineEdit->text().isEmpty())
        portA.insert("data_depth", portADepthLineEdit->text().toInt());

    portJsonRoot["porta"] = portA;

    QJsonObject portB;

    QComboBox *portBWidthComboBox = portBOptionsWidget->findChild<QComboBox *>("Port B Width");
    if(portBWidthComboBox && !portBWidthComboBox->currentText().isEmpty()) {
        portB.insert("data_width", portBWidthComboBox->currentText().toInt());
        portB.insert("data_depth", portBOptionsWidget->getDepth());
    }

    portJsonRoot["portb"] = portB;

    QJsonDocument doc(portJsonRoot);
    port_info = doc.toJson(QJsonDocument::Indented);
    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/port_info.json"));
    if(file.isOpen()) {
        QTextStream in(&file);
        in << port_info;
        file.close();
    }
    qDebug() << port_info;
}

void BlockMemoryGenerator::setup_port_info()
{
    QList<QLineEdit *> lineEdits = portAOptionsWidget->findChildren<QLineEdit *>();
    for(const QLineEdit* lineEdit : lineEdits) {
        if (lineEdit) {
            QObject::connect(lineEdit, &QLineEdit::textChanged, [=]() {
                updatePortInfoJson();
            });
        }
    }

    QList<QComboBox *> comboboxs = portBOptionsWidget->findChildren<QComboBox *>();
    for (const QComboBox *combobox : comboboxs) {
        QObject::connect(combobox, &QComboBox::currentTextChanged, [=]() {
            updatePortInfoJson();
        });
    }

    QObject::connect(
            portAOptionsWidget->portAWidthLineEdit,
            &QLineEdit::textChanged,
            this,
            [this](const QString &text) {
                portBOptionsWidget->UpdatePortBWidth(portAOptionsWidget);
            });

    QObject::connect(
            portAOptionsWidget->portADepthLineEdit,
            &QLineEdit::textChanged,
            this,
            [this](const QString &text) {
                portBOptionsWidget->UpdatePortBWidth(portAOptionsWidget);
            });

}


void BlockMemoryGenerator::accept()
{
    qDebug() << "ok";
    QProcess process;
    QString pythonPath = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("ipcore/rom_ip.exe");
    QStringList arguments;
    arguments << "--core_generation_info" << core_generation_info
              << "--port_info" << port_info
            ;

    process.setWorkingDirectory(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("ip"));

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
