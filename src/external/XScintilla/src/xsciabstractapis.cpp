// This module implements the XsciAbstractAPIs class.
 


#include "Xsci/xsciabstractapis.h"

#include "Xsci/xscilexer.h"


// The ctor.
XsciAbstractAPIs::XsciAbstractAPIs(XsciLexer *lexer)
    : QObject(lexer), lex(lexer)
{
    lexer->setAPIs(this);
}


// The dtor.
XsciAbstractAPIs::~XsciAbstractAPIs()
{
}


// Return the lexer.
XsciLexer *XsciAbstractAPIs::lexer() const
{
    return lex;
}


// Called when the user has made a selection from the auto-completion list.
void XsciAbstractAPIs::autoCompletionSelected(const QString &selection)
{
    Q_UNUSED(selection);
}
