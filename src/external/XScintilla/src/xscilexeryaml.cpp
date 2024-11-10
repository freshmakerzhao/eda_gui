// This module implements the XsciLexerYAML class.
 


#include "Xsci/xscilexeryaml.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
XsciLexerYAML::XsciLexerYAML(QObject *parent)
    : XsciLexer(parent), fold_comments(false)
{
}


// The dtor.
XsciLexerYAML::~XsciLexerYAML()
{
}


// Returns the language name.
const char *XsciLexerYAML::language() const
{
    return "YAML";
}


// Returns the lexer name.
const char *XsciLexerYAML::lexer() const
{
    return "yaml";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerYAML::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x88,0x00);

    case Identifier:
        return QColor(0x00,0x00,0x88);

    case Keyword:
        return QColor(0x88,0x00,0x88);

    case Number:
        return QColor(0x88,0x00,0x00);

    case Reference:
        return QColor(0x00,0x88,0x88);

    case DocumentDelimiter:
    case SyntaxErrorMarker:
        return QColor(0xff,0xff,0xff);

    case TextBlockMarker:
        return QColor(0x33,0x33,0x66);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool XsciLexerYAML::defaultEolFill(int style) const
{
    if (style == DocumentDelimiter || style == SyntaxErrorMarker)
        return true;

    return XsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont XsciLexerYAML::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Default:
    case TextBlockMarker:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        break;

    case Identifier:
        f = XsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DocumentDelimiter:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setBold(true);
        break;

    case SyntaxErrorMarker:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        f.setBold(true);
        f.setItalic(true);
        break;

    default:
        f = XsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *XsciLexerYAML::keywords(int set) const
{
    if (set == 1)
        return "true false yes no";

    return 0;
}


// Returns the user name of a style.
QString XsciLexerYAML::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Identifier:
        return tr("Identifier");

    case Keyword:
        return tr("Keyword");

    case Number:
        return tr("Number");

    case Reference:
        return tr("Reference");

    case DocumentDelimiter:
        return tr("Document delimiter");

    case TextBlockMarker:
        return tr("Text block marker");

    case SyntaxErrorMarker:
        return tr("Syntax error marker");

    case Operator:
        return tr("Operator");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor XsciLexerYAML::defaultPaper(int style) const
{
    switch (style)
    {
    case DocumentDelimiter:
        return QColor(0x00,0x00,0x88);

    case SyntaxErrorMarker:
        return QColor(0xff,0x00,0x00);
    }

    return XsciLexer::defaultPaper(style);
}


// Refresh all properties.
void XsciLexerYAML::refreshProperties()
{
    setCommentProp();
}


// Read properties from the settings.
bool XsciLexerYAML::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();

    return rc;
}


// Write properties to the settings.
bool XsciLexerYAML::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);

    return rc;
}


// Return true if comments can be folded.
bool XsciLexerYAML::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void XsciLexerYAML::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment.yaml" property.
void XsciLexerYAML::setCommentProp()
{
    emit propertyChanged("fold.comment.yaml",(fold_comments ? "1" : "0"));
}
