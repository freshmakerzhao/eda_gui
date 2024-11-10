// This module implements the XsciStyle class.
 


#include "Xsci/xscistyle.h"

#include <qapplication.h>

#include "Xsci/xsciscintillabase.h"


// A ctor.
XsciStyle::XsciStyle(int style)
{
    init(style);

    QPalette pal = QApplication::palette();
    setColor(pal.text().color());
    setPaper(pal.base().color());

    setFont(QApplication::font());
    setEolFill(false);
}


// A ctor.
XsciStyle::XsciStyle(int style, const QString &description,
        const QColor &color, const QColor &paper, const QFont &font,
        bool eolFill)
{
    init(style);

    setDescription(description);

    setColor(color);
    setPaper(paper);

    setFont(font);
    setEolFill(eolFill);
}


// Initialisation common to all ctors.
void XsciStyle::init(int style)
{
    // The next style number to allocate.  The initial values corresponds to
    // the amount of space that Scintilla initially creates for styles.
    static int next_style_nr = 63;

    // See if a new style should be allocated.  Note that we allow styles to be
    // passed in that are bigger than STYLE_MAX because the styles used for
    // annotations are allowed to be.
    if (style < 0)
    {
        // Note that we don't deal with the situation where the newly allocated
        // style number has already been used explicitly.
        if (next_style_nr > XsciScintillaBase::STYLE_LASTPREDEFINED)
            style = next_style_nr--;
    }

    style_nr = style;

    // Initialise the minor attributes.
    setTextCase(XsciStyle::OriginalCase);
    setVisible(true);
    setChangeable(true);
    setHotspot(false);
}


// Apply the style to a particular editor.
void XsciStyle::apply(XsciScintillaBase *sci) const
{
    // Don't do anything if the style is invalid.
    if (style_nr < 0)
        return;

    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETFORE, style_nr,
            style_color);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETBACK, style_nr,
            style_paper);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETFONT, style_nr,
            style_font.family().toLatin1().data());
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETSIZEFRACTIONAL, style_nr,
            long(style_font.pointSizeF() * XsciScintillaBase::SC_FONT_SIZE_MULTIPLIER));

    // Pass the Qt weight via the back door.
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETWEIGHT, style_nr,
            -style_font.weight());

    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETITALIC, style_nr,
            style_font.italic());
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETUNDERLINE, style_nr,
            style_font.underline());
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETEOLFILLED, style_nr,
            style_eol_fill);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETCASE, style_nr,
            (long)style_case);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETVISIBLE, style_nr,
            style_visible);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETCHANGEABLE, style_nr,
            style_changeable);
    sci->SendScintilla(XsciScintillaBase::SCI_STYLESETHOTSPOT, style_nr,
            style_hotspot);
}


// Set the color attribute.
void XsciStyle::setColor(const QColor &color)
{
    style_color = color;
}


// Set the paper attribute.
void XsciStyle::setPaper(const QColor &paper)
{
    style_paper = paper;
}


// Set the font attribute.
void XsciStyle::setFont(const QFont &font)
{
    style_font = font;
}


// Set the eol fill attribute.
void XsciStyle::setEolFill(bool eolFill)
{
    style_eol_fill = eolFill;
}


// Set the text case attribute.
void XsciStyle::setTextCase(XsciStyle::TextCase text_case)
{
    style_case = text_case;
}


// Set the visible attribute.
void XsciStyle::setVisible(bool visible)
{
    style_visible = visible;
}


// Set the changeable attribute.
void XsciStyle::setChangeable(bool changeable)
{
    style_changeable = changeable;
}


// Set the hotspot attribute.
void XsciStyle::setHotspot(bool hotspot)
{
    style_hotspot = hotspot;
}


// Refresh the style.  Note that since we had to add apply() then this can't do
// anything useful so we leave it as a no-op.
void XsciStyle::refresh()
{
}
