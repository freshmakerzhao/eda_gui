#include "editor.h"


Editor::Editor(QWidget *parent)
    : QsciScintilla(parent)
{
    // textChanged
    connect(this, &QsciScintilla::textChanged, this, &Editor::refreshSaveState);

    this->setCaretWidth(10); // 光标宽度

    // 加载字体文件
    QFontDatabase::addApplicationFont(":/resource/JetBrainsMonoNL-Bold.ttf");
    // 创建字体
    QFont font("JetBrains Mono NL", 11);
    // 设置行号字体
    this->setMarginsFont(font);
    // 设置显示行号
    this->setMarginLineNumbers(0, true);
    // 设置行号的宽度
    this->setMarginWidth(0, 50);
    // 设置折叠选项
    this->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    this->setMarginWidth(2, 20);
    // 设置折叠提示按钮
    this->setMarginMarkerMask(1, QsciScintilla::SC_MASK_FOLDERS);
    this->setMarginType(1, QsciScintilla::SymbolMargin);
    this->setMarginSensitivity(1, true);
    this->setMarginWidth(1, 20);
    this->setMarginMarkerMask(1, 0x00200000);
    // 创建词法分析器
    textLexer = new QsciLexerVerilog;
    textLexer->setFont(font);
    apis = new QsciAPIs(textLexer);

    QStringList keywords;
    QFile file(":/resource/keywords.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString keyword = in.readLine();
            keywords.append(keyword);
        }
        file.close();
        qDebug() << "keywords load success";
    }

    // 将关键词添加到自动完成列表
    foreach(const QString &keyword, keywords) {
        apis->add(keyword);
    }

    apis->prepare();
    //设置自动完成所有项
    this->setAutoCompletionSource(QsciScintilla::AcsAll);
    //设置大小写敏感
    this->setAutoCompletionCaseSensitivity(true);
    //每输入3个字符就出现自动完成的提示
    this->setAutoCompletionThreshold(3);
    // 括号匹配
    this->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    // 设置词法分析器
    this->setLexer(textLexer);

}

Editor::~Editor()
{

}

// void Editor::openFile(QString path)
// {
//     m_path = path;
//     QFile file(path);
//     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QTextStream in(&file);
//         this->setText(in.readAll());
//         file.close();
//     }

//     initSaveState();
// }

bool Editor::saveFile()
{
    QString path = m_path;
    if (!path.isEmpty()) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << this->text();
            file.close();
            return true;
        }
    }

    return false;
}

void Editor::setFilePath(QString path)
{
    m_path = path;
}

bool Editor::checkSaved()
{
    return isSaved;
}

void Editor::initSaveState()
{
    isSaved = true;
}

void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    QAction *readOnlyAction = new QAction("Read-Only", this);
    readOnlyAction->setCheckable(true);
    readOnlyAction->setChecked(isReadOnly()); // 设置初始只读状态
    connect(readOnlyAction, &QAction::triggered, this, [this, readOnlyAction](){
        setReadOnly(!isReadOnly());
        readOnlyAction->setChecked(isReadOnly()); // 设置按钮的选中状态
    });
    menu->addAction(readOnlyAction);
    menu->exec(event->globalPos());
    delete menu;
}

void Editor::refreshSaveState()
{
    isSaved = false;
}
