#include "TextEditor.h"
#include "EditorManager.h"
#include "mainwindow.h"
#include "dialog/CustomMessageBox.h"
#include <QFontDatabase>

TextEditor::TextEditor(QWidget *parent)
    : ExsciScintilla(parent)
{
    qDebug() << "[Editor] Constructing...";

    // setStyleSheet("QScrollBar:vertical {"
    //                 "width: 16px;"
    //               "}"
    //               "QScrollBar:horizontal {"
    //                 "height: 16px;"
    //               "}"
    // );

    // updateActionState
    connect(this, &ExsciScintilla::textChanged, MainWindow::instance(), &MainWindow::updateActionState);
    // updateLineWidth
    connect(this, &ExsciScintilla::linesChanged, this, &TextEditor::resizeLineWidth);
    // 光标宽度
    setCaretWidth(10);
    // 创建字体
    QString fontName = "LFT Etica Mono";
    QFontDatabase database;
    QStringList fontFamilies = database.families();
    if (!fontFamilies.contains(fontName)) {
        fontName = "Consolas";
    }
    QFont font(fontName, 9);
    QFontMetrics _fontMetrics(font);
    _width = _fontMetrics.horizontalAdvance(QChar('0'));
    // 设置行号字体
    setMarginsFont(font);
    // 设置显示行号
    setMarginLineNumbers(0, true);
    // 设置折叠选项
    setFolding(ExsciScintilla::BoxedTreeFoldStyle);
    setMarginWidth(2, 20);
    // 创建词法分析器
    verilogLexer = new ExsciLexerVerilog(this);
    verilogLexer->setFont(font);
    verilogLexer->setFoldComments(true); // 开启注释可折叠
    verilogLexer->setFoldAtModule(true); // 开启模块(Module)可折叠
    tclLexer = new ExsciLexerTCL(this);
    tclLexer->setFont(font);
    // tclLexer->setColor(QColor(128, 0, 0), QsciLexerTCL::Identifier);
    //设置自动完成所有项
    setAutoCompletionSource(ExsciScintilla::AcsAll);
    //设置大小写敏感
    setAutoCompletionCaseSensitivity(true);
    //每输入2个字符就出现自动完成的提示
    setAutoCompletionThreshold(2);
    // 括号匹配
    setBraceMatching(ExsciScintilla::SloppyBraceMatch);
    // EnCoding UTF-8
    SendScintilla(ExsciScintilla::SCI_SETCODEPAGE, ExsciScintilla::SC_CP_UTF8);
    // 缩进宽度
    setTabWidth(4);
    // 缩进级别可见
    setIndentationGuides(true);
    // 自动缩进
    setAutoIndent(true);

    // 行号字体颜色
    setMarginsForegroundColor(QColor(85, 156, 179));
    // 行号背景颜色
    setMarginsBackgroundColor(QColor(240, 240, 240));

    configCodec();
    // setEolMode(ExsciScintilla::EolWindows);
    // setEolVisibility(true);

}

TextEditor::~TextEditor()
{
    qDebug() << "[Editor] Distructing...";
    if (apis)  {
        apis->deleteLater();
    }
}

bool TextEditor::openFile(const QString &path)
{
    _path = path;
    QFile file(path);
    QFileInfo fileInfo(file);
    if (!fileInfo.isFile()) {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (fileInfo.suffix() == "v") {
        apis = new ExsciAPIs(verilogLexer);
        QStringList keywords;
        QFile file(":/resource/keywords.txt");
        if (file.open(QIODevice::ReadOnly)) {
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
        apis = new ExsciAPIs(tclLexer);
        apis->prepare();
        setLexer(tclLexer);
    }
    QTextStream in(&file);
    in.setCodec(encoding.toStdString().c_str());
    this->setText(in.readAll());
    file.close();
    this->setModified(false);
    // 绑定Tab标签
    connect(this, &ExsciScintillaBase::SCN_SAVEPOINTLEFT, EditorManager::instance(), &EditorManager::setSavePointFlag);
    connect(this, &ExsciScintillaBase::SCN_SAVEPOINTREACHED, EditorManager::instance(), &EditorManager::resetSavePointFlag);
    return true;
}

bool TextEditor::saveFile()
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

        //! In `QIODevice::Text` mode, `QTextStream` automatically converts `\n`
        //! to the appropriate newline character for the operating system.
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out.setCodec(encoding.toStdString().c_str());
            out << this->text();
            file.close();
            this->setModified(false);
            return true;
        }
    }

    return false;
}

bool TextEditor::saveAsFile()
{
    AdvancedFileDialog dialog(this);
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
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot write file:\n" + file.errorString());
        return false;
    }
    _path = path;
    QTextStream out(&file);
    out.setCodec(encoding.toStdString().c_str());
    out << this->text();
    file.close();
    this->setModified(false);
    return true;
}

void TextEditor::contextMenuEvent(QContextMenuEvent *event)
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

void TextEditor::keyPressEvent(QKeyEvent *event)
{
    static const std::map<int, QString> autoCompletionMap = {
        {Qt::Key_BracketLeft, "]"},
        {Qt::Key_ParenLeft, ")"},
        {Qt::Key_BraceLeft, "}"}
    };

    auto it = autoCompletionMap.find(event->key());
    if (it != autoCompletionMap.end()) {
        ExsciScintilla::keyPressEvent(event);
        int pos = SendScintilla(ExsciScintilla::SCI_GETCURRENTPOS);
        insert(it->second);
        SendScintilla(ExsciScintilla::SCI_SETCURRENTPOS, pos);
        return;
    }

    ExsciScintilla::keyPressEvent(event);
}

void TextEditor::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y();
        // 如果滚轮向上滚动（delta > 0），增加缩放；向下滚动（delta < 0），减少缩放
        int currentZoom = SendScintilla(SCI_GETZOOM, 0);
        if (delta > 0) {
            SendScintilla(SCI_SETZOOM, currentZoom + 1);
        } else if (delta < 0) {
            SendScintilla(SCI_SETZOOM, currentZoom - 1);
        }
        return;
    }

    ExsciScintilla::wheelEvent(event);

}

void TextEditor::resizeLineWidth()
{
    // 设置行号的宽度
    setMarginWidth(0, QString::number(lines()).size() * _width + 16);
}

void TextEditor::configCodec()
{
    QSettings settings("HybrdChip", "HybrdLink");
    encoding = settings.value("TextEditor/encoding", "UTF-8").toString();
}
