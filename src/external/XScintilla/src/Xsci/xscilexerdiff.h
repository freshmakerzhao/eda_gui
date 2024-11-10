// This defines the interface to the XsciLexerDiff class.
 


#ifndef XSCILEXERDIFF_H
#define XSCILEXERDIFF_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerDiff class encapsulates the Scintilla Diff
//! lexer.
class XSCINTILLA_EXPORT XsciLexerDiff : public XsciLexer
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

        //! A line removed.
        LineRemoved = 5,

        //! A line added.
        LineAdded = 6,

        //! A line changed.
        LineChanged = 7,

        //! An adding patch added.
        AddingPatchAdded = 8,

        //! A removing patch added.
        RemovingPatchAdded = 9,

        //! An adding patch added.
        AddingPatchRemoved = 10,

        //! A removing patch added.
        RemovingPatchRemoved = 11,
    };

    //! Construct a XsciLexerDiff with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerDiff(QObject *parent = 0);

    //! Destroys the XsciLexerDiff instance.
    virtual ~XsciLexerDiff();

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
    XsciLexerDiff(const XsciLexerDiff &);
    XsciLexerDiff &operator=(const XsciLexerDiff &);
};

#endif
