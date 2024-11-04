#include "ClockingWizard.h"
#include "utils/ProjectManager.h"

ClockingWizard::ClockingWizard(QWidget *parent) :
    QDialog(parent)
{
    resize(1600, 900);
    setWindowTitle("Custom IP");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    // vLayout->setAlignment(Qt::AlignTop);
    setStyleSheet("QScrollArea{"
                  " border: none;"
                  "}");
    // ! ---------------------- C区 ----------------------------
    // C区基控件
    QWidget *Cwidget = new QWidget(this);
    // C区布局
    QVBoxLayout *CvLayout = new QVBoxLayout(Cwidget);
    CvLayout->setMargin(0);
    QFormLayout *cmptNameLayout = new QFormLayout;
    componentNameLineEdit = new QLineEdit(this);
    componentNameLineEdit->setText("clk_wiz_0");
    cmptNameLayout->addRow("Component Name", componentNameLineEdit);
    tabWidget = new QTabWidget(this);
    CvLayout->addLayout(cmptNameLayout);
    CvLayout->addWidget(tabWidget);

    // ! ---------------------- B区 ----------------------------
    // viewModule = new QGraphicsView;

    // ! ---------------------- A区 ----------------------------
    QVBoxLayout *AvLayout = new QVBoxLayout;
    QHBoxLayout *titleLayout = new QHBoxLayout;
    // titleLayout->setAlignment(Qt::AlignLeading);
    QLabel *ipNameLabel = new QLabel("Clocking Wizard(6.0)", this);
    ipNameLabel->setStyleSheet("font-size: 20px;"
                               "font-weight: bold;");
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignRight);
    QPixmap image(":/resource/icon.png");
    titleLayout->addWidget(ipNameLabel);
    titleLayout->addWidget(logoLabel);
    logoLabel->setPixmap(image.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    AvLayout->addLayout(titleLayout);
    AvLayout->setAlignment(Qt::AlignTop);
    QToolBar *toolBar = new QToolBar(this);
    AvLayout->addWidget(toolBar);
    docAction = new QAction("Documentation", this);
    toolBar->addAction(docAction);
    ipLocAction = new QAction("IP Location", this);
    toolBar->addAction(ipLocAction);
    switchToDefaultAction = new QAction("Switch to Defaults", this);
    toolBar->addAction(switchToDefaultAction);
    // ! --------------------- 总布局 ---------------------------
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(15);
    // splitter->addWidget(viewModule);
    splitter->addWidget(Cwidget);
    QList<int> list;
    list << 500 << 1100;
    splitter->setSizes(list);
    vLayout->addLayout(AvLayout);
    vLayout->addWidget(splitter);
    vLayout->setStretchFactor(AvLayout, 1);
    vLayout->setStretchFactor(splitter, 13);

    clockingOptionsWidget = new ClockingOptionsWidget(inputClockInformationTableView, inputClockInformationModel, this);
    outputClocksWidget = new OutputClocksWidget(outputClockInforTableView, outputClockInforModel, this);
    portRenamingWidget = new PortRenamingWidget(this);
    mmcmSettingsWidget = new MMCMSettingsWidget(this);
    summaryWidget = new SummaryWidget(this);

    tabWidget->addTab(clockingOptionsWidget, "Clocking Options");
    tabWidget->addTab(outputClocksWidget, "Output Clocks");
    tabWidget->addTab(portRenamingWidget, "Port Renaming");
    tabWidget->addTab(mmcmSettingsWidget, "MMCM Settings");
    tabWidget->addTab(summaryWidget, "Summary");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ClockingWizard::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ClockingWizard::reject);

    vLayout->addWidget(buttonBox);

    setup_core_generation_info(nullptr, nullptr);

    updateCoreGenerationInfoJson(nullptr, nullptr);

    setup_input_clock_infor(inputClockInformationTableView, inputClockInformationModel);

    updateInputClockInforJson(inputClockInformationTableView, inputClockInformationModel);

    setup_output_clock_infor(outputClockInforTableView, outputClockInforModel);

    updateOutputClockInforJson(outputClockInforTableView, outputClockInforModel);

    QList<QRadioButton *> radioButtons = clockingOptionsWidget->findChildren<QRadioButton *>();
    for (const QRadioButton * radioButton : radioButtons) {
        QObject::connect(radioButton, &QRadioButton::toggled, [=]() {
            updateCoreGenerationInfoJson(nullptr, nullptr);
        });
    }

    QList<QCheckBox *> checkBoxs = clockingOptionsWidget->findChildren<QCheckBox *>();
    for (const QCheckBox *checkBox : checkBoxs) {
        QObject::connect(checkBox, &QCheckBox::toggled, [=]() {
            updateCoreGenerationInfoJson(nullptr, nullptr);
        });
    }
}

