// This defines the interface to the ExsciLexerPostScript class.



#ifndef EXSCILEXERPOSTSCRIPT_H
#define EXSCILEXERPOSTSCRIPT_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerPostScript class encapsulates the Scintilla PostScript
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerPostScript : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! PostScript lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A DSC comment.
        DSCComment = 2,

        //! A DSC comment value.
        DSCCommentValue = 3,

        //! A number.
        Number = 4,

        //! A name.
        Name = 5,

        //! A keyword.
        Keyword = 6,

        //! A literal.
        Literal = 7,

        //! An immediately evaluated literal.
        ImmediateEvalLiteral = 8,

        //! Array parenthesis.
        ArrayParenthesis = 9,

        //! Dictionary parenthesis.
        DictionaryParenthesis = 10,

        //! Procedure parenthesis.
        ProcedureParenthesis = 11,

        //! Text.
        Text = 12,

        //! A hexadecimal string.
        HexString = 13,

        //! A base85 string.
        Base85String = 14,

        //! A bad string character.
        BadStringCharacter = 15
    };

    //! Construct a ExsciLexerPostScript with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerPostScript(QObject *parent = 0);

    //! Destroys the ExsciLexerPostScript instance.
    virtual ~ExsciLexerPostScript();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.  Set 5 can be used to provide
    //! additional user defined keywords.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if tokens should be marked.
    //!
    //! \sa setTokenize()
    bool tokenize() const;

    //! Returns the PostScript level.
    //!
    //! \sa setLevel()
    int level() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! Returns true if else blocks can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const;

public slots:
    //! If \a tokenize is true then tokens are marked.  The default is false.
    //!
    //! \sa tokenize()
    virtual void setTokenize(bool tokenize);

    //! The PostScript level is set to \a level.  The default is 3.
    //!
    //! \sa level()
    virtual void setLevel(int level);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! If \a fold is true then else blocks can be folded.  The default is
    //! false.
    //!
    //! \sa foldAtElse()
    virtual void setFoldAtElse(bool fold);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setTokenizeProp();
    void setLevelProp();
    void setCompactProp();
    void setAtElseProp();

    bool ps_tokenize;
    int ps_level;
    bool fold_compact;
    bool fold_atelse;

    ExsciLexerPostScript(const ExsciLexerPostScript &);
    ExsciLexerPostScript &operator=(const ExsciLexerPostScript &);
};

#endif
