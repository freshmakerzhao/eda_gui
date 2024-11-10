// This module implements the XsciLexerXML class.
 


#include "Xsci/xscilexerxml.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerXML::XsciLexerXML(QObject *parent)
    : XsciLexerHTML(parent), scripts(true)
{
}


// The dtor.
XsciLexerXML::~XsciLexerXML()
{
}


// Returns the language name.
const char *XsciLexerXML::language() const
{
    return "XML";
}


// Returns the lexer name.
const char *XsciLexerXML::lexer() const
{
    return "xml";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerXML::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Tag:
    case UnknownTag:
    case XMLTagEnd:
    case SGMLDefault:
    case SGMLCommand:
        return QColor(0x00,0x00,0x80);

    case Attribute:
    case UnknownAttribute:
        return QColor(0x00,0x80,0x80);

    case HTMLNumber:
        return QColor(0x00,0x7f,0x7f);

    case HTMLDoubleQuotedString:
    case HTMLSingleQuotedString:
        return QColor(0x7f,0x00,0x7f);

    case OtherInTag:
    case Entity:
    case XMLStart:
    case XMLEnd:
        return QColor(0x80,0x00,0x80);

    case HTMLComment:
    case SGMLComment:
        return QColor(0x80,0x80,0x00);

    case CDATA:
    case PHPStart:
    case SGMLDoubleQuotedString:
    case SGMLError:
        return QColor(0x80,0x00,0x00);

    case HTMLValue:
        return QColor(0x60,0x80,0x60);

    case SGMLParameter:
        return QColor(0x00,0x66,0x00);

    case SGMLSingleQuotedString:
        return QColor(0x99,0x33,0x00);

    case SGMLSpecial:
        return QColor(0x33,0x66,0xff);

    case SGMLEntity:
        return QColor(0x33,0x33,0x33);

    case SGMLBlockDefault:
        return QColor(0x00,0x00,0x66);
    }

    return XsciLexerHTML::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerXML::defaultEolFill(int style) const
{
    if (style == CDATA)
        return true;

    return XsciLexerHTML::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerXML::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Default:
    case Entity:
    case CDATA:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman",11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter",10);
#endif
        break;

    case XMLStart:
    case XMLEnd:
    case SGMLCommand:
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = XsciLexerHTML::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerXML::keywords(int set) const
{
    if (set == 6)
        return XsciLexerHTML::keywords(set);

    return 0;
}


// Returns the background colour of the text for a style.
QColor XsciLexerXML::defaultPaper(int style) const
{
    switch (style)
    {
    case CDATA:
        return QColor(0xff,0xf0,0xf0);

    case SGMLDefault:
    case SGMLCommand:
    case SGMLParameter:
    case SGMLDoubleQuotedString:
    case SGMLSingleQuotedString:
    case SGMLSpecial:
    case SGMLEntity:
    case SGMLComment:
        return QColor(0xef,0xef,0xff);

    case SGMLError:
        return QColor(0xff,0x66,0x66);

    case SGMLBlockDefault:
        return QColor(0xcc,0xcc,0xe0);
    }

    return XsciLexerHTML::defaultPaper(style);
}


// Refresh all properties.
void XsciLexerXML::refreshProperties()
{
    setScriptsProp();
}


// Read properties from the settings.
bool XsciLexerXML::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = XsciLexerHTML::readProperties(qs, prefix);

    scripts = qs.value(prefix + "scriptsstyled", true).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerXML::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = XsciLexerHTML::writeProperties(qs, prefix);

    qs.setValue(prefix + "scriptsstyled", scripts);

    return rc;
}


// Return true if scripts are styled.
bool XsciLexerXML::scriptsStyled() const
{
    return scripts;
}


// Set if scripts are styled.
void XsciLexerXML::setScriptsStyled(bool styled)
{
    scripts = styled;

    setScriptsProp();
}


// Set the "lexer.xml.allow.scripts" property.
void XsciLexerXML::setScriptsProp()
{
    emit propertyChanged("lexer.xml.allow.scripts",(scripts ? "1" : "0"));
}