void ClockingWizard::accept()
{
    QProcess process;
    QString pythonPath = QDir(GlobalConfig::GLOBAL_RESOURCE_PATH).filePath("ipcore/clk_ip.exe");
    QStringList arguments;
    arguments << "--core_generation_info" << core_generation_info
              << "--input_clock_infor" << input_clock_infor
              << "--output_clock_infor" << output_clock_infor
            ;

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

void ClockingWizard::setup_core_generation_info(QTableView *tableView, QStandardItemModel *model)
{
    QObject::connect(componentNameLineEdit, &QLineEdit::textChanged, [=]() {
        updateCoreGenerationInfoJson(tableView, model);
    });
}

void ClockingWizard::setup_input_clock_infor(QTableView *tableView, QStandardItemModel *model)
{
    QList<QLineEdit*> lineEdits = {
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 2))),
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 3))),
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 6))),
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 2))),
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 3))),
        qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 6)))
    };

    for (QLineEdit *edit : lineEdits) {
        if (edit) {
            QObject::connect(edit, &QLineEdit::textChanged, [=]() {
                updateInputClockInforJson(tableView, model);
            });
        }
    }

    QList<QComboBox*> comboBoxes = {
        qobject_cast<QComboBox*>(tableView->indexWidget(model->index(0, 5))),
        qobject_cast<QComboBox*>(tableView->indexWidget(model->index(0, 7))),
        qobject_cast<QComboBox*>(tableView->indexWidget(model->index(1, 7)))
    };

    for (QComboBox *comboBox : comboBoxes) {
        if (comboBox) {
            QObject::connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]() {
                updateInputClockInforJson(tableView, model);
            });
        }
    }
}

void ClockingWizard::setup_output_clock_infor(QTableView *tableView, QStandardItemModel *model)
{
    for (int i = 0; i < model->rowCount(); i++) {
        // 监听 "Output Clock"
        if (tableView->indexWidget(model->index(i, 0))) {
            QCheckBox *outputClockCheckBox = tableView->indexWidget(model->index(i, 0))->findChild<QCheckBox *>();
            QObject::connect(outputClockCheckBox, &QCheckBox::stateChanged, [=]() {
                updateOutputClockInforJson(tableView, model);
            });
        }

        // 监听 "Port Name"
        QLineEdit *portNameEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 1)));
        QObject::connect(portNameEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model);
        });

        // 监听 "Output Freq(MHz) Requested"
        QLineEdit *freqEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 2)));
        QObject::connect(freqEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model);
        });

        // 监听 "Phase (degrees) Requested"
        QLineEdit *phaseEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 4)));
        QObject::connect(phaseEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model);
        });

        // 监听 "Duty Cycle (%) Requested"
        QLineEdit *dutyCycleEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 6)));
        QObject::connect(dutyCycleEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model);
        });

        // 监听 "Drive"
        QComboBox *driveCombo = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(i, 8)));
        QObject::connect(driveCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]() {
            updateOutputClockInforJson(tableView, model);
        });

        // 监听 "UseFine PS"
        if (tableView->indexWidget(model->index(i, 9))) {
            QCheckBox *finePsCheckBox =  tableView->indexWidget(model->index(i, 9))->findChild<QCheckBox *>();
            QObject::connect(finePsCheckBox, &QCheckBox::stateChanged, [=]() {
                updateOutputClockInforJson(tableView, model);
            });
        }

    }
}

