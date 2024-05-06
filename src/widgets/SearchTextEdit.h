#ifndef SEARCHTEXTEDIT_H
#define SEARCHTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class SearchTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    SearchTextEdit(QWidget *parent = nullptr);

public slots:
    void searchForward(const QString &text);
    void searchBackward(const QString &text);
    void clearHighlight();
private:
    QString m_searchText;
    QTextCursor m_lastMatchCursor;

    void highlightMatch(const QTextCursor &cursor);

    // void keyPressEvent(QKeyEvent *event) override;
};

#endif // SEARCHTEXTEDIT_H
