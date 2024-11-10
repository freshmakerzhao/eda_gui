// This module implements the XsciLexerPO class.
 


#include "Xsci/xscilexerpo.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerPO::XsciLexerPO(QObject *parent)
    : XsciLexer(parent), fold_comments(false), fold_compact(true)
{
}


// The dtor.
XsciLexerPO::~XsciLexerPO()
{
}


// Returns the language name.
const char *XsciLexerPO::language() const
{
    return "PO";
}


// Returns the lexer name.
const char *XsciLexerPO::lexer() const
{
    return "po";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerPO::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont XsciLexerPO::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS", 9);
#elif defined(Q_OS_MAC)
        f = QFont("Georgia", 13);
#else
        f = QFont("Bitstream Vera Serif", 9);
#endif
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the user name of a style.
QString XsciLexerPO::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case MessageId:
        return tr("Message identifier");

    case MessageIdText:
        return tr("Message identifier text");

    case MessageString:
        return tr("Message string");

    case MessageStringText:
        return tr("Message string text");

    case MessageContext:
        return tr("Message context");

    case MessageContextText:
        return tr("Message context text");

    case Fuzzy:
        return tr("Fuzzy flag");

    case ProgrammerComment:
        return tr("Programmer comment");

    case Reference:
        return tr("Reference");

    case Flags:
        return tr("Flags");

    case MessageIdTextEOL:
        return tr("Message identifier text end-of-line");

    case MessageStringTextEOL:
        return tr("Message string text end-of-line");

    case MessageContextTextEOL:
        return tr("Message context text end-of-line");
    }

    return QString();
}


// Refresh all properties.
void XsciLexerPO::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool XsciLexerPO::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerPO::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if comments can be folded.
bool XsciLexerPO::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void XsciLexerPO::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void XsciLexerPO::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool XsciLexerPO::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void XsciLexerPO::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void XsciLexerPO::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
