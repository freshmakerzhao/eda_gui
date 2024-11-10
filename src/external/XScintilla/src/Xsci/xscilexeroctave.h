// This defines the interface to the XsciLexerOctave class.
 


#ifndef XSCILEXEROCTAVE_H
#define XSCILEXEROCTAVE_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexermatlab.h>


//! \brief The XsciLexerOctave class encapsulates the Scintilla Octave file
//! lexer.
class XSCINTILLA_EXPORT XsciLexerOctave : public XsciLexerMatlab
{
    Q_OBJECT

public:
    //! Construct a XsciLexerOctave with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerOctave(QObject *parent = 0);

    //! Destroys the XsciLexerOctave instance.
    virtual ~XsciLexerOctave();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    XsciLexerOctave(const XsciLexerOctave &);
    XsciLexerOctave &operator=(const XsciLexerOctave &);
};

#endif
