/**
  ******************************************************************************
  * @file           : ProjectNamePage.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/3/26
  ******************************************************************************
  */

#include "ProjectNamePage.h"
#include "dialog/AdvancedFileDialog.h"

ProjectNamePage::ProjectNamePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Project Name");
    setSubTitle("Enter a name for your project and specify a directory "
                "where the project data files will be stored.");

    QLabel *nameLabel = new QLabel("Project name:");
    nameLineEdit = new QLineEdit;
    nameLineEdit->setText("project_1");
    registerField("projectName*", nameLineEdit);

    QLabel *pathLabel = new QLabel("Project path:");
    pathLineEdit = new QLineEdit;
    pathLineEdit->setText(QDir::homePath());
    QPushButton *browseButton = new QPushButton("Browse");
    connect(browseButton, &QPushButton::clicked, [=]() {
        // QString path = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
        QString path = AdvancedFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(), QFileDialog::DontUseNativeDialog);
        if (!path.isEmpty()) {
            pathLineEdit->setText(path);
        }
    });
    registerField("projectPath*", pathLineEdit);

    warningLabel = new QLabel;
    warningLabel->setStyleSheet("color: red;");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(pathLabel, 1, 0);
    layout->addWidget(pathLineEdit, 1, 1);
    layout->addWidget(browseButton, 1, 2);
    layout->addWidget(warningLabel, 2, 1);
    setLayout(layout);
}

bool ProjectNamePage::isComplete() const
{
    QString projectName = nameLineEdit->text();
    QString projectPath = pathLineEdit->text();

    if (projectName.isEmpty()) {
        warningLabel->setText("Project name is empty!");
        return false;
    }

    if (projectPath.isEmpty() || !QDir(projectPath).exists()) {
        warningLabel->setText("Project path is empty or does not exist!");
        return false;
    }

    // 检查路径下是否存在相同的名称
    QDir projectDir(projectPath);
    QStringList entryList = projectDir.entryList();
    if (entryList.contains(projectName, Qt::CaseSensitive)) {
        warningLabel->setText("Same name folder already exists in the selected path!");
        return false;
    }

    // 检查工程名是否合法
    bool isEnglishPath = QRegularExpression("^[a-zA-Z0-9_]+$").match(projectName).hasMatch();
    if (!isEnglishPath) {
        warningLabel->setText("Non-English Project name");
        return false;
    }

    // 通过检查
    warningLabel->setText("");
    return true;


}
