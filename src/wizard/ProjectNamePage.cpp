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
    nameLineEdit->setClearButtonEnabled(true);
    registerField("projectName*", nameLineEdit);

    QLabel *pathLabel = new QLabel("Project path:");
    pathLineEdit = new QLineEdit(QDir::homePath());
    pathLineEdit->setClearButtonEnabled(true);
    registerField("projectPath*", pathLineEdit);

    nameLineEdit->setText(autoSetName(pathLineEdit->text()));

    QPushButton *browseButton = new QPushButton("Browse");
    connect(browseButton, &QPushButton::clicked, [=]() {
        QString path = AdvancedFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(), QFileDialog::DontUseNativeDialog);
        if (!path.isEmpty()) {
            pathLineEdit->setText(path);
        }
    });

    //! Close auto-naming after user input.
    //! setText() will not trigger this signal.
    connect(nameLineEdit, &QLineEdit::textEdited, [this]() {
        enableAutoSetName = false;
    });

    connect(pathLineEdit, &QLineEdit::textChanged, [this]() {
        if (enableAutoSetName) {
            nameLineEdit->setText(autoSetName(pathLineEdit->text()));
        }
    });

    hintLabel = new QLabel;

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(pathLabel, 1, 0);
    layout->addWidget(pathLineEdit, 1, 1);
    layout->addWidget(browseButton, 1, 2);
    layout->addWidget(hintLabel, 2, 0, 1, 2);
}

bool ProjectNamePage::isComplete() const
{
    QString projectName = nameLineEdit->text();
    QString projectPath = pathLineEdit->text();

    if (projectName.isEmpty()) {
        hintLabel->setStyleSheet("color: red;");
        hintLabel->setText("Project name is empty!");
        return false;
    }

    if (projectPath.isEmpty()) {
        hintLabel->setStyleSheet("color: red;");
        hintLabel->setText("Project path is empty!");
        return false;
    }

    // 检查路径下是否存在相同的名称
    QDir dir(projectPath);
    if (dir.exists(projectName)) {
        hintLabel->setStyleSheet("color: red;");
        hintLabel->setText("Same name folder already exists in the selected path!");
        return false;
    }

    // 检查工程名是否合法
    bool isEnglishPath = QRegularExpression("^[a-zA-Z0-9_]+$").match(projectName).hasMatch();
    if (!isEnglishPath) {
        hintLabel->setStyleSheet("color: red;");
        hintLabel->setText("Non-English Project name");
        return false;
    }

    // 通过检查
    hintLabel->setStyleSheet("color: black;");
    hintLabel->setText(QString("Project will be created at %1").arg(dir.filePath(projectName)));
    return true;
}

QString ProjectNamePage::autoSetName(const QString &projectDir)
{
    QDir dir(projectDir);
    unsigned long long int number = 1;
    QString result;
    do {
        result = QString("project_%1").arg(number);
        number++;
    } while (dir.exists(result));
    return result;
}

