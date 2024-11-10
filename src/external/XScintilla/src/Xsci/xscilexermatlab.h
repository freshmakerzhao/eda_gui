// This defines the interface to the XsciLexerMatlab class.
 


#ifndef XSCILEXERMATLAB_H
#define XSCILEXERMATLAB_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerMatlab class encapsulates the Scintilla Matlab file
//! lexer.
class XSCINTILLA_EXPORT XsciLexerMatlab : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Matlab file lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A command.
        Command = 2,

        //! A number.
        Number = 3,

        //! A keyword.
        Keyword = 4,

        //! A single quoted string.
        SingleQuotedString = 5,

        //! An operator
        Operator = 6,

        //! An identifier.
        Identifier = 7,

        //! A double quoted string.
        DoubleQuotedString = 8
    };

    //! Construct a XsciLexerMatlab with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerMatlab(QObject *parent = 0);

    //! Destroys the XsciLexerMatlab instance.
    virtual ~XsciLexerMatlab();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerMatlab(const XsciLexerMatlab &);
    XsciLexerMatlab &operator=(const XsciLexerMatlab &);
};

#endif
