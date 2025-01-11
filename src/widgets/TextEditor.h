#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QDebug>
#include <QTextStream>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QKeyEvent>
#include <QSettings>

#include "Exsci/exsciscintilla.h"
#include "Exsci/exsciapis.h"      // 自动补全的apis
#include <Exsci/exscilexerverilog.h>   // Verilog词法分析器
// #include <Exsci/exscilexervhdl.h>      // VHDL词法分析器(未启用)
#include <Exsci/exscilexertcl.h>

#include "dialog/AdvancedFileDialog.h"

class TextEditor : public ExsciScintilla
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

public:
    bool openFile(const QString &path);
    bool saveFile();
    bool saveAsFile();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

private slots:
    void resizeLineWidth();

private:
    void configCodec();

private:
    QString encoding;

    int _width = 16; // 字符宽度
    ExsciLexerVerilog *verilogLexer;
    ExsciLexerTCL *tclLexer;
    ExsciAPIs *apis = nullptr;
    QString _path;
};

#endif // EDITOR_H
