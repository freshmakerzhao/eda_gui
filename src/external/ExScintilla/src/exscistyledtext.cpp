// This module implements the ExsciStyledText class.



#include "Exsci/exscistyledtext.h"

#include "Exsci/exsciscintillabase.h"
#include "Exsci/exscistyle.h"


// A ctor.
ExsciStyledText::ExsciStyledText(const QString &text, int style)
    : styled_text(text), style_nr(style), explicit_style(0)
{
}


// A ctor.
ExsciStyledText::ExsciStyledText(const QString &text, const ExsciStyle &style)
    : styled_text(text), style_nr(-1)
{
    explicit_style = new ExsciStyle(style);
}


// Return the number of the style.
int ExsciStyledText::style() const
{
    return explicit_style ? explicit_style->style() : style_nr;
}


// Apply any explicit style to an editor.
void ExsciStyledText::apply(ExsciScintillaBase *sci) const
{
    if (explicit_style)
        explicit_style->apply(sci);
}
