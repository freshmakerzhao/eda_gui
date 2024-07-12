#include "AdvancedFileDialog.h"

AdvancedFileDialog::AdvancedFileDialog(QWidget *parent) :
    QFileDialog(parent),
    metadataLabel(new QLabel("<b>Information</b>", this)),
    metadataEdit(new QTextEdit(promptText, this)),
    toolBar(new QToolBar(this))
{
    resize(1350, 600);
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

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    // Add the custom widget to the QFileDialog
    setOption(QFileDialog::DontUseNativeDialog, true);
    setViewMode(QFileDialog::List);
    QGridLayout *gridLayout = qobject_cast<QGridLayout *>(this->layout());
    // 调整布局
    QListView *listView = this->findChild<QListView *>();
    gridLayout->removeWidget(listView);
    QStackedWidget *stackedWidget = this->findChild<QStackedWidget *>("stackedWidget");
    gridLayout->removeWidget(stackedWidget);
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
    jumpToHomeDirAction->setIcon(QIcon(":/resources/home.png"));
    connect(jumpToHomeDirAction, &QAction::triggered, [this]() {
        setDirectory(QDir::homePath());
    });
    toolBar->addAction(jumpToHomeDirAction);

    jumpToCurWorkDirAction = new QAction("Jump To Current Working Directory", this);
    jumpToCurWorkDirAction->setIcon(QIcon(":/resources/computer.png"));
    connect(jumpToCurWorkDirAction, &QAction::triggered, [this]() {
        setDirectory(curWorkDir);
    });
    toolBar->addAction(jumpToCurWorkDirAction);

    jumpToRecPrjDirAction = new QAction("Jump To Recent Project Directory", this);
    jumpToRecPrjDirAction->setIcon(QIcon(":/resource/icon.png"));
    toolBar->addAction(jumpToRecPrjDirAction);
}

void AdvancedFileDialog::setCurWorkDir(const QString &dir)
{
    curWorkDir = dir;
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

QString AdvancedFileDialog::formatDateTime(const QDateTime &dateTime) {
    QLocale englishLocale(QLocale::English, QLocale::UnitedStates);
    return englishLocale.toString(dateTime, "dddd yyyy/MM/dd HH:mm AP");
}
