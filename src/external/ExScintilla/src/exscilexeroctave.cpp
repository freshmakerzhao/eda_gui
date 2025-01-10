// This module implements the ExsciLexerOctave class.



#include "Exsci/exscilexeroctave.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
ExsciLexerOctave::ExsciLexerOctave(QObject *parent)
    : ExsciLexerMatlab(parent)
{
}


// The dtor.
ExsciLexerOctave::~ExsciLexerOctave()
{
}


// Returns the language name.
const char *ExsciLexerOctave::language() const
{
    return "Octave";
}


// Returns the lexer name.
const char *ExsciLexerOctave::lexer() const
{
    return "octave";
}


// Returns the set of keywords.
const char *ExsciLexerOctave::keywords(int set) const
{
    if (set == 1)
        return
            "__FILE__ __LINE__ break case catch classdef continue do else "
            "elseif end end_try_catch end_unwind_protect endclassdef "
            "endenumeration endevents endfor endfunction endif endmethods "
            "endparfor endproperties endswitch endwhile enumeration events "
            "for function get global if methods otherwise parfor persistent "
            "properties return set static switch try until unwind_protect "
            "unwind_protect_cleanup while";

    return 0;
}
