// This module implements the ExsciLexerJava class.



#include "Exsci/exscilexerjava.h"


// The ctor.
ExsciLexerJava::ExsciLexerJava(QObject *parent)
    : ExsciLexerCPP(parent)
{
}


// The dtor.
ExsciLexerJava::~ExsciLexerJava()
{
}


// Returns the language name.
const char *ExsciLexerJava::language() const
{
    return "Java";
}


// Returns the set of keywords.
const char *ExsciLexerJava::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "abstract assert boolean break byte case catch char class "
           "const continue default do double else extends final finally "
           "float for future generic goto if implements import inner "
           "instanceof int interface long native new null operator outer "
           "package private protected public rest return short static "
           "super switch synchronized this throw throws transient try var "
           "void volatile while";
}
