// This module implements the ExsciLexerCSS class.



#include "Exsci/exscilexercss.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
ExsciLexerCSS::ExsciLexerCSS(QObject *parent)
    : ExsciLexer(parent),
      fold_comments(false), fold_compact(true), hss_language(false),
      less_language(false), scss_language(false)
{
}


// The dtor.
ExsciLexerCSS::~ExsciLexerCSS()
{
}


// Returns the language name.
const char *ExsciLexerCSS::language() const
{
    return "CSS";
}


// Returns the lexer name.
const char *ExsciLexerCSS::lexer() const
{
    return "css";
}


// Return the list of characters that can start a block.
const char *ExsciLexerCSS::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *ExsciLexerCSS::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the string of characters that comprise a word.
const char *ExsciLexerCSS::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
}


// Returns the foreground colour of the text for a style.
QColor ExsciLexerCSS::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0xff,0x00,0x80);

    case Tag:
        return QColor(0x00,0x00,0x7f);

    case PseudoClass:
    case Attribute:
        return QColor(0x80,0x00,0x00);

    case UnknownPseudoClass:
    case UnknownProperty:
        return QColor(0xff,0x00,0x00);

    case Operator:
        return QColor(0x00,0x00,0x00);

    case CSS1Property:
        return QColor(0x00,0x40,0xe0);

    case Value:
    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f,0x00,0x7f);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case IDSelector:
        return QColor(0x00,0x7f,0x7f);

    case Important:
        return QColor(0xff,0x80,0x00);

    case AtRule:
    case MediaRule:
        return QColor(0x7f,0x7f,0x00);

    case CSS2Property:
        return QColor(0x00,0xa0,0xe0);
    }

    return ExsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont ExsciLexerCSS::defaultFont(int style) const
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
    {
        f = ExsciLexer::defaultFont(style);

        switch (style)
        {
        case Tag:
        case Important:
        case MediaRule:
            f.setBold(true);
            break;

        case IDSelector:
            f.setItalic(true);
            break;
        }
    }

    return f;
}


// Returns the set of keywords.
const char *ExsciLexerCSS::keywords(int set) const
{
    if (set == 1)
        return
            "color background-color background-image "
            "background-repeat background-attachment "
            "background-position background font-family "
            "font-style font-variant font-weight font-size font "
            "word-spacing letter-spacing text-decoration "
            "vertical-align text-transform text-align "
            "text-indent line-height margin-top margin-right "
            "margin-bottom margin-left margin padding-top "
            "padding-right padding-bottom padding-left padding "
            "border-top-width border-right-width "
            "border-bottom-width border-left-width border-width "
            "border-top border-right border-bottom border-left "
            "border border-color border-style width height float "
            "clear display white-space list-style-type "
            "list-style-image list-style-position list-style";

    if (set == 2)
        return
            "first-letter first-line link active visited "
            "first-child focus hover lang before after left "
            "right first";

    if (set == 3)
        return
            "border-top-color border-right-color "
            "border-bottom-color border-left-color border-color "
            "border-top-style border-right-style "
            "border-bottom-style border-left-style border-style "
            "top right bottom left position z-index direction "
            "unicode-bidi min-width max-width min-height "
            "max-height overflow clip visibility content quotes "
            "counter-reset counter-increment marker-offset size "
            "marks page-break-before page-break-after "
            "page-break-inside page orphans widows font-stretch "
            "font-size-adjust unicode-range units-per-em src "
            "panose-1 stemv stemh slope cap-height x-height "
            "ascent descent widths bbox definition-src baseline "
            "centerline mathline topline text-shadow "
            "caption-side table-layout border-collapse "
            "border-spacing empty-cells speak-header cursor "
            "outline outline-width outline-style outline-color "
            "volume speak pause-before pause-after pause "
            "cue-before cue-after cue play-during azimuth "
            "elevation speech-rate voice-family pitch "
            "pitch-range stress richness speak-punctuation "
            "speak-numeral";

    return 0;
}


// Returns the user name of a style.
QString ExsciLexerCSS::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Tag:
        return tr("Tag");

    case ClassSelector:
        return tr("Class selector");

    case PseudoClass:
        return tr("Pseudo-class");

    case UnknownPseudoClass:
        return tr("Unknown pseudo-class");

    case Operator:
        return tr("Operator");

    case CSS1Property:
        return tr("CSS1 property");

    case UnknownProperty:
        return tr("Unknown property");

    case Value:
        return tr("Value");

    case IDSelector:
        return tr("ID selector");

    case Important:
        return tr("Important");

    case AtRule:
        return tr("@-rule");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case CSS2Property:
        return tr("CSS2 property");

    case Attribute:
        return tr("Attribute");

    case CSS3Property:
        return tr("CSS3 property");

    case PseudoElement:
        return tr("Pseudo-element");

    case ExtendedCSSProperty:
        return tr("Extended CSS property");

    case ExtendedPseudoClass:
        return tr("Extended pseudo-class");

    case ExtendedPseudoElement:
        return tr("Extended pseudo-element");

    case MediaRule:
        return tr("Media rule");

    case Variable:
        return tr("Variable");
    }

    return QString();
}


// Refresh all properties.
void ExsciLexerCSS::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setHSSProp();
    setLessProp();
    setSCSSProp();
}


// Read properties from the settings.
bool ExsciLexerCSS::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    hss_language = qs.value(prefix + "hsslanguage", false).toBool();
    less_language = qs.value(prefix + "lesslanguage", false).toBool();
    scss_language = qs.value(prefix + "scsslanguage", false).toBool();

    return rc;
}


// Write properties to the settings.
bool ExsciLexerCSS::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "hsslanguage", hss_language);
    qs.setValue(prefix + "lesslanguage", less_language);
    qs.setValue(prefix + "scsslanguage", scss_language);

    return rc;
}


// Return true if comments can be folded.
bool ExsciLexerCSS::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void ExsciLexerCSS::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void ExsciLexerCSS::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool ExsciLexerCSS::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void ExsciLexerCSS::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void ExsciLexerCSS::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Set if HSS is supported.
void ExsciLexerCSS::setHSSLanguage(bool enabled)
{
    hss_language = enabled;

    setHSSProp();
}


// Set the "lexer.css.hss.language" property.
void ExsciLexerCSS::setHSSProp()
{
    emit propertyChanged("lexer.css.hss.language",(hss_language ? "1" : "0"));
}


// Set if Less CSS is supported.
void ExsciLexerCSS::setLessLanguage(bool enabled)
{
    less_language = enabled;

    setLessProp();
}


// Set the "lexer.css.less.language" property.
void ExsciLexerCSS::setLessProp()
{
    emit propertyChanged("lexer.css.less.language",(less_language ? "1" : "0"));
}


// Set if Sassy CSS is supported.
void ExsciLexerCSS::setSCSSLanguage(bool enabled)
{
    scss_language = enabled;

    setSCSSProp();
}


// Set the "lexer.css.scss.language" property.
void ExsciLexerCSS::setSCSSProp()
{
    emit propertyChanged("lexer.css.scss.language",(scss_language ? "1" : "0"));
}
