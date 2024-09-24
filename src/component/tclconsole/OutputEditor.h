#ifndef OUTPUTEDITOR_H
#define OUTPUTEDITOR_H

#include <QTextEdit>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>

class OutputEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit OutputEditor(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // OUTPUTEDITOR_H
