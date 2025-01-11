// This defines the interface to the ExsciLexerTeX class.



#ifndef EXSCILEXERTEX_H
#define EXSCILEXERTEX_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerTeX class encapsulates the Scintilla TeX lexer.
class EXSCINTILLA_EXPORT ExsciLexerTeX : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! TeX lexer.
    enum {
        //! The default.
        Default = 0,

        //! A special.
        Special = 1,

        //! A group.
        Group = 2,

        //! A symbol.
        Symbol = 3,

        //! A command.
        Command = 4,

        //! Text.
        Text = 5
    };

    //! Construct a ExsciLexerTeX with parent \a parent.  \a parent is typically
    //! the ExsciScintilla instance.
    ExsciLexerTeX(QObject *parent = 0);

    //! Destroys the ExsciLexerTeX instance.
    virtual ~ExsciLexerTeX();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a fold is true then multi-line comment blocks can be folded.  The
    //! default is false.
    //!
    //! \sa foldComments()
    void setFoldComments(bool fold);

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const {return fold_comments;}

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    void setFoldCompact(bool fold);

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! If \a enable is true then comments are processed as TeX source
    //! otherwise they are ignored.  The default is false.
    //!
    //! \sa processComments()
    void setProcessComments(bool enable);

    //! Returns true if comments are processed as TeX source.
    //!
    //! \sa setProcessComments()
    bool processComments() const {return process_comments;}

    //! If \a enable is true then \\if<unknown> processed is processed as a
    //! command.  The default is true.
    //!
    //! \sa processIf()
    void setProcessIf(bool enable);

    //! Returns true if \\if<unknown> is processed as a command.
    //!
    //! \sa setProcessIf()
    bool processIf() const {return process_if;}

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs, const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    void setCommentProp();
    void setCompactProp();
    void setProcessCommentsProp();
    void setAutoIfProp();

    bool fold_comments;
    bool fold_compact;
    bool process_comments;
    bool process_if;

    ExsciLexerTeX(const ExsciLexerTeX &);
    ExsciLexerTeX &operator=(const ExsciLexerTeX &);
};

#endif
