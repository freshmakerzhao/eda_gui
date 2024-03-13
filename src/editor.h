#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QFontDatabase>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>

#include "Qsci/qsciscintilla.h" // QsciScintilla本体
#include "Qsci/qsciapis.h"      // 自动补全的apis
#include <qscilexerverilog.h>   // Verilog词法分析器
// #include <qscilexervhdl.h>      // VHDL词法分析器(未启用)


class Editor : public QsciScintilla
{
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);
    ~Editor();

public:
    bool openFile(const QString path);
    bool saveFile();
    bool saveAsFile();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QsciLexerVerilog *textLexer;
    QsciAPIs *apis;
    QString m_path;
};

#endif // EDITOR_H
