// This defines the interface to the XsciLexerAVS class.
 


#ifndef XSCILEXERAVS_H
#define XSCILEXERAVS_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerAVS class encapsulates the Scintilla AVS lexer.
class XSCINTILLA_EXPORT XsciLexerAVS : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! AVS lexer.
    enum {
        //! The default.
        Default = 0,

        //! A block comment.
        BlockComment = 1,

        //! A nested block comment.
        NestedBlockComment = 2,

        //! A line comment.
        LineComment = 3,

        //! A number.
        Number = 4,

        //! An operator.
        Operator = 5,

        //! An identifier
        Identifier = 6,

        //! A string.
        String = 7,

        //! A triple quoted string.
        TripleString = 8,

        //! A keyword (as defined by keyword set number 1)..
        Keyword = 9,

        //! A filter (as defined by keyword set number 2).
        Filter = 10,

        //! A plugin (as defined by keyword set number 3).
        Plugin = 11,

        //! A function (as defined by keyword set number 4).
        Function = 12,

        //! A clip property (as defined by keyword set number 5).
        ClipProperty = 13,

        //! A keyword defined in keyword set number 6.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet6 = 14
    };

    //! Construct a XsciLexerAVS with parent \a parent.  \a parent is typically
    //! the XsciScintilla instance.
    XsciLexerAVS(QObject *parent = 0);

    //! Destroys the XsciLexerAVS instance.
    virtual ~XsciLexerAVS();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

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

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

public slots:
    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

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
    void setCommentProp();
    void setCompactProp();

    bool fold_comments;
    bool fold_compact;

    XsciLexerAVS(const XsciLexerAVS &);
    XsciLexerAVS &operator=(const XsciLexerAVS &);
};

#endif
