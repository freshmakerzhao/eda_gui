// This module implements the ExsciLexerCoffeeScript class.



#include "Exsci/exscilexercoffeescript.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
ExsciLexerCoffeeScript::ExsciLexerCoffeeScript(QObject *parent)
    : ExsciLexer(parent),
      fold_comments(false), fold_compact(true), style_preproc(false),
      dollars(true)
{
}


// The dtor.
ExsciLexerCoffeeScript::~ExsciLexerCoffeeScript()
{
}


// Returns the language name.
const char *ExsciLexerCoffeeScript::language() const
{
    return "CoffeeScript";
}


// Returns the lexer name.
const char *ExsciLexerCoffeeScript::lexer() const
{
    return "coffeescript";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList ExsciLexerCoffeeScript::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << ".";

    return wl;
}


// Return the list of keywords that can start a block.
const char *ExsciLexerCoffeeScript::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "catch class do else finally for if try until when while";
}


// Return the list of characters that can start a block.
const char *ExsciLexerCoffeeScript::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *ExsciLexerCoffeeScript::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the style used for braces.
int ExsciLexerCoffeeScript::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *ExsciLexerCoffeeScript::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerCoffeeScript::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80, 0x80, 0x80);

    case Comment:
    case CommentLine:
    case CommentBlock:
    case BlockRegexComment:
        return QColor(0x00, 0x7f, 0x00);

    case CommentDoc:
    case CommentLineDoc:
        return QColor(0x3f, 0x70, 0x3f);

    case Number:
        return QColor(0x00, 0x7f, 0x7f);

    case Keyword:
        return QColor(0x00, 0x00, 0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f, 0x00, 0x7f);

    case PreProcessor:
        return QColor(0x7f, 0x7f, 0x00);

    case Operator:
    case UnclosedString:
        return QColor(0x00, 0x00, 0x00);

    case VerbatimString:
        return QColor(0x00, 0x7f, 0x00);

    case Regex:
    case BlockRegex:
        return QColor(0x3f, 0x7f, 0x3f);

    case CommentDocKeyword:
        return QColor(0x30, 0x60, 0xa0);

    case CommentDocKeywordError:
        return QColor(0x80, 0x40, 0x20);

    case InstanceProperty:
        return QColor(0xc0, 0x60, 0x00);
    }

    return ExsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool ExsciLexerCoffeeScript::defaultEolFill(int style) const
{
    switch (style)
    {
    case UnclosedString:
    case VerbatimString:
    case Regex:
        return true;
    }

    return ExsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerCoffeeScript::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case CommentDoc:
    case CommentLineDoc:
    case CommentDocKeyword:
    case CommentDocKeywordError:
    case CommentBlock:
    case BlockRegexComment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = ExsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DoubleQuotedString:
    case SingleQuotedString:
    case UnclosedString:
    case VerbatimString:
    case Regex:
    case BlockRegex:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    default:
        f = ExsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *ExsciLexerCoffeeScript::keywords(int set) const
{
    if (set == 1)
        return
            "true false null this new delete typeof in instanceof return "
            "throw break continue debugger if else switch for while do try "
            "catch finally class extends super "
            "undefined then unless until loop of by when and or is isnt not "
            "yes no on off";

    return 0;
}


// Returns the user name of a style.
QString ExsciLexerCoffeeScript::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("C-style comment");

    case CommentLine:
        return tr("C++-style comment");

    case CommentDoc:
        return tr("JavaDoc C-style comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UUID:
        return tr("IDL UUID");

    case PreProcessor:
        return tr("Pre-processor block");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case VerbatimString:
        return tr("C# verbatim string");

    case Regex:
        return tr("Regular expression");

    case CommentLineDoc:
        return tr("JavaDoc C++-style comment");

    case KeywordSet2:
        return tr("Secondary keywords and identifiers");

    case CommentDocKeyword:
        return tr("JavaDoc keyword");

    case CommentDocKeywordError:
        return tr("JavaDoc keyword error");

    case GlobalClass:
        return tr("Global classes");

    case CommentBlock:
        return tr("Block comment");

    case BlockRegex:
        return tr("Block regular expression");

    case BlockRegexComment:
        return tr("Block regular expression comment");

    case InstanceProperty:
        return tr("Instance property");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor ExsciLexerCoffeeScript::defaultPaper(int style) const
{
    switch (style)
    {
    case UnclosedString:
        return QColor(0xe0,0xc0,0xe0);

    case VerbatimString:
        return QColor(0xe0,0xff,0xe0);

    case Regex:
        return QColor(0xe0,0xf0,0xe0);
    }

    return ExsciLexer::defaultPaper(style);
}


// Refresh all properties.
void ExsciLexerCoffeeScript::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setStylePreprocProp();
    setDollarsProp();
}


// Read properties from the settings.
bool ExsciLexerCoffeeScript::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    style_preproc = qs.value(prefix + "stylepreprocessor", false).toBool();
    dollars = qs.value(prefix + "dollars", true).toBool();

    return rc;
}


// Write properties to the settings.
bool ExsciLexerCoffeeScript::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "stylepreprocessor", style_preproc);
    qs.setValue(prefix + "dollars", dollars);

    return rc;
}


// Set if comments can be folded.
void ExsciLexerCoffeeScript::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void ExsciLexerCoffeeScript::setCommentProp()
{
    emit propertyChanged("fold.coffeescript.comment",
            (fold_comments ? "1" : "0"));
}


// Set if folds are compact
void ExsciLexerCoffeeScript::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void ExsciLexerCoffeeScript::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Set if preprocessor lines are styled.
void ExsciLexerCoffeeScript::setStylePreprocessor(bool style)
{
    style_preproc = style;

    setStylePreprocProp();
}


// Set the "styling.within.preprocessor" property.
void ExsciLexerCoffeeScript::setStylePreprocProp()
{
    emit propertyChanged("styling.within.preprocessor",
            (style_preproc ? "1" : "0"));
}


// Set if '$' characters are allowed.
void ExsciLexerCoffeeScript::setDollarsAllowed(bool allowed)
{
    dollars = allowed;

    setDollarsProp();
}


// Set the "lexer.cpp.allow.dollars" property.
void ExsciLexerCoffeeScript::setDollarsProp()
{
    emit propertyChanged("lexer.cpp.allow.dollars", (dollars ? "1" : "0"));
}
