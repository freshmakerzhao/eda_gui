// This module implements the XsciLexerProperties class.
 


#include "Xsci/xscilexerproperties.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerProperties::XsciLexerProperties(QObject *parent)
    : XsciLexer(parent), fold_compact(true), initial_spaces(true)
{
}


// The dtor.
XsciLexerProperties::~XsciLexerProperties()
{
}


// Returns the language name.
const char *XsciLexerProperties::language() const
{
    return "Properties";
}


// Returns the lexer name.
const char *XsciLexerProperties::lexer() const
{
    return "props";
}


// Return the string of characters that comprise a word.
const char *XsciLexerProperties::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerProperties::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00,0x7f,0x7f);

    case Section:
        return QColor(0x7f,0x00,0x7f);

    case Assignment:
        return QColor(0xb0,0x60,0x00);

    case DefaultValue:
        return QColor(0x7f,0x7f,0x00);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerProperties::defaultEolFill(int style) const
{
    if (style == Section)
        return true;

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerProperties::defaultFont(int style) const
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
QString XsciLexerProperties::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Section:
        return tr("Section");

    case Assignment:
        return tr("Assignment");

    case DefaultValue:
        return tr("Default value");

    case Key:
        return tr("Key");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerProperties::defaultPaper(int style) const
{
    if (style == Section)
        return QColor(0xe0,0xf0,0xf0);

    return XsciLexer::defaultPaper(style);
}


// Refresh all properties.
void XsciLexerProperties::refreshProperties()
{
    setCompactProp();
    setInitialSpacesProp();
}


// Read properties from the settings.
bool XsciLexerProperties::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    initial_spaces = qs.value(prefix + "initialspaces", true).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerProperties::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "initialspaces", initial_spaces);

    return rc;
}


// Set if folds are compact
void XsciLexerProperties::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void XsciLexerProperties::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Set if initial spaces are allowed.
void XsciLexerProperties::setInitialSpaces(bool enable)
{
    initial_spaces = enable;

    setInitialSpacesProp();
}


// Set the "lexer.props.allow.initial.spaces" property.
void XsciLexerProperties::setInitialSpacesProp()
{
    emit propertyChanged("lexer.props.allow.initial.spaces", (fold_compact ? "1" : "0"));
}
