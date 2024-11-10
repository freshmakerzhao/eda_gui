// This defines the interface to the XsciLexerCSharp class.
 


#ifndef XSCILEXERCSHARP_H
#define XSCILEXERCSHARP_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexercpp.h>


//! \brief The XsciLexerCSharp class encapsulates the Scintilla C#
//! lexer.
class XSCINTILLA_EXPORT XsciLexerCSharp : public XsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a XsciLexerCSharp with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerCSharp(QObject *parent = 0);

    //! Destroys the XsciLexerCSharp instance.
    virtual ~XsciLexerCSharp();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerCSharp(const XsciLexerCSharp &);
    XsciLexerCSharp &operator=(const XsciLexerCSharp &);
};

#endif
