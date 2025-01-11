// This module implements the ExsciLexerMatlab class.



#include "Exsci/exscilexermatlab.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
ExsciLexerMatlab::ExsciLexerMatlab(QObject *parent)
    : ExsciLexer(parent)
{
}


// The dtor.
ExsciLexerMatlab::~ExsciLexerMatlab()
{
}


// Returns the language name.
const char *ExsciLexerMatlab::language() const
{
    return "Matlab";
}


// Returns the lexer name.
const char *ExsciLexerMatlab::lexer() const
{
    return "matlab";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerMatlab::defaultColor(int style) const
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

    return ExsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerMatlab::defaultFont(int style) const
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
        f = ExsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = ExsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *ExsciLexerMatlab::keywords(int set) const
{
    if (set == 1)
        return
            "break case catch continue else elseif end for function "
            "global if otherwise persistent return switch try while";

    return 0;
}


// Returns the user name of a style.
QString ExsciLexerMatlab::description(int style) const
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
