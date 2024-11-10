// This module implements the XsciLexerMakefile class.
 


#include "Xsci/xscilexermakefile.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
XsciLexerMakefile::XsciLexerMakefile(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerMakefile::~XsciLexerMakefile()
{
}


// Returns the language name.
const char *XsciLexerMakefile::language() const
{
    return "Makefile";
}


// Returns the lexer name.
const char *XsciLexerMakefile::lexer() const
{
    return "makefile";
}


// Return the string of characters that comprise a word.
const char *XsciLexerMakefile::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerMakefile::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Preprocessor:
        return QColor(0x7f,0x7f,0x00);

    case Variable:
        return QColor(0x00,0x00,0x80);

    case Target:
        return QColor(0xa0,0x00,0x00);

    case Error:
        return QColor(0xff,0xff,0x00);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerMakefile::defaultEolFill(int style) const
{
    if (style == Error)
        return true;

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerMakefile::defaultFont(int style) const
{
    QFont f;

    if (style == Comment)
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
    else
        f = XsciLexer::defaultFont(style);

    return f;
}


// Returns the user name of a style.
QString XsciLexerMakefile::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Preprocessor:
        return tr("Preprocessor");

    case Variable:
        return tr("Variable");

    case Operator:
        return tr("Operator");

    case Target:
        return tr("Target");

    case Error:
        return tr("Error");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerMakefile::defaultPaper(int style) const
{
    if (style == Error)
        return QColor(0xff,0x00,0x00);

    return XsciLexer::defaultPaper(style);
}
