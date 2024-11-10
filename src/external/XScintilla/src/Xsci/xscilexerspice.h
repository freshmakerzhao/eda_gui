// This defines the interface to the XsciLexerSpice class.
 


#ifndef XSCILEXERSPICE_H
#define XSCILEXERSPICE_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerSpice class encapsulates the Scintilla Spice lexer.
class XSCINTILLA_EXPORT XsciLexerSpice : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Spice lexer.
    enum {
        //! The default.
        Default = 0,

        //! An identifier.
        Identifier = 1,

        //! A command.
        Command = 2,

        //! A function.
        Function = 3,

        //! A parameter.
        Parameter = 4,

        //! A number.
        Number = 5,

        //! A delimiter.
        Delimiter = 6,

        //! A value.
        Value = 7,

        //! A comment.
        Comment = 8
    };

    //! Construct a XsciLexerSpice with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerSpice(QObject *parent = 0);

    //! Destroys the XsciLexerSpice instance.
    virtual ~XsciLexerSpice();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerSpice(const XsciLexerSpice &);
    XsciLexerSpice &operator=(const XsciLexerSpice &);
};

#endif
