// The definition of the class that implements accessibility support.
 


#ifndef _SCIACCESSIBILITY_H
#define _SCIACCESSIBILITY_H

#include <qglobal.h>

#if !defined(QT_NO_ACCESSIBILITY)

#include <QAccessible>
#include <QAccessibleEditableTextInterface>
#include <QAccessibleTextInterface>
#include <QAccessibleWidget>
#include <QByteArray>
#include <QFont>
#include <QList>
#include <QString>


class XsciScintillaBase;


// The implementation of accessibility support.
class XsciAccessibleScintillaBase : public QAccessibleWidget,
        public QAccessibleTextInterface,
        public QAccessibleEditableTextInterface
{
public:
    explicit XsciAccessibleScintillaBase(QWidget *widget);
    ~XsciAccessibleScintillaBase();

    static void initialise();

    static void selectionChanged(XsciScintillaBase *sb, bool selection);
    static void textInserted(XsciScintillaBase *sb, int position,
            const char *text, int length);
    static void textDeleted(XsciScintillaBase *sb, int position,
            const char *text, int length);
    static void updated(XsciScintillaBase *sb);

    void selection(int selectionIndex, int *startOffset, int *endOffset) const;
    int selectionCount() const;
    void addSelection(int startOffset, int endOffset);
    void removeSelection(int selectionIndex);
    void setSelection(int selectionIndex, int startOffset, int endOffset);

    int cursorPosition() const;
    void setCursorPosition(int position);

    QString text(int startOffset, int endOffset) const;
    QString textBeforeOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    QString textAfterOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    QString textAtOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    int characterCount() const;
    QRect characterRect(int offset) const;
    int offsetAtPoint(const QPoint &point) const;
    void scrollToSubstring(int startIndex, int endIndex);
    QString attributes(int offset, int *startOffset, int *endOffset) const;

    void deleteText(int startOffset, int endOffset);
    void insertText(int offset, const QString &text);
    void replaceText(int startOffset, int endOffset, const QString &text);

    QAccessible::State state() const;
    void *interface_cast(QAccessible::InterfaceType t);

private:
    static bool needs_initialising;
    static QList<XsciAccessibleScintillaBase *> all_accessibles;
    int current_cursor_offset;
    bool is_selection;

    static XsciAccessibleScintillaBase *findAccessible(XsciScintillaBase *sb);
    XsciScintillaBase *sciWidget() const;
    int validPosition(int offset) const;
    static bool boundaries(XsciScintillaBase *sb, int position,
            QAccessible::TextBoundaryType boundaryType, int *start_position,
            int *end_position);
    static QString textRange(XsciScintillaBase *sb, int start_position,
            int end_position);
    static QString bytesAsText(XsciScintillaBase *sb, const char *bytes,
            int length);
    static QByteArray textAsBytes(XsciScintillaBase *sb, const QString &text);
    static int positionAsOffset(XsciScintillaBase *sb, int position);
    static void positionRangeAsOffsetRange(XsciScintillaBase *sb,
            int start_position, int end_position, int *startOffset,
            int *endOffset);
    static int offsetAsPosition(XsciScintillaBase *sb, int offset);
    static QString colourAsRGB(int colour);
    static void addAttribute(QString &attrs, const char *name,
            const QString &value);
    QFont fontForStyle(int style) const;
};


#endif

#endif