void ClockingWizard::updateCoreGenerationInfoJson(QTableView *tableView, QStandardItemModel *model)
{
    QJsonObject jsonObject;
    jsonObject.insert("version", "clk_wiz_v6_0_5_0_0");
    jsonObject.insert("component_name", componentNameLineEdit->text());
    jsonObject.insert("file_path", componentNameLineEdit->text() + ".json");
    QCheckBox *phaseAlignmentCheckBox = clockingOptionsWidget->findChild<QCheckBox *>("Phase_Alignment");
    if (phaseAlignmentCheckBox && phaseAlignmentCheckBox->isChecked()) {
        jsonObject.insert("use_phase_alignment", true);
    } else {
        jsonObject.insert("use_phase_alignment", false);
    }

    jsonObject.insert("use_min_o_jitter", false);
    jsonObject.insert("use_max_i_jitter", false);
    QCheckBox *dynamicPhaseShiftCheckBox = clockingOptionsWidget->findChild<QCheckBox *>("Dynamic_Phase_Shift");
    if (dynamicPhaseShiftCheckBox && dynamicPhaseShiftCheckBox->isChecked()) {
        jsonObject.insert("use_dyn_phase_shift", true);
    } else {
        jsonObject.insert("use_dyn_phase_shift", false);
    }

    jsonObject.insert("use_inclk_switchover", true);
    QCheckBox *dynamicReconfigCheckBox = clockingOptionsWidget->findChild<QCheckBox *>("Dynamic_Reconfig");
    if (dynamicReconfigCheckBox && dynamicReconfigCheckBox->isChecked()) {
        jsonObject.insert("use_dyn_reconfig", true);
    } else {
        jsonObject.insert("use_dyn_reconfig", false);
    }

    jsonObject.insert("enable_axi", 0);
    jsonObject.insert("feedback_source", "FDBK_AUTO");
    QRadioButton *mmcmRadioButton = clockingOptionsWidget->findChild<QRadioButton *>("MMCM_Radio_Button");
    QRadioButton *pllRadioButton = clockingOptionsWidget->findChild<QRadioButton *>("PLL_Radio_Button");
    if (mmcmRadioButton && pllRadioButton) {
        if (mmcmRadioButton->isChecked()) {
            jsonObject.insert("PRIMITIVE", "MMCM");
        } else {
            jsonObject.insert("PRIMITIVE", "PLL");
        }
    }
    jsonObject.insert("num_in_clk", 1);
    jsonObject.insert("num_out_clk", 2);
    jsonObject.insert("clkin1_period", 10.101);
    jsonObject.insert("clkin2_period", 15.152);
    jsonObject.insert("use_power_down", false);
    jsonObject.insert("use_reset", true);
    jsonObject.insert("use_locked", true);
    jsonObject.insert("use_inclk_stopped", false);
    jsonObject.insert("feedback_type", "SINGLE");
    jsonObject.insert("CLOCK_MGR_TYPE", "NA");
    jsonObject.insert("manual_override", false);

    QJsonDocument doc(jsonObject);
    core_generation_info = doc.toJson(QJsonDocument::Indented);
    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/core_generation_info.json"));
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    if(file.isOpen()) {
        QTextStream in(&file);
        in << core_generation_info;//向文件写入数据
        file.close();
    }
    qDebug() << core_generation_info;
}

void ClockingWizard::updateInputClockInforJson(QTableView *tableView, QStandardItemModel *model) {
    // QJsonObject json = generateJson(tableView, model);

    QJsonObject jsonRoot;

    // 获取 Primary 数据
    QJsonObject primary;
    primary["input_clock"] = "primary";

    // 端口名称
    QLineEdit *priPortNameEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 2)));
    if (priPortNameEdit)
        primary["port_name"] = priPortNameEdit->text();

    // 输入频率
    QLineEdit *priFrequencyEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 3)));
    if (priFrequencyEdit)
        primary["input_frequency"] = priFrequencyEdit->text().toDouble();

    // 抖动选项
    QComboBox *priJitterOptionsComboBox = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(0, 5)));
    if (priJitterOptionsComboBox)
        primary["jitter_options"] = priJitterOptionsComboBox->currentText();

    // 输入抖动
    QLineEdit *priJitterEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(0, 6)));
    if (priJitterEdit)
        primary["input_jitter"] = priJitterEdit->text().toDouble();

    // 来源
    QComboBox *priSourceBox = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(0, 7)));
    if (priSourceBox)
        primary["source"] = priSourceBox->currentText();

    jsonRoot["Primary"] = primary;

    // 获取 Secondary 数据
    QJsonObject secondary;
    secondary["input_clock"] = "secondary";

    // 端口名称
    QLineEdit *secPortNameEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 2)));
    if (secPortNameEdit)
        secondary["port_name"] = secPortNameEdit->text();

    // 输入频率
    QLineEdit *secFrequencyEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 3)));
    if (secFrequencyEdit)
        secondary["input_frequency"] = secFrequencyEdit->text().toDouble();

    // 抖动选项 (Secondary没有设置)
    secondary["jitter_options"] = "";

    // 输入抖动
    QLineEdit *secJitterEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(1, 6)));
    if (secJitterEdit)
        secondary["input_jitter"] = secJitterEdit->text().toDouble();

    // 来源
    QComboBox *secSourceBox = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(1, 7)));
    if (secSourceBox)
        secondary["source"] = secSourceBox->currentText();

    jsonRoot["Secondary"] = secondary;

    QJsonDocument doc(jsonRoot);
    input_clock_infor = doc.toJson(QJsonDocument::Indented);

    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/input_clock_infor.json"));
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    if(file.isOpen()) {
        QTextStream in(&file);
        in << input_clock_infor;//向文件写入数据
        file.close();
    }
    qDebug() << input_clock_infor;
}

