// This module implements the XsciStyledText class.
 


#include "Xsci/xscistyledtext.h"

#include "Xsci/xsciscintillabase.h"
#include "Xsci/xscistyle.h"


// A ctor.
XsciStyledText::XsciStyledText(const QString &text, int style)
    : styled_text(text), style_nr(style), explicit_style(0)
{
}


// A ctor.
XsciStyledText::XsciStyledText(const QString &text, const XsciStyle &style)
    : styled_text(text), style_nr(-1)
{
    explicit_style = new XsciStyle(style);
}


// Return the number of the style.
int XsciStyledText::style() const
{
    return explicit_style ? explicit_style->style() : style_nr;
}


// Apply any explicit style to an editor.
void XsciStyledText::apply(XsciScintillaBase *sci) const
{
    if (explicit_style)
        explicit_style->apply(sci);
}
