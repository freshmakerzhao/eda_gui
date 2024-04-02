/**
  ******************************************************************************
  * @file           : Wizard.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/9
  ******************************************************************************
  */
#include "Wizard.h"
#include "ProjectNamePage.h"
#include "SourcePage.h"
#include "ConstraintPage.h"
#include "DefaultPartPage.h"
#include "navigator.h"

Wizard::Wizard(QWidget *parent) : QWizard(parent)
{
    qDebug() << "[Wizard] Constructing...";
    setWindowTitle("Wizard");
    setWizardStyle(QWizard::ModernStyle);
    addPage(new ProjectNamePage);
    addPage(new SourcesPage);
    addPage(new ConstraintPage);
    addPage(new DefaultPartPage);
    connect(this, &QWizard::accepted, this, &Wizard::onFinish);

    resize(900, 600);
}

Wizard::~Wizard()
{
    qDebug() << "[Wizard] Distructing...";
}

void Wizard::onFinish()
{
    // 获取项目名称和路径
    QString projectName = field("projectName").toString();
    QString projectPath = field("projectPath").toString();

    // 创建项目文件夹
    QDir dir(projectPath);
    if (dir.mkdir(projectName)) {
        dir.cd(projectName);
        dir.mkdir("sources");
        dir.mkdir("constraints");
        dir.mkdir("doc");
        dir.mkdir("ip");

        // 创建runs文件夹
        if (dir.mkdir("runs")) {
            // cd进入runs文件夹
            dir.cd("runs");
            // 在runs文件夹内创建.works、impl和synth文件夹
            dir.mkdir(".works"); // 记录中间过程，方便后续在此基础上继续执行
            dir.mkdir("impl"); // pack place route
            dir.mkdir("synth"); // synth
            qDebug() << "Folders Created Successfully including runs/.works、runs/impl and runs/synth";
        } else {
            qDebug() << "Failed to create runs folder";
        }

        // 返回到原来的项目文件夹路径下
        dir.cdUp();
    } else {
        qDebug() << "Folders Created Unsuccessfully";
        return;
    }

    QString targetPath =  projectPath + "/" + projectName;
    // 复制文件列表中的文件到项目文件夹sources
    foreach (const QString &file, sourcesFilesList) {
        QFile::copy(file, targetPath + "/sources/" + QFileInfo(file).fileName());
    }
    // 复制文件列表中的文件到项目文件夹constrains
    foreach (const QString &file, constraintFilesList) {
        QFile::copy(file, targetPath + "/constraints/" + QFileInfo(file).fileName());
    }

    // ======================== 记录复制文件后的文件位置 =========================
    QStringList sourcetmp,constrainttmp;
    for (const QString& source : sourcesFilesList) {
        QString newPath =  targetPath + "/sources/" + QFileInfo(source).fileName();
        sourcetmp.append(newPath);
    }
    for (const QString& constraint : constraintFilesList) {
        QString newPath =  targetPath + "/constraints/" + QFileInfo(constraint).fileName();
        constrainttmp.append(newPath);
    }
    // ============================= 生成工程 =================================
    project = new Project(projectName, targetPath, part, arch, archName);
    project->sourceList = sourcetmp;
    project->constraintList = constrainttmp;
    project->makeProject();
    Navigator::instance()->loadFile(project);

    // ============================= 清除缓存 =================================
    QDir dircache("Cache");
    if (!dircache.isEmpty()) {
        dircache.removeRecursively();
    }
}



