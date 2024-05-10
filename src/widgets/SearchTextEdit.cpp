#include "SearchTextEdit.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>

SearchTextEdit::SearchTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setStyleSheet("border:1px solid #DCDCDC");
    m_lastMatchCursor = textCursor();
}

void SearchTextEdit::searchForward(const QString &text)
{
    m_searchText = text;
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_lastMatchCursor.position());

    QTextCursor matchCursor = document()->find(m_searchText, cursor);
    if (matchCursor.isNull()) {
        // Wrap around and search from the beginning
        matchCursor = document()->find(m_searchText);
    }

    if (!matchCursor.isNull()) {
        setTextCursor(matchCursor);
        m_lastMatchCursor = matchCursor;
        highlightMatch(matchCursor);
    }
}

void SearchTextEdit::searchBackward(const QString &text)
{
    m_searchText = text;
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_lastMatchCursor.position(), QTextCursor::KeepAnchor);

    QTextCursor matchCursor = document()->find(m_searchText, cursor, QTextDocument::FindBackward);
    if (matchCursor.isNull()) {
        // Wrap around and search from the end
        matchCursor = document()->find(m_searchText, QTextDocument::FindBackward);
    }

    if (!matchCursor.isNull()) {
        setTextCursor(matchCursor);
        m_lastMatchCursor = matchCursor;
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

// void SearchTextEdit::keyPressEvent(QKeyEvent *event)
// {
//     if (event->matches(QKeySequence::FindNext)) {
//         searchForward(m_searchText);
//     } else if (event->matches(QKeySequence::FindPrevious)) {
//         searchBackward(m_searchText);
//     } else {
//         QPlainTextEdit::keyPressEvent(event);
//     }
// }
