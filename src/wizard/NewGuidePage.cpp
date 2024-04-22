/**
  ******************************************************************************
  * @file           : NewGuidePage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/20
  ******************************************************************************
  */

#include "NewGuidePage.h"

NewGuidePage::NewGuidePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Create a New HybrdLink Project");
    setSubTitle("This wizard will guide you through the creation of a new project.\n"
                "To create a HybrdLink project you will need to provide a name and a "
                "location for your project files.Finally, you will specify your project "
                "sources and choose a default part.");
}
