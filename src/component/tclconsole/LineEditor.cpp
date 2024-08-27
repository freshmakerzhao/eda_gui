#include "LineEditor.h"
#include <QKeyEvent>
#include <QToolTip>

LineEditor::LineEditor(/*ParseHelper *helper, */QWidget *parent) : QLineEdit(parent), index(0)/*, parseHelper(helper)*/
{
    setPlaceholderText("Type a Tcl command here");
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &LineEditor::returnPressed, this, &LineEditor::textInserted);
    connect(this, &LineEditor::customContextMenuRequested, this, &LineEditor::showContextMenu);
}

void LineEditor::keyPressEvent(QKeyEvent *ev)
{

    if (ev->key() == Qt::Key_Up || ev->key() == Qt::Key_Down) {
        QToolTip::hideText();
        if (lines.empty())
            return;
        if (ev->key() == Qt::Key_Up)
            index--;
        if (ev->key() == Qt::Key_Down)
            index++;

        if (index < 0)
            index = 0;
        if (index >= lines.size()) {
            index = lines.size();
            clear();
            return;
        }
        setText(lines[index]);
    } else if (ev->key() == Qt::Key_Escape) {
        QToolTip::hideText();
        clear();
        return;
    } else if (ev->key() == Qt::Key_Tab) {
        autoComplete();
        return;
    }
    QToolTip::hideText();

    QLineEdit::keyPressEvent(ev);
}

// This makes TAB work
bool LineEditor::focusNextPrevChild(bool next) { return false; }

void LineEditor::textInserted()
{
    if (lines.empty() || lines.back() != text())
        lines += text();
    if (lines.size() > 100)
        lines.removeFirst();
    index = lines.size();
    clear();
    Q_EMIT textLineInserted(lines.back());
}

void LineEditor::showContextMenu(const QPoint &pt)
{
    QMenu *contextMenu = createStandardContextMenu();
    QAction clearAction("Clear &history");
    clearAction.setStatusTip("Clears line edit history");
    connect(&clearAction, &QAction::triggered, this, &LineEditor::clearHistory);
    contextMenu->addSeparator();
    contextMenu->addAction(&clearAction);
    contextMenu->exec(mapToGlobal(pt));
    delete contextMenu;
}

void LineEditor::clearHistory()
{
    lines.clear();
    index = 0;
    clear();
}

void LineEditor::autoComplete()
{
    QString line = text();
    // const std::list<std::string> &suggestions = pyinterpreter_suggest(line.toStdString());
    // if (suggestions.size() == 1) {
    //     line = suggestions.back().c_str();
    // } else {
    //     // try to complete to longest common prefix
    //     std::string prefix = LongestCommonPrefix(suggestions.begin(), suggestions.end());
    //     if (prefix.size() > (size_t)line.size()) {
    //         line = prefix.c_str();
    //     } else {
    //         ColumnFormatter fmt;
    //         fmt.setItems(suggestions.begin(), suggestions.end());
    //         fmt.format(width() / 5);
    //         QString out = "";
    //         for (auto &it : fmt.formattedOutput()) {
    //             if (!out.isEmpty())
    //                 out += "\n";
    //             out += it.c_str();
    //         }
    //         QToolTip::setFont(font());
    //         if (!out.trimmed().isEmpty())
    //             QToolTip::showText(mapToGlobal(QPoint(0, 0)), out);
    //     }
    // }
    // set up the next line on the console
    setText(line);
}
