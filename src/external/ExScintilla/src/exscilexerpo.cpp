// This module implements the ExsciLexerPO class.



#include "Exsci/exscilexerpo.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
ExsciLexerPO::ExsciLexerPO(QObject *parent)
    : ExsciLexer(parent), fold_comments(false), fold_compact(true)
{
}


// The dtor.
ExsciLexerPO::~ExsciLexerPO()
{
}


// Returns the language name.
const char *ExsciLexerPO::language() const
{
    return "PO";
}


// Returns the lexer name.
const char *ExsciLexerPO::lexer() const
{
    return "po";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerPO::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    }

    return ExsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerPO::defaultFont(int style) const
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
        f = ExsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the user name of a style.
QString ExsciLexerPO::description(int style) const
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
void ExsciLexerPO::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool ExsciLexerPO::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool ExsciLexerPO::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if comments can be folded.
bool ExsciLexerPO::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void ExsciLexerPO::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void ExsciLexerPO::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool ExsciLexerPO::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void ExsciLexerPO::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void ExsciLexerPO::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
