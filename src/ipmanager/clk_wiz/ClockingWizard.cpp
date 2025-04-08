#include "ClockingWizard.h"
#include "utils/ProjectManager.h"
#include "ipmanager/common/SetNameUtils.h"

ClockingWizard::ClockingWizard(QWidget *parent) :
    BaseDialog(parent)
{
    displayNameLabel->setText("Clocking Wizard(6.0)");
    QString componentName = QString("clk_wiz_%1").arg(SetNameUtils::getComponentNameIndex());
    componentNameLineEdit->setText(componentName);

    clockingOptionsWidget = new ClockingOptionsWidget(inputClockInformationTableView, inputClockInformationModel, this);
    outputClocksWidget = new OutputClocksWidget(outputClockInforTableView, outputClockInforModel, this);
    portRenamingWidget = new PortRenamingWidget(this);
    mmcmSettingsWidget = new MMCMSettingsWidget(this);
    summaryWidget = new ClkWizSummary(this);

    tabWidget->addTab(clockingOptionsWidget, "Clocking Options");
    tabWidget->addTab(outputClocksWidget, "Output Clocks");
    tabWidget->addTab(portRenamingWidget, "Port Renaming");
    tabWidget->addTab(mmcmSettingsWidget, "MMCM Settings");
    tabWidget->addTab(summaryWidget, "Summary");

    setup_core_generation_info(nullptr, nullptr);

    updateCoreGenerationInfoJson(nullptr, nullptr);

    setup_input_clock_infor(inputClockInformationTableView, inputClockInformationModel);

    updateInputClockInforJson(inputClockInformationTableView, inputClockInformationModel);

    setup_output_clock_infor(outputClockInforTableView, outputClockInforModel);

    updateOutputClockInforJson(outputClockInforTableView, outputClockInforModel,-1,-1, "init");

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
                updateOutputClockInforJson(tableView, model, i, 0, "Output Clock");
            });
        }

        // 监听 "Port Name"
        QLineEdit *portNameEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 1)));
        QObject::connect(portNameEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model, i, 1, "Port Name");
        });

        // 监听 "Output Freq(MHz) Requested"
        QLineEdit *freqEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 2)));
        QObject::connect(freqEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model, i, 2, "Output Freq");
        });

        // 监听 "Phase (degrees) Requested"
        QLineEdit *phaseEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 4)));
        QObject::connect(phaseEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model, i, 4, "Phase");
        });

        // 监听 "Duty Cycle (%) Requested"
        QLineEdit *dutyCycleEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 6)));
        QObject::connect(dutyCycleEdit, &QLineEdit::textChanged, [=]() {
            updateOutputClockInforJson(tableView, model, i, 6, "Duty Cycle");
        });

        // 监听 "Drive"
        QComboBox *driveCombo = qobject_cast<QComboBox*>(tableView->indexWidget(model->index(i, 8)));
        QObject::connect(driveCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]() {
            updateOutputClockInforJson(tableView, model, i, 8, "Drive");
        });

        // 监听 "UseFine PS"
        if (tableView->indexWidget(model->index(i, 9))) {
            QCheckBox *finePsCheckBox =  tableView->indexWidget(model->index(i, 9))->findChild<QCheckBox *>();
            QObject::connect(finePsCheckBox, &QCheckBox::stateChanged, [=]() {
                updateOutputClockInforJson(tableView, model, i, 9, "UseFine PS");
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

    inputJsonRoot["Primary"] = primary;

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

    inputJsonRoot["Secondary"] = secondary;

    QJsonDocument doc(inputJsonRoot);
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

void ClockingWizard::updateOutputClockInforJson(QTableView *tableView, QStandardItemModel *model, int row_num, int col_num, QString cur_group)
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
        clockObject.insert("phase", phaseEdit->text().toDouble());

        // Phase Actual
        QVariant phaseData = model->data(model->index(i, 5));
        clockObject.insert("phase_actual", phaseData.toDouble());

        // Duty Cycle
        QLineEdit *dutyCycleEdit = qobject_cast<QLineEdit*>(tableView->indexWidget(model->index(i, 6)));
        clockObject.insert("duty_cycle", dutyCycleEdit->text().toDouble());

        // Duty Cycle Actual
        QVariant dutyCycleData = model->data(model->index(i, 7));
        clockObject.insert("duty_cycle_actual", dutyCycleData.toDouble());

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

//    qDebug() << "outputClockContentArray : " << outputClockContentArray;

    // ===================== 动态调整 开始 ==============================
    if (cur_group == "Output Freq" || cur_group == "Phase" || cur_group == "Duty Cycle"){
        // 只有当这些内容变化时，才进行计算
         qDebug() << "[ClockingWizard] Dynamic adjustment";
        // Primary 输入频率
        QJsonObject primaryObject = inputJsonRoot["Primary"].toObject();
        double inputFrequency = primaryObject["input_frequency"].toDouble();
//        qDebug() << "input_frequency:" << inputFrequency;
        // 寻找所有可能的v m d
        std::vector<std::tuple<double, double, double>> v_m_d_lists = find_possible_m_d(inputFrequency);
//

        std::vector<std::tuple<double, double, double, std::vector<double>>> v_m_d_o_lists = find_possible_o(outputClockContentArray, v_m_d_lists);

//        for (const auto& [vco, m, d, o] : v_m_d_o_lists) {
//            qDebug() << " vco : " << vco << ", "<< " m : " << m << ", "<< " d : " << d;
//        }
        // 定义存储结构
        struct OutputData {
            double phase_value;
            double phase_error;
            double duty_cycle_value;
            double duty_cycle_error;
        };

        // 当前最小误差
        double best_min_phase_error = std::numeric_limits<double>::max();
        // 最优解的v m d
        std::map<std::string, double> the_best_group_parameter;
        // 最优解的各端口o(divide)
        std::vector<double> the_best_group_divide_list;
        // 最优解的相移占空比
        std::vector<OutputData> the_best_group_phase_duty;

        for (auto &[v, m, d, o_list] : v_m_d_o_lists) {
            std::vector<OutputData> cur_group(outputClockCount); // 当前的 group 存储
            bool valid_group = true; // 标记当前组是否有效
            // 使用当前组v m d o，与所有端口进行匹配
            for (int output_clock_index = 0; output_clock_index < outputClockCount; ++output_clock_index) {
                QJsonValue clk_content_value = outputClockContentArray.at(output_clock_index);
                QJsonObject clk_content = clk_content_value.toObject();
                // 拿到用户期望数值
                double output_freq = clk_content["output_freq"].toDouble();
                double phase_target = clk_content["phase"].toDouble();
                double duty_cycle_target = clk_content["duty_cycle"].toDouble();

                // 调用计算函数
                // 得到当前参数下，最接近的相移，phase_error为-0.1时，表示无法得到相移
                auto [closest_phase, phase_error] = find_closest_phase(v, output_freq, phase_target);
                // 得到当前参数下，最接近的占空比，当 HT 或者 LT 为 0时，表示无法得到占空比
                auto [HT, LT, closest_duty_cycle, duty_cycle_error] = find_closest_duty_cycle(o_list[output_clock_index], duty_cycle_target);
                // 当 得到一组占空比和相移时
                if (phase_error >= 0 && HT != 0 && LT != 0 && duty_cycle_error >= 0) {
                    // 相移保留三位小数四舍五入，并记录对应的偏差
                    // 占空比保留五位小数四舍五入，并记录对应的偏差
                    cur_group[output_clock_index] = {
                            round(closest_phase * 1000) / 1000,
                            phase_error,
                            round(closest_duty_cycle * 100000) / 100000,
                            duty_cycle_error
                    };
                } else {
                    valid_group = false;
                    break;
                }
            }

            // 如果该组参数能够得到满足所有端口的相移和占空比
            if (valid_group) {
                // 计算所有端口相移误差和，相移的优先级更高
                double cur_phase_error = 0;
                for (const auto &data : cur_group) {
                    cur_phase_error += data.phase_error;
                }

                // 如果这一组的误差小于当前最小误差
                if (best_min_phase_error >= cur_phase_error) {
                    // 更新当前最小误差
                    best_min_phase_error = cur_phase_error;

                    // 将其记录为最优解
                    the_best_group_parameter["vco"] = v;
                    the_best_group_parameter["m"] = m;
                    the_best_group_parameter["d"] = d;
                    the_best_group_divide_list = o_list;
                    the_best_group_phase_duty = {};
                    for (const auto &data : cur_group) {
                        the_best_group_phase_duty.push_back(data);
                    }
                }
            }
        }

        if (the_best_group_phase_duty.empty()){
            // 无解时报错

        } else {

        }

        int table_data_index = 0;
        for (const auto &item : the_best_group_phase_duty) {
            qDebug() << " phase_value : "<< item.phase_value;
            qDebug() << " duty_cycle_value : "<< item.duty_cycle_value;
            // 更新 phase actual
            if (outputClockInforTableView->modifyCell(table_data_index,5,item.phase_value)) {
                QJsonValue item_value = outputClockContentArray[table_data_index];
                QJsonObject item_obj = item_value.toObject();
                item_obj["phase_actual"] = std::round(item.phase_value * 1000.0) / 1000.0;  // 修改 phase_actual

                outputClockContentArray.replace(table_data_index, item_obj);  // 将修改后的对象写回
                qDebug() << "Cell updated successfully.";
            } else {
                qDebug() << "Failed to update cell.";
            }
            // 更新 duty actual
            if (outputClockInforTableView->modifyCell(table_data_index,7,item.duty_cycle_value*100)) {
                QJsonValue item_value = outputClockContentArray[table_data_index];
                QJsonObject item_obj = item_value.toObject();
                item_obj["duty_cycle_actual"] = std::round(item.duty_cycle_value*100 * 1000.0) / 1000.0;  // 修改 duty_cycle_actual
                outputClockContentArray.replace(table_data_index, item_obj);  // 将修改后的对象写回
                qDebug() << "Cell updated successfully.";
            } else {
                qDebug() << "Failed to update cell.";
            }
            table_data_index++;
        }
        qDebug() << "vco" << the_best_group_parameter["vco"];
        qDebug() << "m" << the_best_group_parameter["m"];
        qDebug() << "d" << the_best_group_parameter["d"];
        qDebug() << "动态调整 结束";
    }
    // ===================== 动态调整 结束 ==============================

    rootObject["output_clock_content"] = outputClockContentArray;
//    qDebug() << "rootObject";
//    qDebug() << rootObject;

    QJsonDocument doc(rootObject);
    output_clock_infor = doc.toJson(QJsonDocument::Indented);
    QFile file(QDir(ProjectManager::instance().getParameter(Project::Path)).filePath("runs/.works/output_clock_infor.json"));
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    if(file.isOpen()) {
        QTextStream in(&file);
        in << output_clock_infor;//向文件写入数据
        file.close();
    }
//    qDebug() << output_clock_infor;
}

// 函数：寻找最接近的占空比
std::tuple<int, int, double, double> ClockingWizard::find_closest_duty_cycle(double clk_out_divide, double duty_cycle_target) {
    int best_HT = 0, best_LT = 0;
    double closest_duty_cycle = 0.0;
    double min_error = std::numeric_limits<double>::max();

    for (int HT = 1; HT <= 63; ++HT) { // HT 的取值范围
        int LT = clk_out_divide - HT; // 根据 in1 得到 y，这里是准确解
        if (LT >= 1 && LT <= 63) { // 确保 LT 在范围内
            // 偶数直接除，奇数加0.5
            double actual_duty_cycle = (std::fmod(clk_out_divide, 2.0) < 0.0001)
                                ? (LT*1.0 / (HT + LT))
                                : ((LT + 0.5) / (HT + LT));

            double error = abs(actual_duty_cycle - duty_cycle_target/100); // 计算误差
            if (error < min_error) { // 找到更小误差时更新结果
                best_HT = HT;
                best_LT = LT;
                closest_duty_cycle = actual_duty_cycle; // 更新最逼近的占空比
                min_error = error; // 更新最小误差
            }
        }
    }
    return {best_HT, best_LT, closest_duty_cycle, min_error};
}

// 函数：计算最接近的 Phase
std::pair<double, double> ClockingWizard::find_closest_phase(double vco, double clk_out, double phase_target) {
    double closest_phase = -0.1; // 默认-0.1
    double min_error = std::numeric_limits<double>::max();

    for (int i = 0; i <= 800; ++i) { // 800 步，代表 0.125 到 100,000
        double k = i * 0.125;
        double phase = (k * clk_out * 360) / vco; // 计算 phase
        double error = std::abs(phase - phase_target); // 计算差值

        if (error < min_error) {
            // 为当前最小误差时更新
            closest_phase = phase;
            min_error = error;
        }
        if (error < 1e-9) break; // 误差足够小或者相等，跳出循环
    }

    return {closest_phase, min_error};
}

// 函数：查找满足条件的 O_n
std::vector<std::tuple<double, double, double, std::vector<double>>>
ClockingWizard::find_possible_o(const QJsonArray& f_out_group,
                                const std::vector<std::tuple<double, double, double>> &vco_m_d_group) {
    std::vector<std::tuple<double, double, double, std::vector<double>>> results;
    // 一组一组的遍历 vco m d
    for (auto& [vco, m, d] : vco_m_d_group) {
        std::vector<double> o_n_list;
        bool valid_group = true; // 标记当前组是否有效
        for (const auto& out_group : f_out_group) {
            if (out_group.isObject()) {
                QJsonObject clk_out_oj = out_group.toObject();
                double output_freq = clk_out_oj["output_freq"].toDouble();
                if (fmod(vco, output_freq) == 0.0){
                    o_n_list.push_back(vco / output_freq);
                } else {
                    valid_group = false;
                    break;
                }
            }
        }
        if (valid_group) {
            results.emplace_back(vco, m, d, o_n_list);
        }
    }
    return results;
}

std::vector<std::tuple<double, double, double>> ClockingWizard::find_possible_m_d(double f_in, bool is_integer, std::vector<int> possible_d,
                                       std::pair<int, int> f_vco_range, std::pair<int, int> m_range,
                                       std::pair<int, int> d_range) {
    // 记录能够满足条件的 m 和 d 的组合
    std::vector<std::tuple<double, double, double>> results;
    // 取 vco 范围，不同型号芯片，vco范围不同
    int f_vco_min = f_vco_range.first;
    int f_vco_max = f_vco_range.second;
    // 取 m 和 d 范围，当前默认是 m 1-64, d 1-8
    int m_min = m_range.first, m_max = m_range.second;
    int d_min = d_range.first, d_max = d_range.second;
    // possible_d 不为空时，优先考虑d在这个范围上的取值可能
    if (!possible_d.empty()) {
        for (int d : possible_d) {
            if (d < d_min || d > d_max) continue; // 越界不考虑
            for (int m = m_min; m <= m_max; ++m) {
                double f_vco = m * f_in / d; // 当 vco 的频率在范围内时，就算是满足
                // fmod(f_vco, 1.0) 取 f_vco 的小数部分，如果小数部分为 0.0，表示其为整数
                if (fmod(f_vco, 1.0) == 0.0 && f_vco >= f_vco_min && f_vco <= f_vco_max) {
                    results.emplace_back(f_vco, m, d); // 满足条件则把该组结果放入 results
                }
            }
        }
    } else {
        // possible_d 为空，则找所有取值可能
        for (int d = d_min; d <= d_max; ++d) {
            for (int m = m_min; m <= m_max; ++m) {
                double f_vco = m * f_in / d;
                if (fmod(f_vco, 1.0) == 0.0 && f_vco >= f_vco_min && f_vco <= f_vco_max) {
                    results.emplace_back(f_vco, m, d);
                }
            }
        }
    }
    return results;
}



