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
#include "NewGuidePage.h"
#include "AddGuidePage.h"
#include "ProjectNamePage.h"
#include "SourcePage.h"
#include "ConstraintPage.h"
#include "DefaultPartPage.h"
#include "widgets/Navigator.h"

Wizard::Wizard(QWidget *parent, const int mode, Project *pro) : QWizard(parent)
{
    qDebug() << "[Wizard] Constructing...";
    resize(960, 640);
    setWindowTitle("Wizard");
    // setWizardStyle(QWizard::NStyles);
    // QPixmap pix(":/resource/icon.png");
    // setPixmap(QWizard::LogoPixmap, pix);
    // setOption(QWizard::IndependentPages, true);
    switch (mode) {
    case 0:     // 完整新建工程流程
        qDebug() << "[Wizard] mode 0";
        addPage(new NewGuidePage);
        addPage(new ProjectNamePage);
        addPage(new SourcesPage);
        addPage(new ConstraintPage);
        addPage(new DefaultPartPage);
        connect(this, &QWizard::accepted, this, &Wizard::onNewFinish);
        break;
    case 1:     // 添加Sources
        current_project = pro;
        qDebug() << "[Wizard] mode 1";
        setPage(Page_AddGuide, new AddGuidePage);
        setPage(Page_Source, new SourcesPage(this, 1));
        setPage(Page_Constraint, new ConstraintPage);
        connect(this, &QWizard::accepted, this, &Wizard::onAddFinish);
        break;
    default:
        break;
    }
}

Wizard::~Wizard()
{
    qDebug() << "[Wizard] Distructing...";
}

void Wizard::onNewFinish()
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
    new_project = new Project(projectName, targetPath, part, arch, archName);
    new_project->sourceList = sourcetmp;
    new_project->constraintList = constrainttmp;
    new_project->makeProject();
    Navigator::instance()->loadFile(new_project);

    // ============================= 清除缓存 =================================
    QDir dircache("Cache");
    if (!dircache.isEmpty()) {
        dircache.removeRecursively();
    }
}

void Wizard::onAddFinish()
{
    qDebug() << "Add Finish";
    if(current_project == nullptr) {
        return;
    }
    QString path = current_project->getParam("path");
    qDebug() << path;
    QString addSourcesPath = path + "/sources/";
    QStringList files = sourcesFilesList;
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, addSourcesPath + QFileInfo(file).fileName());
            current_project->sourceList.append(addSourcesPath + QFileInfo(file).fileName());
        }
    }

    QString constrainsPath = path + "/constraints/";
    files = constraintFilesList;
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, constrainsPath + QFileInfo(file).fileName());
            current_project->constraintList.append(constrainsPath + QFileInfo(file).fileName());
        }
    }
}



