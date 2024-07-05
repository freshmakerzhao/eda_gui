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
#include "utils/ProjectManager.h"

Wizard::Wizard(QWidget *parent, const int &mode) : QWizard(parent)
{
    qDebug() << "[Wizard] Constructing...";
    resize(960, 640);
    setWindowTitle("Wizard");
    // setWizardStyle(QWizard::NStyles);
    // QPixmap pix(":/resource/icon.png");
    // setPixmap(QWizard::LogoPixmap, pix);
    // setOption(QWizard::IndependentPages, true);

#ifdef Q_OS_WIN
    // 设置自定义按钮可用
    setOption (QWizard::HaveCustomButton1, true);
    // 获取并设置自定义按钮的文本
    auto *customButton = this->button(QWizard::CustomButton1);
    customButton->setText("Back");
    // 连接自定义按钮的点击信号到槽函数
    connect(customButton, &QPushButton::clicked, this, &Wizard::back);
    // 绑定来更新按钮状态
    connect(this, &QWizard::currentIdChanged, this, [=](int id) {
        customButton->setEnabled(id != 0);
    });
    // 初始化按钮状态
    customButton->setEnabled(currentId() != 0);
#endif

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

    QString targetPath = QString("%1/%2").arg(projectPath, projectName);
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
    ProjectManager::instance().createProject(projectName,
                                             targetPath,
                                             part,
                                             arch,
                                             archName,
                                             sourcetmp,
                                             constrainttmp,
                                             displayPart,
                                             familyName);
    // ============================= 清除缓存 =================================
    QDir dircache("Cache");
    if (!dircache.isEmpty()) {
        dircache.removeRecursively();
    }
}

void Wizard::onAddFinish()
{
    qDebug() << "Add Finish";
    if (!sourcesFilesList.empty()) {
        ProjectManager::instance().addSourcesInProject(sourcesFilesList, 0);
    }

    if (!constraintFilesList.empty()){
        ProjectManager::instance().addSourcesInProject(constraintFilesList, 1);
    }
}



