/**
  ******************************************************************************
  * @file           : Editor.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/7
  ******************************************************************************
  */

#include "Editor.h"
#include "mainwindow.h"
#include "dialog/CustomMessageBox.h"

Editor::Editor(QWidget *parent)
    : QsciScintilla(parent)
{
    qDebug() << "[Editor] Constructing...";

    // updateActionState
    connect(this, &QsciScintilla::textChanged, MainWindow::instance(), &MainWindow::updateActionState);
    // 光标宽度
    setCaretWidth(10);
    // 加载字体文件
    QFontDatabase::addApplicationFont(":/resource/JetBrainsMonoNL-Bold.ttf");
    // 创建字体
    QFont font("JetBrains Mono NL", 9);
    // 设置行号字体
    setMarginsFont(font);
    // 设置显示行号
    setMarginLineNumbers(0, true);
    // 设置行号的宽度
    setMarginWidth(0, 50);
    // 设置折叠选项
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setMarginWidth(2, 20);
    // 创建词法分析器
    verilogLexer = new QsciLexerVerilog(this);
    verilogLexer->setFont(font);
    tclLexer = new QsciLexerTCL(this);
    tclLexer->setFont(font);
    // tclLexer->setColor(QColor(128, 0, 0), QsciLexerTCL::Identifier);
    //设置自动完成所有项
    setAutoCompletionSource(QsciScintilla::AcsAll);
    //设置大小写敏感
    setAutoCompletionCaseSensitivity(true);
    //每输入2个字符就出现自动完成的提示
    setAutoCompletionThreshold(2);
    // 括号匹配
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    // 设置词法分析器
    setLexer(verilogLexer);
    // EnCoding UTF-8
    SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    // 缩进宽度
    setTabWidth(4);
    // 缩进级别可见
    setIndentationGuides(true);
    // 自动缩进
    setAutoIndent(true);
}

Editor::~Editor()
{
    qDebug() << "[Editor] Distructing...";
    delete apis;
}

bool Editor::openFile(const QString path)
{
    _path = path;
    QFile file(path);
    QFileInfo fileInfo(file);
    if (!fileInfo.isFile()) {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file:\n" + file.errorString());
        return false;
    }
    if (fileInfo.suffix() == "v") {
        apis = new QsciAPIs(verilogLexer);
        QStringList keywords;
        QFile file(":/resource/keywords.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString keyword = in.readLine();
                keywords.append(keyword);
            }
            file.close();
            qDebug() << "keywords loaded successfully";
        }

        // 将关键词添加到自动完成列表
        foreach(const QString &keyword, keywords) {
            apis->add(keyword);
        }
        apis->prepare();
        setLexer(verilogLexer); // 设置词法分析器
    } else if (fileInfo.suffix() == "xdc") {
        apis = new QsciAPIs(tclLexer);
        apis->prepare();
        setLexer(tclLexer);
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");   // Decoding files using UTF-8
    this->setText(in.readAll());
    file.close();
    this->setModified(false);
    return true;
}

bool Editor::saveFile()
{
    QString path = _path;
    if (!path.isEmpty()) {
        QFile file(path);
        QFileInfo fileInfo(file);
        if (!fileInfo.isWritable()) {
            // 提示用户文件只读
            CustomMessageBox::showWarning(MainWindow::instance(), "Warning",
                                 "The file is read-only. Writing operation failed.");
            return false;
        }
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << this->text();
            file.close();
            this->setModified(false);
            return true;
        }
    }

    return false;
}

bool Editor::saveAsFile()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save As");

    QFileInfo fileInfo(_path);
    // 获取文件的扩展名
    QString extension = fileInfo.suffix();

    if (extension == "v") {
        dialog.selectFile("untitled.v");
        dialog.setNameFilter("Verilog Source Files (*.v)");
    } else if (extension == "xdc") {
        dialog.selectFile("untitled.xdc");
        dialog.setNameFilter("Xilinx Design Constraints (*.xdc)");
    }

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return false; // 用户取消了另存为操作
    }

    QString path = dialog.selectedFiles().value(0, "");
    QFile file(path);
    // 处理另存为文件异常
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot write file:\n" + file.errorString());
        return false;
    }
    _path = path;
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << this->text();
    file.close();
    this->setModified(false);
    return true;
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
        MainWindow::instance()->updateActionState();
    });
    menu->addAction(readOnlyAction);
    menu->exec(event->globalPos());
    delete menu;
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    static const std::map<int, QString> autoCompletionMap = {
        {Qt::Key_BracketLeft, "]"},
        {Qt::Key_ParenLeft, ")"},
        {Qt::Key_BraceLeft, "}"}
    };

    auto it = autoCompletionMap.find(event->key());
    if (it != autoCompletionMap.end()) {
        QsciScintilla::keyPressEvent(event);
        int pos = SendScintilla(QsciScintilla::SCI_GETCURRENTPOS);
        insert(it->second);
        SendScintilla(QsciScintilla::SCI_SETCURRENTPOS, pos);
        return;
    }

    QsciScintilla::keyPressEvent(event);
}

