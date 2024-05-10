/**
  ******************************************************************************
  * @file           : Editor.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/2/7
  ******************************************************************************
  */

#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QKeyEvent>

#include "Qsci/qsciscintilla.h" // QsciScintilla本体
#include "Qsci/qsciapis.h"      // 自动补全的apis
#include <qscilexerverilog.h>   // Verilog词法分析器
// #include <qscilexervhdl.h>      // VHDL词法分析器(未启用)
#include <qscilexertcl.h>

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

    void keyPressEvent(QKeyEvent *event) override;

private:
    QsciLexerVerilog *verilogLexer;
    QsciLexerTCL *tclLexer;
    QsciAPIs *apis;
    QString _path;
};

#endif // EDITOR_H
