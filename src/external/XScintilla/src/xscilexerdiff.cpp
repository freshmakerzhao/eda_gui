// This module implements the XsciLexerDiff class.
 


#include "Xsci/xscilexerdiff.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerDiff::XsciLexerDiff(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerDiff::~XsciLexerDiff()
{
}


// Returns the language name.
const char *XsciLexerDiff::language() const
{
    return "Diff";
}


// Returns the lexer name.
const char *XsciLexerDiff::lexer() const
{
    return "diff";
}


// Return the string of characters that comprise a word.
const char *XsciLexerDiff::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerDiff::defaultColor(int style) const
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
    case AddingPatchRemoved:
    case RemovingPatchRemoved:
        return QColor(0x00,0x7f,0x7f);

    case LineAdded:
    case AddingPatchAdded:
    case RemovingPatchAdded:
        return QColor(0x00,0x00,0x7f);

    case LineChanged:
        return QColor(0x7f,0x7f,0x7f);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString XsciLexerDiff::description(int style) const
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

    case AddingPatchAdded:
        return tr("Added adding patch");

    case RemovingPatchAdded:
        return tr("Removed adding patch");

    case AddingPatchRemoved:
        return tr("Added removing patch");

    case RemovingPatchRemoved:
        return tr("Removed removing patch");
    }

    return QString();
}
