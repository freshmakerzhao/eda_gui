// This defines the interface to the XsciLexerBatch class.
 


#ifndef XSCILEXERBATCH_H
#define XSCILEXERBATCH_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerBatch class encapsulates the Scintilla batch file
//! lexer.
class XSCINTILLA_EXPORT XsciLexerBatch : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! batch file lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A keyword.
        Keyword = 2,

        //! A label.
        Label = 3,

        //! An hide command character.
        HideCommandChar = 4,

        //! An external command .
        ExternalCommand = 5,

        //! A variable.
        Variable = 6,
        
        //! An operator
        Operator = 7
    };

    //! Construct a XsciLexerBatch with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerBatch(QObject *parent = 0);

    //! Destroys the XsciLexerBatch instance.
    virtual ~XsciLexerBatch();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! \internal Returns true if the language is case sensitive.
    bool caseSensitive() const;

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
    XsciLexerBatch(const XsciLexerBatch &);
    XsciLexerBatch &operator=(const XsciLexerBatch &);
};

#endif
