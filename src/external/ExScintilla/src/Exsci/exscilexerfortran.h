// This defines the interface to the ExsciLexerFortran class.



#ifndef EXSCILEXERFORTRAN_H
#define EXSCILEXERFORTRAN_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexerfortran77.h>


//! \brief The ExsciLexerFortran class encapsulates the Scintilla Fortran lexer.
class EXSCINTILLA_EXPORT ExsciLexerFortran : public ExsciLexerFortran77
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerFortran with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerFortran(QObject *parent = 0);

    //! Destroys the ExsciLexerFortran instance.
    virtual ~ExsciLexerFortran();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    ExsciLexerFortran(const ExsciLexerFortran &);
    ExsciLexerFortran &operator=(const ExsciLexerFortran &);
};

#endif
