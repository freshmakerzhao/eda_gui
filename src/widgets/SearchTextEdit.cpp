#include "SearchTextEdit.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>

SearchTextEdit::SearchTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setObjectName("SearchTextEdit");
    _lastMatchCursor = textCursor();
}

void SearchTextEdit::searchForward(const QString &text)
{
    _searchText = text;
    QTextCursor cursor = textCursor();
    cursor.setPosition(_lastMatchCursor.position());

    QTextCursor matchCursor = document()->find(_searchText, cursor);
    if (matchCursor.isNull()) {
        // Wrap around and search from the beginning
        matchCursor = document()->find(_searchText);
    }

    if (!matchCursor.isNull()) {
        setTextCursor(matchCursor);
        _lastMatchCursor = matchCursor;
        highlightMatch(matchCursor);
    }
}

void SearchTextEdit::searchBackward(const QString &text)
{
    _searchText = text;
    QTextCursor cursor = textCursor();
    cursor.setPosition(_lastMatchCursor.position(), QTextCursor::KeepAnchor);

    QTextCursor matchCursor = document()->find(_searchText, cursor, QTextDocument::FindBackward);
    if (matchCursor.isNull()) {
        // Wrap around and search from the end
        matchCursor = document()->find(_searchText, QTextDocument::FindBackward);
    }

    if (!matchCursor.isNull()) {
        setTextCursor(matchCursor);
        _lastMatchCursor = matchCursor;
        highlightMatch(matchCursor);
    }
}

void SearchTextEdit::highlightMatch(const QTextCursor &cursor)
{
    QTextCharFormat format;
    format.setBackground(Qt::yellow);

    QTextCursor highlightCursor(document());
    highlightCursor.setPosition(cursor.selectionStart());
    highlightCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, cursor.selectionEnd() - cursor.selectionStart());
    highlightCursor.setCharFormat(format);
}

void SearchTextEdit::clearHighlight()
{
    QTextCharFormat format;
    format.setBackground(Qt::white);

    QTextCursor clearCursor(document());
    clearCursor.select(QTextCursor::Document);
    clearCursor.setCharFormat(format);
}
