// This module implements the XsciLexerCSharp class.
 


#include "Xsci/xscilexercsharp.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
XsciLexerCSharp::XsciLexerCSharp(QObject *parent)
    : XsciLexerCPP(parent)
{
}


// The dtor.
XsciLexerCSharp::~XsciLexerCSharp()
{
}


// Returns the language name.
const char *XsciLexerCSharp::language() const
{
    return "C#";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerCSharp::defaultColor(int style) const
{
    if (style == VerbatimString)
        return QColor(0x00,0x7f,0x00);

    return XsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerCSharp::defaultEolFill(int style) const
{
    if (style == VerbatimString)
        return true;

    return XsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerCSharp::defaultFont(int style) const
{
    if (style == VerbatimString)
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
const char *XsciLexerCSharp::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "abstract as base bool break byte case catch char checked "
           "class const continue decimal default delegate do double else "
           "enum event explicit extern false finally fixed float for "
           "foreach goto if implicit in int interface internal is lock "
           "long namespace new null object operator out override params "
           "private protected public readonly ref return sbyte sealed "
           "short sizeof stackalloc static string struct switch this "
           "throw true try typeof uint ulong unchecked unsafe ushort "
           "using virtual void while";
}


// Returns the user name of a style.
QString XsciLexerCSharp::description(int style) const
{
    if (style == VerbatimString)
        return tr("Verbatim string");

    return XsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor XsciLexerCSharp::defaultPaper(int style) const
{
    if (style == VerbatimString)
        return QColor(0xe0,0xff,0xe0);

    return XsciLexer::defaultPaper(style);
}
