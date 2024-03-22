#include "navigator.h"
#include "mainwindow.h"
#include "taskmanager.h"
#include "utils/StringUtilities.h"

Navigator *Navigator::instance(QWidget *parent)
{
    static Navigator *m_instance = nullptr;
    if (!m_instance) {
        m_instance = new Navigator(parent);
    }
    return m_instance;
}

void Navigator::loadFile(Project *proj)
{
    /*如果加载的是同一个工程,刷新工程文件表,否则运行新进程,在新进程加载工程*/
    if(p != nullptr) { // 已加载工程
        if (p != proj) { // 已加载的是其他工程
            // 获取程序路径
            QString programPath = QCoreApplication::applicationFilePath();
            // 创建一个新进程
            QProcess *process = new QProcess();
            // 传入新打开工程路径
            QString hprfile = proj->path +  "/" + proj->name + ".hpr";
            qDebug() << hprfile;
            // 启动程序本身
            process->start(programPath, QStringList() << hprfile);
            return;
        }
    } else { // 未加载工程
        p = proj;
    }
    // 存储设计文件与约束文件
    TaskManager::instance()->sourcePathList = proj->sourceList;
    TaskManager::instance()->constraintPathList = proj->constraintList;
    // 存储路径
    TaskManager::instance()->projectSynthPath = proj->path + "/runs/synth";
    TaskManager::instance()->projectImplPath = proj->path + "/runs/impl";
    TaskManager::instance()->projectPath = proj->path;
    // 存储partname
    TaskManager::instance()->partName = proj->part;
    TaskManager::instance()->archName = proj->archName;
    TaskManager::instance()->arch = proj->arch;
    // 测试用
     // TaskManager::instance()->GLOBAL_RESOURCE_PATH = "E:/workspace/qt_demo/resource_win";
    // 打包用
    TaskManager::instance()->GLOBAL_RESOURCE_PATH = QString::fromStdString(StringUtilities::concatPath({QCoreApplication::applicationDirPath().toStdString(), "resource_win"}));


    qDebug() << "[Navigator] loadFile...";
    qDebug() << "[Navigator] proj->sourceList：" << proj->sourceList;
    qDebug() << "[Navigator] proj->constraintList：" << proj->constraintList;

    QTreeWidgetItem *nameItem = new QTreeWidgetItem(navTree);
    nameItem->setText(0, proj->name);
    QTreeWidgetItem *sourceItem = new QTreeWidgetItem(nameItem);
    sourceItem->setText(0, "sources");
    foreach (const QString &file, proj->sourceList) {
        QTreeWidgetItem *sourcefile = new QTreeWidgetItem(sourceItem);
        sourcefile->setText(0, QFileInfo(file).fileName());
        sourcefile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    QTreeWidgetItem *constraintsItem = new QTreeWidgetItem(nameItem);
    constraintsItem->setText(0, "constraints");
    foreach (const QString &file, proj->constraintList) {
        QTreeWidgetItem *constraintsfile = new QTreeWidgetItem(constraintsItem);
        constraintsfile->setText(0, QFileInfo(file).fileName());
        constraintsfile->setData(0, Qt::UserRole, QFileInfo(file).filePath());
    }
    // TODO:Load doc, ip
    QTreeWidgetItem *docItem = new QTreeWidgetItem(nameItem);
    docItem->setText(0, "doc");

    QTreeWidgetItem *ipItem = new QTreeWidgetItem(nameItem);
    ipItem->setText(0, "ip");
}

void Navigator::clickedFile(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    // qDebug() << "Navigator:: open " << path;
    MainWindow::instance()->createEditorTab(path);
}

void Navigator::showContextMenu(const QPoint &pos) {
    QMenu contextMenu;
    QAction closeProject("Close Project");
    QAction addSources("Add Sources");
    QAction addConstraints("Add Constraints");
    QAction deleteFileAction("Remove File from Project");
    if (navTree->currentItem() == nullptr) {
        // qDebug() << "Empty Item";
        return;
    }
    if (navTree->currentItem()->parent() == nullptr) {
        contextMenu.addAction(&closeProject);
        connect(&closeProject, &QAction::triggered, this, &Navigator::closeProjectAction);
        contextMenu.addAction(&addSources);
        connect(&addSources, &QAction::triggered, this, &Navigator::addSourcesAction);
        contextMenu.addAction(&addConstraints);
        connect(&addConstraints, &QAction::triggered, this, &Navigator::addConstraintsAction);
    } else if (QFileInfo(navTree->currentItem()->data(0, Qt::UserRole).toString()).isFile()) {
        contextMenu.addAction(&deleteFileAction);
        connect(&deleteFileAction, &QAction::triggered, this, &Navigator::removeFileAction);

    }
    contextMenu.exec(navTree->mapToGlobal(pos));
}

void Navigator::closeProjectAction()
{
    if (!MainWindow::instance()->cleanEditorTab()) {
        return;
    }
    delete navTree->topLevelItem(0);
    delete p;
    p = nullptr;
}

void Navigator::addSourcesAction()
{
    QString path = p->path;
    qDebug() << path;
    QString addSourcesPath = path + "/sources/";
    qDebug() << "addSources path:" << addSourcesPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, addSourcesPath + QFileInfo(file).fileName());
            p->sourceList.append(addSourcesPath + QFileInfo(file).fileName());
        }
    }
    p->makeProject();
    delete navTree->topLevelItem(0);
    loadFile(p);
}

void Navigator::addConstraintsAction()
{
    QString path = p->path;
    QString constrainsPath = path + "/constraints/";
    qDebug() << "constraints path:" << constrainsPath;
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files", "", "");
    if (!files.isEmpty()) {
        foreach (const QString &file, files) {
            QFile::copy(file, constrainsPath + QFileInfo(file).fileName());
            p->constraintList.append(constrainsPath + QFileInfo(file).fileName());
        }
    }
    p->makeProject();
    delete navTree->topLevelItem(0);
    loadFile(p);
}

void Navigator::removeFileAction()
{
    QString path = navTree->currentItem()->data(0, Qt::UserRole).toString();
    QFileInfo fileInfo(path);
    QString folderName = fileInfo.dir().dirName();
    qDebug() << folderName;
    if (folderName == "sources") {
        p->sourceList.removeOne(path);
    } else if (folderName == "constraints") {
        p->constraintList.removeOne(path);
    }

    QFile file(navTree->currentItem()->text(0));
    file.remove();
    delete navTree->currentItem();
    p->makeProject();
}

bool Navigator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == navTree->viewport()) {
        //点击树的空白,取消选中
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                QModelIndex index = navTree->indexAt(mouseEvent->pos());
                if (!index.isValid()) {
                    navTree->setCurrentIndex(QModelIndex());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

Navigator::Navigator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "[Navigator] Constructing...";
    navTree = new QTreeWidget(this);
    navTree->viewport()->installEventFilter(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(navTree);
    layout->setMargin(0);

    navTree->setContextMenuPolicy(Qt::CustomContextMenu);

    // 点击打开文件
    QObject::connect(navTree, &QTreeWidget::itemDoubleClicked, this, &Navigator::clickedFile);
    // 绑定右键菜单
    QObject::connect(navTree, &QTreeWidget::customContextMenuRequested, this, &Navigator::showContextMenu);


    navTree->setColumnCount(1);
    navTree->setHeaderHidden(true);
}

Navigator::~Navigator()
{
    qDebug() << "[Navigator] Distructing...";
}