void ClockingWizard::updateOutputClockInforJson(QTableView *tableView, QStandardItemModel *model)
{
    int outputClockCount = 0;
    for (int i = 0; i < model->rowCount(); i++) {
        QWidget *widget = tableView->indexWidget(model->index(i, 0));
        if (widget) {
            QCheckBox *outputClockCheckBox = widget->findChild<QCheckBox *>();
            if (outputClockCheckBox && outputClockCheckBox->isChecked()) {
                outputClockCount ++;
            } else {
                // 如果中间存在未选择的行，则停止计算
                break;
            }
        }
    }

    qDebug() << "outputClockCount: " << outputClockCount;
    // out_clk_nums = outputClockCount;

    for (int row = 0; row <= outputClockCount; ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QWidget *widget = tableView->indexWidget(model->index(row, col));
            if (widget) {
                widget->setEnabled(true);
            }
        }
    }

    for (int row = outputClockCount; row <= model->rowCount() - 1; ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            if ((row == outputClockCount) && (col == 0)) { // 仅保留该行的Checkbox启用
                continue;
            }
            QWidget *widget = tableView->indexWidget(model->index(row, col));
            if (widget) {
                widget->setEnabled(false);
                if (col == 0) {
                    QCheckBox *outputClockCheckBox = widget->findChild<QCheckBox *>();
                    if (outputClockCheckBox && outputClockCheckBox->isChecked()) {
                        outputClockCheckBox->setChecked(false);
                    }
                }
            }
        }
    }

    QJsonObject rootObject;
    QJsonObject baseObject;
    baseObject["nums"] = outputClockCount;
    rootObject["base"] = baseObject;

    QJsonArray outputClockContentArray;

    for (int i = 0; i < outputClockCount; i++) {
        QJsonObject clockObject;

        // Output Clock 名称
        // clockObject["output_clock"] = QString("clk_out%1").arg(i);
        clockObject.insert("output_clock", QString("clk_out%1").arg(i));

        // Port Name
        QLineEdit *portNameEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 1)));
        // clockObject["port_name"] = portNameEdit->text();
        clockObject.insert("port_name", portNameEdit->text());

        // Output Frequency
        QLineEdit *freqEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 2)));
        // clockObject["output_freq"] = freqEdit->text().toDouble();
        clockObject.insert("output_freq", freqEdit->text().toDouble());

        // Phase
        QLineEdit *phaseEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 4)));
        // clockObject["phase"] = phaseEdit->text().toDouble();
        clockObject.insert("phase", phaseEdit->text().toDouble());

        // Duty Cycle
        QLineEdit *dutyCycleEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 6)));
        // clockObject["duty_cycle"] = dutyCycleEdit->text().toDouble();
        clockObject.insert("duty_cycle", dutyCycleEdit->text().toDouble());

        // Drive Type
        QComboBox *driveCombo = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(i, 8)));
        // clockObject["drive"] = driveCombo->currentText();
        clockObject.insert("drive", driveCombo->currentText());

        clockObject.insert("CLKOUT_DIVIDE", "18");

        // UseFine PS
        // QCheckBox *finePsCheckBox = qobject_cast<QCheckBox*>(tableView->indexWidget(model->index(i, 9)));
        // clockObject["use_fine_ps"] = finePsCheckBox->isChecked();
        QWidget *widget = tableView->indexWidget(model->index(i, 9));
        if (widget) {
            QCheckBox *finePsCheckBox = widget->findChild<QCheckBox *>();
            if (finePsCheckBox) {
                // clockObject["use_fine_ps"] = finePsCheckBox->isChecked();
                clockObject.insert("use_fine_ps", finePsCheckBox->isChecked());
            }
        }

        outputClockContentArray.append(clockObject);
    }
    rootObject["output_clock_content"] = outputClockContentArray;

    QJsonDocument doc(rootObject);
    output_clock_infor = doc.toJson(QJsonDocument::Indented);
    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/output_clock_infor.json"));
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    if(file.isOpen()) {
        QTextStream in(&file);
        in << output_clock_infor;//向文件写入数据
        file.close();
    }
    qDebug() << output_clock_infor;
}


