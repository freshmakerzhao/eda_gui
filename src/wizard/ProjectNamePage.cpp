#include "ProjectNamePage.h"

ProjectNamePage::ProjectNamePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Project Name");
    setSubTitle("Enter a name for your project and specify a directory "
                "where the project data files will be stored.");

    QLabel *nameLabel = new QLabel("Project name:");
    QLineEdit *nameLineEdit = new QLineEdit;
    registerField("projectName*", nameLineEdit);

    QLabel *pathLabel = new QLabel("Project path:");
    QLineEdit *pathLineEdit = new QLineEdit;
    QPushButton *browseButton = new QPushButton("Browse");
    connect(browseButton, &QPushButton::clicked, [=]() {
        QString path = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
        if (!path.isEmpty()) {
            pathLineEdit->setText(path);
        }
    });
    registerField("projectPath*", pathLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(pathLabel, 1, 0);
    layout->addWidget(pathLineEdit, 1, 1);
    layout->addWidget(browseButton, 1, 2);
    setLayout(layout);
}
