// This module defines interface to the XsciStyledText class.
 


#ifndef XSCISTYLEDTEXT_H
#define XSCISTYLEDTEXT_H

#include <qstring.h>

#include <Xsci/xsciglobal.h>


class XsciScintillaBase;
class XsciStyle;


//! \brief The XsciStyledText class is a container for a piece of text and the
//! style used to display the text.
class XSCINTILLA_EXPORT XsciStyledText
{
public:
    //! Constructs a XsciStyledText instance for text \a text and style number
    //! \a style.
    XsciStyledText(const QString &text, int style);

    //! Constructs a XsciStyledText instance for text \a text and style \a
    //! style.
    XsciStyledText(const QString &text, const XsciStyle &style);

    //! \internal Apply the style to a particular editor.
    void apply(XsciScintillaBase *sci) const;

    //! Returns a reference to the text.
    const QString &text() const {return styled_text;}

    //! Returns the number of the style.
    int style() const;

private:
    QString styled_text;
    int style_nr;
    const XsciStyle *explicit_style;
};

#endif
