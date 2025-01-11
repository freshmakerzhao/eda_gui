// This module implements the ExsciLexerDiff class.



#include "Exsci/exscilexerdiff.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
ExsciLexerDiff::ExsciLexerDiff(QObject *parent)
    : ExsciLexer(parent)
{
}


// The dtor.
ExsciLexerDiff::~ExsciLexerDiff()
{
}


// Returns the language name.
const char *ExsciLexerDiff::language() const
{
    return "Diff";
}


// Returns the lexer name.
const char *ExsciLexerDiff::lexer() const
{
    return "diff";
}


// Return the string of characters that comprise a word.
const char *ExsciLexerDiff::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerDiff::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Command:
        return QColor(0x7f,0x7f,0x00);

    case Header:
        return QColor(0x7f,0x00,0x00);

    case Position:
        return QColor(0x7f,0x00,0x7f);

    case LineRemoved:
        return QColor(0x00,0x7f,0x7f);

    case LineAdded:
        return QColor(0x00,0x00,0x7f);

    case LineChanged:
        return QColor(0x7f,0x7f,0x7f);
    }

    return ExsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString ExsciLexerDiff::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Command:
        return tr("Command");

    case Header:
        return tr("Header");

    case Position:
        return tr("Position");

    case LineRemoved:
        return tr("Removed line");

    case LineAdded:
        return tr("Added line");

    case LineChanged:
        return tr("Changed line");
    }

    return QString();
}
