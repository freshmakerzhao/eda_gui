#ifndef FIFOGENERATOR_H
#define FIFOGENERATOR_H

#include "ipmanager/common/BaseDialog.h"
#include "ipmanager/fifo_generator/Basic.h"
#include "ipmanager/fifo_generator/NativePorts.h"
#include "ipmanager/fifo_generator/DataCounts.h"

class FIFOGenerator : public BaseDialog
{
    Q_OBJECT
public:
    FIFOGenerator(QWidget *parent = nullptr);

private:
    Basic *basic;

    NativePorts *nativePorts;

    DataCounts *dataCounts;
};

#endif // FIFOGENERATOR_H
