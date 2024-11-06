#ifndef BLKMEMGENSUMMARY_H
#define BLKMEMGENSUMMARY_H

#include "ipmanager/common/BasePage.h"

class BlkMemGenSummary : public BasePage
{
    Q_OBJECT
public:
    BlkMemGenSummary(QWidget *parent = nullptr);

    void setMemoryTypeInformation(const QString &option);

private:
    const QString titleLabelQss = "QLabel{"
                                  "font-size: 20px;"
                                  "font-weight: bold;"
                                  "border-bottom: 2px solid #DCDCDC;"
                                  "border-radius: 0px;" // 不能删除，我也不知道为什么
                                  "}";

    QLabel *memoryTypeLabel;
    QLabel *AddressWidthALabel;
};

#endif // BLKMEMGENSUMMARY_H
