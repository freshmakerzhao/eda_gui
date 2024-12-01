#ifndef CLOCKINGWIZARD_H
#define CLOCKINGWIZARD_H


#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QGraphicsView>
#include <QToolBar>
#include <QSplitter>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include "ipmanager/common/BaseDialog.h"
#include "ipmanager/clk_wiz/ClockingOptionsWidget.h"
#include "ipmanager/clk_wiz/OutputClocksWidget.h"
#include "ipmanager/clk_wiz/PortRenamingWidget.h"
#include "ipmanager/clk_wiz/MMCMSettingsWidget.h"
#include "ipmanager/clk_wiz/ClkWizSummary.h"
#include "base/Globals.h"
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <algorithm>

class ClockingWizard : public BaseDialog
{
    Q_OBJECT
public:
    ClockingWizard(QWidget *parent = nullptr);
protected:
    void accept() override;
private:
    // QTabWidget *tabWidget;

    ClockingOptionsWidget *clockingOptionsWidget;

    OutputClocksWidget *outputClocksWidget;

    PortRenamingWidget *portRenamingWidget;

    MMCMSettingsWidget *mmcmSettingsWidget;

    ClkWizSummary *summaryWidget;

    // int out_clk_nums = 0;

    AdvancedTableView *inputClockInformationTableView = new AdvancedTableView;

    QStandardItemModel *inputClockInformationModel = new QStandardItemModel;

    AdvancedTableView *outputClockInforTableView = new AdvancedTableView;

    QStandardItemModel *outputClockInforModel = new QStandardItemModel(8, 11);



    void setup_core_generation_info(QTableView *tableView, QStandardItemModel *model);

    void setup_input_clock_infor(QTableView *tableView, QStandardItemModel *model);

    void setup_output_clock_infor(QTableView *tableView, QStandardItemModel *model);

    void updateCoreGenerationInfoJson(QTableView *tableView, QStandardItemModel *model);

    void updateInputClockInforJson(QTableView *tableView, QStandardItemModel *model);

    void updateOutputClockInforJson(QTableView *tableView, QStandardItemModel *model, int row_num, int col_num, QString cur_group);

    // 查找满足条件的M和D组合
    // :param f_in: 输入频率
    // :param is_integer: 是否是需要整数，PLL需要整数
    // :param possible_d: possible_d 有值时，优先考虑d在该范围上的m和d的组合
    // :param f_vco_range: VCO频率范围，元组 (min, max)
    // :param m_range: M的范围，元组 (min, max)
    // :param d_range: D的范围，元组 (min, max)
    // :return: 所有满足条件的 M, D 组合以及对应的 VCO (vco, m, d)
    std::vector<std::tuple<double, double, double>> find_possible_m_d(double f_in, bool is_integer = false, std::vector<int> possible_d = {},
                           std::pair<int, int> f_vco_range = {800, 2100},
                           std::pair<int, int> m_range = {1, 64},
                           std::pair<int, int> d_range = {1, 8});


    // 查找满足条件的 O_n
    // :param f_out_group: 各端口的输出频率
    // :param vco_m_d_group: vcd、m、d的合集
    // :return: 所有满足条件的 m、d、o 组合以及对应的 VCO (vco, m, d, o_n_list)
    std::vector<std::tuple<double, double, double, std::vector<double>>> find_possible_o(const QJsonArray& f_out_group,
                                                                    const std::vector<std::tuple<double, double, double>>& vco_m_d_group);

    // 找到最接近目标 phase 的解，使得公式 (vco * phase) / (clk_out * 360) = k 成立，且 k 为 0.125整数倍。
    // :param vco: VCO 的值（整数）
    // :param clk_out: clk_out 的值（可能是小数或整数）
    // :param phase_target: 用户期望的 phase（可能是小数或整数）
    // :return: 最接近的 phase, 对应的 误差 值
    std::pair<double, double> find_closest_phase(double vco, double clk_out, double phase_target);

    // 寻找满足条件的整数解 (HT, LT)，使得 LT/(HT+LT) 最接近 duty_cycle_target
    // :param clk_out_divide: 整数, 输出端口的分频系数
    // :param duty_cycle_target: 可为小数或整数, 期望的duty_cycle
    // :return: 最接近的 (HT, LT)、计算得到的实际 duty_cycle、以及误差
    std::tuple<int, int, double, double> find_closest_duty_cycle(double clk_out_divide, double duty_cycle_target);

    QString core_generation_info;

    QString input_clock_infor;

    QString output_clock_infor;

    QJsonObject inputJsonRoot;
};

#endif // CLOCKINGWIZARD_H
