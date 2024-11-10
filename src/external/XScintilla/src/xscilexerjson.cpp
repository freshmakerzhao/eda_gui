// This module implements the XsciLexerJSON class.
 


#include "Xsci/xscilexerjson.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerJSON::XsciLexerJSON(QObject *parent)
    : XsciLexer(parent),
      allow_comments(true), escape_sequence(true), fold_compact(true)
{
}


// The dtor.
XsciLexerJSON::~XsciLexerJSON()
{
}


// Returns the language name.
const char *XsciLexerJSON::language() const
{
    return "JSON";
}


// Returns the lexer name.
const char *XsciLexerJSON::lexer() const
{
    return "json";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerJSON::defaultColor(int style) const
{
    switch (style)
    {
    case UnclosedString:
    case Error:
        return QColor(0xff, 0xff, 0xff);

    case Number:
        return QColor(0x00, 0x7f, 0x7f);

    case String:
        return QColor(0x7f, 0x00, 0x00);

    case Property:
        return QColor(0x88, 0x0a, 0xe8);

    case EscapeSequence:
        return QColor(0x0b, 0x98, 0x2e);

    case CommentLine:
    case CommentBlock:
        return QColor(0x05, 0xbb, 0xae);

    case Operator:
        return QColor(0x18, 0x64, 0x4a);

    case IRI:
        return QColor(0x00, 0x00, 0xff);

    case IRICompact:
        return QColor(0xd1, 0x37, 0xc1);

    case Keyword:
        return QColor(0x0b, 0xce, 0xa7);

    case KeywordLD:
        return QColor(0xec, 0x28, 0x06);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerJSON::defaultEolFill(int style) const
{
    switch (style)
    {
    case UnclosedString:
        return true;
    }

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerJSON::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case CommentLine:
        f = XsciLexer::defaultFont(style);
        f.setItalic(true);
        break;

    case Keyword:
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerJSON::keywords(int set) const
{
    if (set == 1)
        return "false true null";

    if (set == 2)
        return
            "@id @context @type @value @language @container @list @set "
            "@reverse @index @base @vocab @graph";

    return 0;
}


// Returns the user name of a style.
QString XsciLexerJSON::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Number:
        return tr("Number");

    case String:
        return tr("String");

    case UnclosedString:
        return tr("Unclosed string");

    case Property:
        return tr("Property");

    case EscapeSequence:
        return tr("Escape sequence");

    case CommentLine:
        return tr("Line comment");

    case CommentBlock:
        return tr("Block comment");

    case Operator:
        return tr("Operator");

    case IRI:
        return tr("IRI");

    case IRICompact:
        return tr("JSON-LD compact IRI");

    case Keyword:
        return tr("JSON keyword");

    case KeywordLD:
        return tr("JSON-LD keyword");

    case Error:
        return tr("Parsing error");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerJSON::defaultPaper(int style) const
{
    switch (style)
    {
    case UnclosedString:
    case Error:
        return QColor(0xff, 0x00, 0x00);
    }

    return XsciLexer::defaultPaper(style);
}


// Refresh all properties.
void XsciLexerJSON::refreshProperties()
{   
    setAllowCommentsProp();
    setEscapeSequenceProp();
    setCompactProp();
}


// Read properties from the settings.
bool XsciLexerJSON::readProperties(QSettings &qs,const QString &prefix)
{
    allow_comments = qs.value(prefix + "allowcomments", true).toBool();
    escape_sequence = qs.value(prefix + "escapesequence", true).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return true;
}


// Write properties to the settings.
bool XsciLexerJSON::writeProperties(QSettings &qs,const QString &prefix) const
{
    qs.setValue(prefix + "allowcomments", allow_comments);
    qs.setValue(prefix + "escapesequence", escape_sequence);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return true;
}


// Set if comments are highlighted
void XsciLexerJSON::setHighlightComments(bool highlight)
{
    allow_comments = highlight;

    setAllowCommentsProp();
}


// Set the "lexer.json.allow.comments" property.
void XsciLexerJSON::setAllowCommentsProp()
{
    emit propertyChanged("lexer.json.allow.comments",
            (allow_comments ? "1" : "0"));
}


// Set if escape sequences are highlighted.
void XsciLexerJSON::setHighlightEscapeSequences(bool highlight)
{
    escape_sequence = highlight;

    setEscapeSequenceProp();
}


// Set the "lexer.json.escape.sequence" property.
void XsciLexerJSON::setEscapeSequenceProp()
{
    emit propertyChanged("lexer.json.escape.sequence",
            (escape_sequence ? "1" : "0"));
}


// Set if folds are compact.
void XsciLexerJSON::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void XsciLexerJSON::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}
