// This module implements the ExsciAbstractAPIs class.



#include "Exsci/exsciabstractapis.h"

#include "Exsci/exscilexer.h"


// The ctor.
ExsciAbstractAPIs::ExsciAbstractAPIs(ExsciLexer *lexer)
    : QObject(lexer), lex(lexer)
{
    lexer->setAPIs(this);
}


// The dtor.
ExsciAbstractAPIs::~ExsciAbstractAPIs()
{
}


// Return the lexer.
ExsciLexer *ExsciAbstractAPIs::lexer() const
{
    return lex;
}


// Called when the user has made a selection from the auto-completion list.
void ExsciAbstractAPIs::autoCompletionSelected(const QString &selection)
{
}
