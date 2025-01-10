// This module implements the ExsciLexerProperties class.



#include "Exsci/exscilexerproperties.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
ExsciLexerProperties::ExsciLexerProperties(QObject *parent)
    : ExsciLexer(parent), fold_compact(true), initial_spaces(true)
{
}


// The dtor.
ExsciLexerProperties::~ExsciLexerProperties()
{
}


// Returns the language name.
const char *ExsciLexerProperties::language() const
{
    return "Properties";
}


// Returns the lexer name.
const char *ExsciLexerProperties::lexer() const
{
    return "props";
}


// Return the string of characters that comprise a word.
const char *ExsciLexerProperties::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerProperties::defaultColor(int style) const
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

    return ExsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool ExsciLexerProperties::defaultEolFill(int style) const
{
    if (style == Section)
        return true;

    return ExsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerProperties::defaultFont(int style) const
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
        f = ExsciLexer::defaultFont(style);

    return f;
}


// Returns the user name of a style.
QString ExsciLexerProperties::description(int style) const
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
QColor ExsciLexerProperties::defaultPaper(int style) const
{
    if (style == Section)
        return QColor(0xe0,0xf0,0xf0);

    return ExsciLexer::defaultPaper(style);
}


// Refresh all properties.
void ExsciLexerProperties::refreshProperties()
{
    setCompactProp();
    setInitialSpacesProp();
}


// Read properties from the settings.
bool ExsciLexerProperties::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    initial_spaces = qs.value(prefix + "initialspaces", true).toBool();

    return rc;
}


// Write properties to the settings.
bool ExsciLexerProperties::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "initialspaces", initial_spaces);

    return rc;
}


// Set if folds are compact
void ExsciLexerProperties::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void ExsciLexerProperties::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Set if initial spaces are allowed.
void ExsciLexerProperties::setInitialSpaces(bool enable)
{
    initial_spaces = enable;

    setInitialSpacesProp();
}


// Set the "lexer.props.allow.initial.spaces" property.
void ExsciLexerProperties::setInitialSpacesProp()
{
    emit propertyChanged("lexer.props.allow.initial.spaces", (fold_compact ? "1" : "0"));
}
