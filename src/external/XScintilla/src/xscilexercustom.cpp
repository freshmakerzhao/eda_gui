// This module implements the XsciLexerCustom class.
 


#include "Xsci/xscilexercustom.h"

#include "Xsci/xsciscintilla.h"
#include "Xsci/xsciscintillabase.h"
#include "Xsci/xscistyle.h"


// The ctor.
XsciLexerCustom::XsciLexerCustom(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerCustom::~XsciLexerCustom()
{
}


// Start styling.
void XsciLexerCustom::startStyling(int start, int)
{
    if (!editor())
        return;

    editor()->SendScintilla(XsciScintillaBase::SCI_STARTSTYLING, start);
}


// Set the style for a number of characters.
void XsciLexerCustom::setStyling(int length, int style)
{
    if (!editor())
        return;

    editor()->SendScintilla(XsciScintillaBase::SCI_SETSTYLING, length, style);
}


// Set the style for a number of characters.
void XsciLexerCustom::setStyling(int length, const XsciStyle &style)
{
    setStyling(length, style.style());
}


// Set the attached editor.
void XsciLexerCustom::setEditor(XsciScintilla *new_editor)
{
    if (editor())
        disconnect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));

    XsciLexer::setEditor(new_editor);

    if (editor())
        connect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));
}


// Return the number of style bits needed by the lexer.
int XsciLexerCustom::styleBitsNeeded() const
{
    return 5;
}


// Handle a request to style some text.
void XsciLexerCustom::handleStyleNeeded(int pos)
{
    int start = editor()->SendScintilla(XsciScintillaBase::SCI_GETENDSTYLED);
    int line = editor()->SendScintilla(XsciScintillaBase::SCI_LINEFROMPOSITION,
            start);
    start = editor()->SendScintilla(XsciScintillaBase::SCI_POSITIONFROMLINE,
            line);

    if (start != pos)
        styleText(start, pos);
}
