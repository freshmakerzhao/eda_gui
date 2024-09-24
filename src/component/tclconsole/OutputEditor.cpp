#include "OutputEditor.h"

OutputEditor::OutputEditor(QWidget *parent) : QTextEdit(parent) {
    setReadOnly(true);
}

void OutputEditor::contextMenuEvent(QContextMenuEvent *event) {
    QMenu *menu = createStandardContextMenu();
    QAction *clearAction = menu->addAction("Clear All");
    connect(clearAction, &QAction::triggered, this, &OutputEditor::clear);
    menu->exec(event->globalPos());
    delete menu;
}

