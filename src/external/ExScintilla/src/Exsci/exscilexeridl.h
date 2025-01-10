// This defines the interface to the ExsciLexerIDL class.



#ifndef EXSCILEXERIDL_H
#define EXSCILEXERIDL_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexercpp.h>


//! \brief The ExsciLexerIDL class encapsulates the Scintilla IDL
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerIDL : public ExsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerIDL with parent \a parent.  \a parent is typically
    //! the ExsciScintilla instance.
    ExsciLexerIDL(QObject *parent = 0);

    //! Destroys the ExsciLexerIDL instance.
    virtual ~ExsciLexerIDL();

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
    ExsciLexerIDL(const ExsciLexerIDL &);
    ExsciLexerIDL &operator=(const ExsciLexerIDL &);
};

#endif
