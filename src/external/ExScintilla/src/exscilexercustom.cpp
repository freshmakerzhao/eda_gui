// This module implements the ExsciLexerCustom class.



#include "Exsci/exscilexercustom.h"

#include "Exsci/exsciscintilla.h"
#include "Exsci/exsciscintillabase.h"
#include "Exsci/exscistyle.h"


// The ctor.
ExsciLexerCustom::ExsciLexerCustom(QObject *parent)
    : ExsciLexer(parent)
{
}


// The dtor.
ExsciLexerCustom::~ExsciLexerCustom()
{
}


// Start styling.
void ExsciLexerCustom::startStyling(int start, int)
{
    if (!editor())
        return;

    editor()->SendScintilla(ExsciScintillaBase::SCI_STARTSTYLING, start);
}


// Set the style for a number of characters.
void ExsciLexerCustom::setStyling(int length, int style)
{
    if (!editor())
        return;

    editor()->SendScintilla(ExsciScintillaBase::SCI_SETSTYLING, length, style);
}


// Set the style for a number of characters.
void ExsciLexerCustom::setStyling(int length, const ExsciStyle &style)
{
    setStyling(length, style.style());
}


// Set the attached editor.
void ExsciLexerCustom::setEditor(ExsciScintilla *new_editor)
{
    if (editor())
        disconnect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));

    ExsciLexer::setEditor(new_editor);

    if (editor())
        connect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));
}


// Return the number of style bits needed by the lexer.
int ExsciLexerCustom::styleBitsNeeded() const
{
    return 5;
}


// Handle a request to style some text.
void ExsciLexerCustom::handleStyleNeeded(int pos)
{
    int start = editor()->SendScintilla(ExsciScintillaBase::SCI_GETENDSTYLED);
    int line = editor()->SendScintilla(ExsciScintillaBase::SCI_LINEFROMPOSITION,
            start);
    start = editor()->SendScintilla(ExsciScintillaBase::SCI_POSITIONFROMLINE,
            line);

    if (start != pos)
        styleText(start, pos);
}
