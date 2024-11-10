// This defines the interface to the XsciLexerJava class.
 


#ifndef XSCILEXERJAVA_H
#define XSCILEXERJAVA_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexercpp.h>


//! \brief The XsciLexerJava class encapsulates the Scintilla Java lexer.
class XSCINTILLA_EXPORT XsciLexerJava : public XsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a XsciLexerJava with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerJava(QObject *parent = 0);

    //! Destroys the XsciLexerJava instance.
    virtual ~XsciLexerJava();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    XsciLexerJava(const XsciLexerJava &);
    XsciLexerJava &operator=(const XsciLexerJava &);
};

#endif
