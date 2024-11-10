// This module implements the XsciLexerPascal class.
 


#include "Xsci/xscilexerpascal.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerPascal::XsciLexerPascal(QObject *parent)
    : XsciLexer(parent),
      fold_comments(false), fold_compact(true), fold_preproc(false),
      smart_highlight(true)
{
}


// The dtor.
XsciLexerPascal::~XsciLexerPascal()
{
}


// Returns the language name.
const char *XsciLexerPascal::language() const
{
    return "Pascal";
}


// Returns the lexer name.
const char *XsciLexerPascal::lexer() const
{
    return "pascal";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList XsciLexerPascal::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << "." << "^";

    return wl;
}


// Return the list of keywords that can start a block.
const char *XsciLexerPascal::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return
        "case class do else for then private protected public published "
        "repeat try while type";
}


// Return the list of characters that can start a block.
const char *XsciLexerPascal::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "begin";
}


// Return the list of characters that can end a block.
const char *XsciLexerPascal::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "end";
}


// Return the style used for braces.
int XsciLexerPascal::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerPascal::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Identifier:
        break;

    case Comment:
    case CommentParenthesis:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case PreProcessor:
    case PreProcessorParenthesis:
        return QColor(0x7f,0x7f,0x00);

    case Number:
    case HexNumber:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case SingleQuotedString:
    case Character:
        return QColor(0x7f,0x00,0x7f);

    case UnclosedString:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Asm:
        return QColor(0x80,0x40,0x80);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerPascal::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerPascal::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentParenthesis:
    case CommentLine:
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
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case SingleQuotedString:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        f.setItalic(true);
        break;

    case UnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono", 9);
#endif
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the background colour of the text for a style.
QColor XsciLexerPascal::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return XsciLexer::defaultPaper(style);
}


// Returns the set of keywords.
const char *XsciLexerPascal::keywords(int set) const
{
    if (set == 1)
        return
            "absolute abstract and array as asm assembler automated begin "
            "case cdecl class const constructor delayed deprecated destructor "
            "dispid dispinterface div do downto dynamic else end except "
            "experimental export exports external far file final finalization "
            "finally for forward function goto helper if implementation in "
            "inherited initialization inline interface is label library "
            "message mod near nil not object of on operator or out overload "
            "override packed pascal platform private procedure program "
            "property protected public published raise record reference "
            "register reintroduce repeat resourcestring safecall sealed set "
            "shl shr static stdcall strict string then threadvar to try type "
            "unit unsafe until uses var varargs virtual while winapi with xor"
            "add default implements index name nodefault read readonly remove "
            "stored write writeonly"
            "package contains requires";

    return 0;
}


// Returns the user name of a style.
QString XsciLexerPascal::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Identifier:
        return tr("Identifier");

    case Comment:
        return tr("'{ ... }' style comment");

    case CommentParenthesis:
        return tr("'(* ... *)' style comment");

    case CommentLine:
        return tr("Line comment");

    case PreProcessor:
        return tr("'{$ ... }' style pre-processor block");

    case PreProcessorParenthesis:
        return tr("'(*$ ... *)' style pre-processor block");

    case Number:
        return tr("Number");

    case HexNumber:
        return tr("Hexadecimal number");

    case Keyword:
        return tr("Keyword");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UnclosedString:
        return tr("Unclosed string");

    case Character:
        return tr("Character");

    case Operator:
        return tr("Operator");

    case Asm:
        return tr("Inline asm");
    }

    return QString();
}


// Refresh all properties.
void XsciLexerPascal::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setPreprocProp();
    setSmartHighlightProp();
}


// Read properties from the settings.
bool XsciLexerPascal::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_preproc = qs.value(prefix + "foldpreprocessor", true).toBool();
    smart_highlight = qs.value(prefix + "smarthighlight", true).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerPascal::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpreprocessor", fold_preproc);
    qs.setValue(prefix + "smarthighlight", smart_highlight);

    return rc;
}


// Return true if comments can be folded.
bool XsciLexerPascal::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void XsciLexerPascal::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void XsciLexerPascal::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool XsciLexerPascal::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void XsciLexerPascal::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void XsciLexerPascal::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool XsciLexerPascal::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void XsciLexerPascal::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void XsciLexerPascal::setPreprocProp()
{
    emit propertyChanged("fold.preprocessor",(fold_preproc ? "1" : "0"));
}


// Return true if smart highlighting is enabled.
bool XsciLexerPascal::smartHighlighting() const
{
    return smart_highlight;
}


// Set if smart highlighting is enabled.
void XsciLexerPascal::setSmartHighlighting(bool enabled)
{
    smart_highlight = enabled;

    setSmartHighlightProp();
}


// Set the "lexer.pascal.smart.highlighting" property.
void XsciLexerPascal::setSmartHighlightProp()
{
    emit propertyChanged("lexer.pascal.smart.highlighting", (smart_highlight ? "1" : "0"));
}
