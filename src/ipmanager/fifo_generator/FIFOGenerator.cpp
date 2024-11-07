#include "FIFOGenerator.h"

FIFOGenerator::FIFOGenerator(QWidget *parent) :
    BaseDialog(parent)
{
    basic = new Basic;

    tabWidget->addTab(basic, "Basic");
}
