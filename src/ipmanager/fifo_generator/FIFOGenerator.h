#ifndef FIFOGENERATOR_H
#define FIFOGENERATOR_H

#include "ipmanager/common/BaseDialog.h"
#include "ipmanager/fifo_generator/Basic.h"

class FIFOGenerator : public BaseDialog
{
    Q_OBJECT
public:
    FIFOGenerator(QWidget *parent = nullptr);

private:
    Basic *basic;
};

#endif // FIFOGENERATOR_H
