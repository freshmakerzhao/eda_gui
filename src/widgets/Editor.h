#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QDebug>
#include <QTextStream>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QKeyEvent>

#include "Xsci/xsciscintilla.h" // QsciScintilla本体
#include "Xsci/xsciapis.h"      // 自动补全的apis
#include <Xsci/xscilexerverilog.h>   // Verilog词法分析器
// #include <Xsci/xscilexervhdl.h>      // VHDL词法分析器(未启用)
#include <Xsci/xscilexertcl.h>

#include "dialog/AdvancedFileDialog.h"

class Editor : public XsciScintilla
{
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);
    ~Editor();

public:
    bool openFile(const QString &path);
    bool saveFile();
    bool saveAsFile();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void resizeLineWidth();

private:
    int _width = 16; // 字符宽度
    XsciLexerVerilog *verilogLexer;
    XsciLexerTCL *tclLexer;
    XsciAPIs *apis = nullptr;
    QString _path;
};

#endif // EDITOR_H
