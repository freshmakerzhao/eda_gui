// This module implements the XsciLexerJava class.
 


#include "Xsci/xscilexerjava.h"


// The ctor.
XsciLexerJava::XsciLexerJava(QObject *parent)
    : XsciLexerCPP(parent)
{
}


// The dtor.
XsciLexerJava::~XsciLexerJava()
{
}


// Returns the language name.
const char *XsciLexerJava::language() const
{
    return "Java";
}


// Returns the set of keywords.
const char *XsciLexerJava::keywords(int set) const
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
