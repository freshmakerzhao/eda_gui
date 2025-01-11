// This defines the interface to the ExsciLexerDiff class.



#ifndef EXSCILEXERDIFF_H
#define EXSCILEXERDIFF_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerDiff class encapsulates the Scintilla Diff
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerDiff : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Diff lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A command.
        Command = 2,

        //! A header.
        Header = 3,

        //! A position.
        Position = 4,

        //! A removed line.
        LineRemoved = 5,

        //! An added line.
        LineAdded = 6,

        //! A changed line.
        LineChanged = 7
    };

    //! Construct a ExsciLexerDiff with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerDiff(QObject *parent = 0);

    //! Destroys the ExsciLexerDiff instance.
    virtual ~ExsciLexerDiff();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    ExsciLexerDiff(const ExsciLexerDiff &);
    ExsciLexerDiff &operator=(const ExsciLexerDiff &);
};

#endif
