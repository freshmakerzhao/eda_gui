// This defines the interface to the XsciLexerFortran class.
 


#ifndef XSCILEXERFORTRAN_H
#define XSCILEXERFORTRAN_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexerfortran77.h>


//! \brief The XsciLexerFortran class encapsulates the Scintilla Fortran lexer.
class XSCINTILLA_EXPORT XsciLexerFortran : public XsciLexerFortran77
{
    Q_OBJECT

public:
    //! Construct a XsciLexerFortran with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerFortran(QObject *parent = 0);

    //! Destroys the XsciLexerFortran instance.
    virtual ~XsciLexerFortran();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    XsciLexerFortran(const XsciLexerFortran &);
    XsciLexerFortran &operator=(const XsciLexerFortran &);
};

#endif
