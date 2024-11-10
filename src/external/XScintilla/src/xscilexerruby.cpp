// This module implements the XsciLexerRuby class.
 


#include "Xsci/xscilexerruby.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerRuby::XsciLexerRuby(QObject *parent)
    : XsciLexer(parent), fold_comments(false), fold_compact(true)
{
}


// The dtor.
XsciLexerRuby::~XsciLexerRuby()
{
}


// Returns the language name.
const char *XsciLexerRuby::language() const
{
    return "Ruby";
}


// Returns the lexer name.
const char *XsciLexerRuby::lexer() const
{
    return "ruby";
}


// Return the list of words that can start a block.
const char *XsciLexerRuby::blockStart(int *style) const
{
    if (style)
        *style = Keyword;

    return "do";
}


// Return the list of words that can start end a block.
const char *XsciLexerRuby::blockEnd(int *style) const
{
    if (style)
        *style = Keyword;

    return "end";
}


// Return the list of words that can start end a block.
const char *XsciLexerRuby::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "def class if do elsif else case while for";
}


// Return the style used for braces.
int XsciLexerRuby::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerRuby::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case POD:
        return QColor(0x00,0x40,0x00);

    case Number:
    case FunctionMethodName:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
    case DemotedKeyword:
        return QColor(0x00,0x00,0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
    case HereDocument:
    case PercentStringq:
    case PercentStringQ:
        return QColor(0x7f,0x00,0x7f);

    case ClassName:
        return QColor(0x00,0x00,0xff);

    case Regex:
    case HereDocumentDelimiter:
    case PercentStringr:
    case PercentStringw:
        return QColor(0x00,0x00,0x00);

    case Global:
        return QColor(0x80,0x00,0x80);

    case Symbol:
        return QColor(0xc0,0xa0,0x30);

    case ModuleName:
        return QColor(0xa0,0x00,0xa0);

    case InstanceVariable:
        return QColor(0xb0,0x00,0x80);

    case ClassVariable:
        return QColor(0x80,0x00,0xb0);

    case Backticks:
    case PercentStringx:
        return QColor(0xff,0xff,0x00);

    case DataSection:
        return QColor(0x60,0x00,0x00);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerRuby::defaultEolFill(int style) const
{
    bool fill;

    switch (style)
    {
    case POD:
    case DataSection:
    case HereDocument:
        fill = true;
        break;

    default:
        fill = XsciLexer::defaultEolFill(style);
    }

    return fill;
}


// Returns the font of the text for a style.
QFont XsciLexerRuby::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case POD:
    case DoubleQuotedString:
    case SingleQuotedString:
    case PercentStringq:
    case PercentStringQ:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    case Keyword:
    case ClassName:
    case FunctionMethodName:
    case Operator:
    case ModuleName:
    case DemotedKeyword:
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerRuby::keywords(int set) const
{
    if (set == 1)
        return
            "__FILE__ and def end in or self unless __LINE__ "
            "begin defined? ensure module redo super until BEGIN "
            "break do false next rescue then when END case else "
            "for nil require retry true while alias class elsif "
            "if not return undef yield";

    return 0;
}


// Returns the user name of a style.
QString XsciLexerRuby::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Error:
        return tr("Error");

    case Comment:
        return tr("Comment");

    case POD:
        return tr("POD");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case ClassName:
        return tr("Class name");

    case FunctionMethodName:
        return tr("Function or method name");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case Regex:
        return tr("Regular expression");

    case Global:
        return tr("Global");

    case Symbol:
        return tr("Symbol");

    case ModuleName:
        return tr("Module name");

    case InstanceVariable:
        return tr("Instance variable");

    case ClassVariable:
        return tr("Class variable");

    case Backticks:
        return tr("Backticks");

    case DataSection:
        return tr("Data section");

    case HereDocumentDelimiter:
        return tr("Here document delimiter");

    case HereDocument:
        return tr("Here document");

    case PercentStringq:
        return tr("%q string");

    case PercentStringQ:
        return tr("%Q string");

    case PercentStringx:
        return tr("%x string");

    case PercentStringr:
        return tr("%r string");

    case PercentStringw:
        return tr("%w string");

    case DemotedKeyword:
        return tr("Demoted keyword");

    case Stdin:
        return tr("stdin");

    case Stdout:
        return tr("stdout");

    case Stderr:
        return tr("stderr");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerRuby::defaultPaper(int style) const
{
    switch (style)
    {
    case Error:
        return QColor(0xff,0x00,0x00);

    case POD:
        return QColor(0xc0,0xff,0xc0);

    case Regex:
    case PercentStringr:
        return QColor(0xa0,0xff,0xa0);

    case Backticks:
    case PercentStringx:
        return QColor(0xa0,0x80,0x80);

    case DataSection:
        return QColor(0xff,0xf0,0xd8);

    case HereDocumentDelimiter:
    case HereDocument:
        return QColor(0xdd,0xd0,0xdd);

    case PercentStringw:
        return QColor(0xff,0xff,0xe0);

    case Stdin:
    case Stdout:
    case Stderr:
        return QColor(0xff,0x80,0x80);
    }

    return XsciLexer::defaultPaper(style);
}


// Refresh all properties.
void XsciLexerRuby::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool XsciLexerRuby::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerRuby::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.value(prefix + "foldcomments", fold_comments);
    qs.value(prefix + "foldcompact", fold_compact);

    return rc;
}


// Set if comments can be folded.
void XsciLexerRuby::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void XsciLexerRuby::setCommentProp()
{
    emit propertyChanged("fold.comment", (fold_comments ? "1" : "0"));
}


// Set if folds are compact
void XsciLexerRuby::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void XsciLexerRuby::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}
