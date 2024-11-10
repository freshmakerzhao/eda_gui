// This module implements the XsciLexerMatlab class.
 


#include "Xsci/xscilexermatlab.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
XsciLexerMatlab::XsciLexerMatlab(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerMatlab::~XsciLexerMatlab()
{
}


// Returns the language name.
const char *XsciLexerMatlab::language() const
{
    return "Matlab";
}


// Returns the lexer name.
const char *XsciLexerMatlab::lexer() const
{
    return "matlab";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerMatlab::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Command:
        return QColor(0x7f,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case SingleQuotedString:
    case DoubleQuotedString:
        return QColor(0x7f,0x00,0x7f);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont XsciLexerMatlab::defaultFont(int style) const
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

    case Keyword:
    case Operator:
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerMatlab::keywords(int set) const
{
    if (set == 1)
        return
            "break case catch continue else elseif end for function "
            "global if otherwise persistent return switch try while";

    return 0;
}


// Returns the user name of a style.
QString XsciLexerMatlab::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Command:
        return tr("Command");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case DoubleQuotedString:
        return tr("Double-quoted string");
    }

    return QString();
}
