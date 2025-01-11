// This defines the interface to the ExsciLexerJava class.



#ifndef EXSCILEXERJAVA_H
#define EXSCILEXERJAVA_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexercpp.h>


//! \brief The ExsciLexerJava class encapsulates the Scintilla Java lexer.
class EXSCINTILLA_EXPORT ExsciLexerJava : public ExsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerJava with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerJava(QObject *parent = 0);

    //! Destroys the ExsciLexerJava instance.
    virtual ~ExsciLexerJava();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    ExsciLexerJava(const ExsciLexerJava &);
    ExsciLexerJava &operator=(const ExsciLexerJava &);
};

#endif
