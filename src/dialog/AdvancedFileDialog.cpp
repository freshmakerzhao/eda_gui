#include "AdvancedFileDialog.h"
#include "utils/XmlUtilities.h"
#include "base/InitialConfig.h"

AdvancedFileDialog::AdvancedFileDialog(QWidget *parent) :
    QFileDialog(parent),
    metadataLabel(new QLabel("<b>Information</b>", this)),
    metadataEdit(new QTextEdit(promptText, this)),
    toolBar(new QToolBar(this))
{
    resize(1350, 600);
    setOption(QFileDialog::DontUseNativeDialog, true);
    setViewMode(QFileDialog::List);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // 删除问号，只保留关闭
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(metadataLabel);

    metadataEdit->setReadOnly(true);
    metadataEdit->setWordWrapMode(QTextOption::NoWrap);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(metadataEdit);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedWidth(400); // 设置固定宽度
    layout->addWidget(scrollArea);

    QHBoxLayout *hLayout = new QHBoxLayout;

    // Add the custom widget to the QFileDialog
    // 此对话框存在两个ListView，不能找错
    QGridLayout *gridLayout = qobject_cast<QGridLayout *>(this->layout());
    QSplitter *pSplitter =  this->findChild<QSplitter *>();
    QListView *listView = pSplitter->findChild<QListView *>();
    listView->setParent(nullptr);

    // 调整布局
    QStackedWidget *stackedWidget = this->findChild<QStackedWidget *>("stackedWidget");
    gridLayout->removeWidget(stackedWidget);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(listView);
    splitter->addWidget(stackedWidget);
    hLayout->addWidget(splitter);
    hLayout->addLayout(layout);
    gridLayout->addLayout(hLayout, 1, 0, 1, 3);

    connect(this, &QFileDialog::currentChanged, this, &AdvancedFileDialog::updateMetadata);

    // 添加工具栏
    auto *childLayout = gridLayout->itemAtPosition(0, 1)->layout();
    childLayout->addWidget(toolBar);

    toolBar->setIconSize(QSize(22, 22));

    jumpToHomeDirAction = new QAction("Jump To Home Directory", this);
    jumpToHomeDirAction->setIcon(QIcon(":/resource/home.png"));
    connect(jumpToHomeDirAction, &QAction::triggered, [this]() {
        setDirectory(QDir::homePath());
    });
    toolBar->addAction(jumpToHomeDirAction);

    jumpToCurWorkDirAction = new QAction("Jump To Current Working Directory", this);
    jumpToCurWorkDirAction->setIcon(QIcon(":/resource/computer.png"));
    connect(jumpToCurWorkDirAction, &QAction::triggered, [this]() {
        setDirectory(curWorkDir);
    });
    toolBar->addAction(jumpToCurWorkDirAction);

    jumpToRecPrjDirAction = new QAction("Jump To Recent Project Directory", this);
    jumpToRecPrjDirAction->setIcon(QIcon(":/resource/icon.png"));
    connect(jumpToRecPrjDirAction, &QAction::triggered, [this]() {
        // setRecPrjDir(recPrjDir);
        setDirectory(recPrjDir);
    });
    toolBar->addAction(jumpToRecPrjDirAction);

    try {
        std::vector<XmlRecent> recentList = XmlUtilities::instance().getRecentListFromFatherElementName(
            InitialConfig::instance().xmlPath.toStdString().c_str(),
            "RECENT_PROJECTS"
            );
        if (!recentList.empty()) {
            QFileInfo fileInfo(QString::fromStdString(recentList.at(0).getPath()));
            setRecPrjDir(fileInfo.path());
        }
    } catch (const std::exception& e) {
        // 异常
        qDebug() << "[MainWindow] An error occurred from MainWindow recentList: " << e.what();
    }
}

void AdvancedFileDialog::setCurWorkDir(const QString &dir)
{
    curWorkDir = dir;
}

QString AdvancedFileDialog::getOpenFileName(QWidget *parent,
                                            const QString &caption,
                                            const QString &dir,
                                            const QString &filter,
                                            QString *selectedFilter,
                                            Options options)
{
    AdvancedFileDialog dialog(parent);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setWindowTitle(caption);
    dialog.setDirectory(dir);
    dialog.setNameFilter(filter);
    if (selectedFilter != nullptr) {
        dialog.selectNameFilter(*selectedFilter);  // 设置初始选择的文件类型过滤器
    }
    dialog.setOptions(options);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.selectedFiles().at(0);
    }
    return QString();
}

QStringList AdvancedFileDialog::getOpenFileNames(QWidget *parent,
                                                 const QString &caption,
                                                 const QString &dir,
                                                 const QString &filter,
                                                 QString *selectedFilter,
                                                 Options options)
{
    AdvancedFileDialog dialog(parent);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setWindowTitle(caption);
    dialog.setDirectory(dir);
    dialog.setNameFilter(filter);
    dialog.setOptions(options);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.selectedFiles();
    }
    return QStringList();
}

QString AdvancedFileDialog::getSaveFileName(QWidget *parent,
                                            const QString &caption,
                                            const QString &dir,
                                            const QString &filter,
                                            QString *selectedFilter,
                                            Options options)
{
    AdvancedFileDialog dialog(parent);
    dialog.setWindowTitle(caption);
    dialog.setDirectory(dir);
    dialog.setNameFilter(filter);
    dialog.setOptions(options);

    if (selectedFilter != nullptr) {
        dialog.selectNameFilter(*selectedFilter);
    }

    if (dialog.exec() == QDialog::Accepted) {
        if (selectedFilter != nullptr) {
            *selectedFilter = dialog.selectedNameFilter();
        }
        return dialog.selectedFiles().at(0);
    }

    return QString();
}

QString AdvancedFileDialog::getExistingDirectory(QWidget *parent,
                                                 const QString &caption,
                                                 const QString &dir,
                                                 Options options)
{
    AdvancedFileDialog dialog(parent);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setWindowTitle(caption);
    dialog.setDirectory(dir);
    dialog.setOptions(options);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.selectedFiles().at(0);
    }
    return QString();
}

void AdvancedFileDialog::updateMetadata(const QString &path) {
    QFileInfo fileInfo(path);
    if (fileInfo.exists() && fileInfo.isFile()) {
        QString suffix = fileInfo.suffix().toLower();
        if (suffixToDescription.contains(fileInfo.suffix())) {
            suffix = suffixToDescription[suffix];
        }
        QString metadata = QString("<b>File:</b> %1<br>"
                                   "<b>Directory:</b> %2<br>"
                                   "<b>Created:</b> %3<br>"
                                   "<b>Accessed:</b> %4<br>"
                                   "<b>Modified:</b> %5<br>"
                                   "<b>Size:</b> %6 bytes<br>"
                                   "<b>Type:</b> %7<br>")
                               .arg(fileInfo.fileName(),
                                    fileInfo.path(),
                                    formatDateTime(fileInfo.birthTime()),
                                    formatDateTime(fileInfo.lastRead()),
                                    formatDateTime(fileInfo.lastModified()),
                                    QString::number(fileInfo.size()),
                                    suffix);

        metadataEdit->setHtml(metadata);
    } else {
        metadataEdit->setHtml(promptText);
    }
}

void AdvancedFileDialog::setRecPrjDir(const QString &dir)
{
    recPrjDir = dir;
    qDebug() << recPrjDir;
}

QString AdvancedFileDialog::formatDateTime(const QDateTime &dateTime) {
    QLocale englishLocale(QLocale::English, QLocale::UnitedStates);
    return englishLocale.toString(dateTime, "dddd yyyy/MM/dd HH:mm AP");
}
