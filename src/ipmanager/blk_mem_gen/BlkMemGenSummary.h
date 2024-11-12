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
    QLabel *memoryTypeLabel;
    QLabel *AddressWidthALabel;
};

#endif // BLKMEMGENSUMMARY_H
