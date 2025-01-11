// This module implements the ExsciLexerJavaScript class.



#include "Exsci/exscilexerjavascript.h"

#include <qcolor.h>
#include <qfont.h>


// The list of JavaScript keywords that can be used by other friendly lexers.
const char *ExsciLexerJavaScript::keywordClass =
    "abstract boolean break byte case catch char class const continue "
    "debugger default delete do double else enum export extends final "
    "finally float for function goto if implements import in instanceof "
    "int interface long native new package private protected public "
    "return short static super switch synchronized this throw throws "
    "transient try typeof var void volatile while with";


// The ctor.
ExsciLexerJavaScript::ExsciLexerJavaScript(QObject *parent)
    : ExsciLexerCPP(parent)
{
}


// The dtor.
ExsciLexerJavaScript::~ExsciLexerJavaScript()
{
}


// Returns the language name.
const char *ExsciLexerJavaScript::language() const
{
    return "JavaScript";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerJavaScript::defaultColor(int style) const
{
    if (style == Regex)
        return QColor(0x3f,0x7f,0x3f);

    return ExsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool ExsciLexerJavaScript::defaultEolFill(int style) const
{
    if (style == Regex)
        return true;

    return ExsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerJavaScript::defaultFont(int style) const
{
    if (style == Regex)
#if defined(Q_OS_WIN)
        return QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        return QFont("Courier", 12);
#else
        return QFont("Bitstream Vera Sans Mono",9);
#endif

    return ExsciLexerCPP::defaultFont(style);
}


// Returns the set of keywords.
const char *ExsciLexerJavaScript::keywords(int set) const
{
    if (set != 1)
        return 0;

    return keywordClass;
}


// Returns the user name of a style.
QString ExsciLexerJavaScript::description(int style) const
{
    if (style == Regex)
        return tr("Regular expression");

    return ExsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor ExsciLexerJavaScript::defaultPaper(int style) const
{
    if (style == Regex)
        return QColor(0xe0,0xf0,0xff);

    return ExsciLexer::defaultPaper(style);
}
