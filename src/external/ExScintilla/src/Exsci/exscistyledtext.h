// This module defines interface to the ExsciStyledText class.



#ifndef EXSCISTYLEDTEXT_H
#define EXSCISTYLEDTEXT_H

#include <qstring.h>

#include <Exsci/exsciglobal.h>


class ExsciScintillaBase;
class ExsciStyle;


//! \brief The ExsciStyledText class is a container for a piece of text and the
//! style used to display the text.
class EXSCINTILLA_EXPORT ExsciStyledText
{
public:
    //! Constructs a ExsciStyledText instance for text \a text and style number
    //! \a style.
    ExsciStyledText(const QString &text, int style);

    //! Constructs a ExsciStyledText instance for text \a text and style \a
    //! style.
    ExsciStyledText(const QString &text, const ExsciStyle &style);

    //! \internal Apply the style to a particular editor.
    void apply(ExsciScintillaBase *sci) const;

    //! Returns a reference to the text.
    const QString &text() const {return styled_text;}

    //! Returns the number of the style.
    int style() const;

private:
    QString styled_text;
    int style_nr;
    const ExsciStyle *explicit_style;
};

#endif
