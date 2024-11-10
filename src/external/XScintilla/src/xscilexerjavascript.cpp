// This module implements the XsciLexerJavaScript class.
 


#include "Xsci/xscilexerjavascript.h"

#include <qcolor.h>
#include <qfont.h>


// The list of JavaScript keywords that can be used by other friendly lexers.
const char *XsciLexerJavaScript::keywordClass =
    "abstract boolean break byte case catch char class const continue "
    "debugger default delete do double else enum export extends final "
    "finally float for function goto if implements import in instanceof "
    "int interface long native new package private protected public "
    "return short static super switch synchronized this throw throws "
    "transient try typeof var void volatile while with";


// The ctor.
XsciLexerJavaScript::XsciLexerJavaScript(QObject *parent)
    : XsciLexerCPP(parent)
{
}


// The dtor.
XsciLexerJavaScript::~XsciLexerJavaScript()
{
}


// Returns the language name.
const char *XsciLexerJavaScript::language() const
{
    return "JavaScript";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerJavaScript::defaultColor(int style) const
{
    if (style == Regex)
        return QColor(0x3f,0x7f,0x3f);

    return XsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerJavaScript::defaultEolFill(int style) const
{
    if (style == Regex)
        return true;

    return XsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerJavaScript::defaultFont(int style) const
{
    if (style == Regex)
#if defined(Q_OS_WIN)
        return QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        return QFont("Courier", 12);
#else
        return QFont("Bitstream Vera Sans Mono",9);
#endif

    return XsciLexerCPP::defaultFont(style);
}


// Returns the set of keywords.
const char *XsciLexerJavaScript::keywords(int set) const
{
    if (set != 1)
        return 0;

    return keywordClass;
}


// Returns the user name of a style.
QString XsciLexerJavaScript::description(int style) const
{
    if (style == Regex)
        return tr("Regular expression");

    return XsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor XsciLexerJavaScript::defaultPaper(int style) const
{
    if (style == Regex)
        return QColor(0xe0,0xf0,0xff);

    return XsciLexer::defaultPaper(style);
}
