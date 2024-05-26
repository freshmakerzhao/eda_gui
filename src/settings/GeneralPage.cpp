#include "GeneralPage.h"
#include "utils/TaskManager.h"

GeneralPage::GeneralPage(QWidget *parent)
{
    setStyleSheet(".QWidget { background-image: url(:/resource/white.png); }"
                  ".QWidget { border:4px solid #DCDCDC; }"
                  );
    // project_device = new QLineEdit(this);
    top_module_name = new QLineEdit(this);
    fLayout = new QFormLayout(this);
    // fLayout->addRow("Project device:", project_device);
    fLayout->addRow("Top module name:", top_module_name);

    QString topName = TaskManager::instance().getTopModule();
    top_module_name->setText(topName);


}

void GeneralPage::setTopModule()
{
    TaskManager::instance().setTopModule(top_module_name->text());
}
