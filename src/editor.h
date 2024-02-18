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
#include <qscilexervhdl.h>      // VHDL词法分析器(未启用)


class Editor : public QsciScintilla
{
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);
    ~Editor();

public:
    void openFile(QString path);
    bool saveFile();
    void setFilePath(QString path);
    bool checkSaved();
    void initSaveState(); // 初始化保存状态

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void refreshSaveState();

private:
    QsciLexer *textLexer;
    QsciAPIs *apis;

    QString m_path;
    bool isSaved = false;
};

#endif // EDITOR_H
