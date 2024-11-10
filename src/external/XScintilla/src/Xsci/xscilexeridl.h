// This defines the interface to the XsciLexerIDL class.
 


#ifndef XSCILEXERIDL_H
#define XSCILEXERIDL_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexercpp.h>


//! \brief The XsciLexerIDL class encapsulates the Scintilla IDL
//! lexer.
class XSCINTILLA_EXPORT XsciLexerIDL : public XsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a XsciLexerIDL with parent \a parent.  \a parent is typically
    //! the XsciScintilla instance.
    XsciLexerIDL(QObject *parent = 0);

    //! Destroys the XsciLexerIDL instance.
    virtual ~XsciLexerIDL();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerIDL(const XsciLexerIDL &);
    XsciLexerIDL &operator=(const XsciLexerIDL &);
};

#endif
