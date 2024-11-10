// This module implements the XsciLexerBatch class.
 


#include "Xsci/xscilexerbatch.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerBatch::XsciLexerBatch(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerBatch::~XsciLexerBatch()
{
}


// Returns the language name.
const char *XsciLexerBatch::language() const
{
    return "Batch";
}


// Returns the lexer name.
const char *XsciLexerBatch::lexer() const
{
    return "batch";
}


// Return the string of characters that comprise a word.
const char *XsciLexerBatch::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerBatch::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Keyword:
    case ExternalCommand:
        return QColor(0x00,0x00,0x7f);

    case Label:
        return QColor(0x7f,0x00,0x7f);

    case HideCommandChar:
        return QColor(0x7f,0x7f,0x00);

    case Variable:
        return QColor(0x80,0x00,0x80);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerBatch::defaultEolFill(int style) const
{
    switch (style)
    {
    case Label:
        return true;
    }

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerBatch::defaultFont(int style) const
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
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case ExternalCommand:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        f.setBold(true);
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerBatch::keywords(int set) const
{
    if (set == 1)
        return
            "rem set if exist errorlevel for in do break call "
            "chcp cd chdir choice cls country ctty date del "
            "erase dir echo exit goto loadfix loadhigh mkdir md "
            "move path pause prompt rename ren rmdir rd shift "
            "time type ver verify vol com con lpt nul";

    return 0;
}


// Return the case sensitivity.
bool XsciLexerBatch::caseSensitive() const
{
    return false;
}


// Returns the user name of a style.
QString XsciLexerBatch::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Keyword:
        return tr("Keyword");

    case Label:
        return tr("Label");

    case HideCommandChar:
        return tr("Hide command character");

    case ExternalCommand:
        return tr("External command");

    case Variable:
        return tr("Variable");

    case Operator:
        return tr("Operator");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerBatch::defaultPaper(int style) const
{
    switch (style)
    {
    case Label:
        return QColor(0x60,0x60,0x60);
    }

    return XsciLexer::defaultPaper(style);
}
