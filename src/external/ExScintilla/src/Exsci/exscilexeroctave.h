// This defines the interface to the ExsciLexerOctave class.



#ifndef EXSCILEXEROCTAVE_H
#define EXSCILEXEROCTAVE_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexermatlab.h>


//! \brief The ExsciLexerOctave class encapsulates the Scintilla Octave file
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerOctave : public ExsciLexerMatlab
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerOctave with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerOctave(QObject *parent = 0);

    //! Destroys the ExsciLexerOctave instance.
    virtual ~ExsciLexerOctave();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    ExsciLexerOctave(const ExsciLexerOctave &);
    ExsciLexerOctave &operator=(const ExsciLexerOctave &);
};

#endif
