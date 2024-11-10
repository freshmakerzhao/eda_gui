// This defines the interface to the XsciLexerMakefile class.
 


#ifndef XSCILEXERMAKEFILE_H
#define XSCILEXERMAKEFILE_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerMakefile class encapsulates the Scintilla
//! Makefile lexer.
class XSCINTILLA_EXPORT XsciLexerMakefile : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Makefile lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A pre-processor directive.
        Preprocessor = 2,

        //! A variable.
        Variable = 3,

        //! An operator.
        Operator = 4,

        //! A target.
        Target = 5,

        //! An error.
        Error = 9
    };

    //! Construct a XsciLexerMakefile with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerMakefile(QObject *parent = 0);

    //! Destroys the XsciLexerMakefile instance.
    virtual ~XsciLexerMakefile();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

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

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerMakefile(const XsciLexerMakefile &);
    XsciLexerMakefile &operator=(const XsciLexerMakefile &);
};

#endif
