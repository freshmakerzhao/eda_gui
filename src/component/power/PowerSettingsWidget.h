#ifndef POWERSETTINGSWISGET_H
#define POWERSETTINGSWISGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "utils/json.hpp"

class PowerSettingsWidget : public QWidget
{
public:
    PowerSettingsWidget(QWidget *parent = nullptr);

    void loadData(nlohmann::basic_json<nlohmann::ordered_map> j);

private:
    const QString TITLE_LABEL_STYLESHEET = "QLabel{"
                                           "font-size: 20px;"
                                           "font-weight: bold;"
                                           "border-bottom: 2px solid #DCDCDC;"
                                           "border-radius: 0px;"
                                           "}";
};

#endif // POWERSETTINGSWISGET_H
