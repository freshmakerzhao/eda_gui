// This defines the interface to the ExsciLexerJSON class.



#ifndef EXSCILEXERJSON_H
#define EXSCILEXERJSON_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerJSON class encapsulates the Scintilla JSON lexer.
class EXSCINTILLA_EXPORT ExsciLexerJSON : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! JSON lexer.
    enum {
        //! The default.
        Default = 0,

        //! A number.
        Number = 1,

        //! A string.
        String = 2,

        //! An unclosed string.
        UnclosedString = 3,

        //! A property.
        Property = 4,

        //! An escape sequence.
        EscapeSequence = 5,

        //! A line comment.
        CommentLine = 6,

        //! A block comment.
        CommentBlock = 7,

        //! An operator.
        Operator = 8,

        //! An Internationalised Resource Identifier (IRI).
        IRI = 9,

        //! A JSON-LD compact IRI.
        IRICompact = 10,

        //! A JSON keyword.
        Keyword = 11,

        //! A JSON-LD keyword.
        KeywordLD = 12,

        //! A parsing error.
        Error = 13,
    };

    //! Construct a ExsciLexerJSON with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerJSON(QObject *parent = 0);

    //! Destroys the ExsciLexerJSON instance.
    virtual ~ExsciLexerJSON();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

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

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a highlight is true then line and block comments will be
    //! highlighted.  The default is true.
    //!
    //! \sa hightlightComments()
    void setHighlightComments(bool highlight);

    //! Returns true if line and block comments are highlighted
    //!
    //! \sa setHightlightComments()
    bool highlightComments() const {return allow_comments;}

    //! If \a highlight is true then escape sequences in strings are
    //! highlighted.  The default is true.
    //!
    //! \sa highlightEscapeSequences()
    void setHighlightEscapeSequences(bool highlight);

    //! Returns true if escape sequences in strings are highlighted.
    //!
    //! \sa setHighlightEscapeSequences()
    bool highlightEscapeSequences() const {return escape_sequence;}

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    void setFoldCompact(bool fold);

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

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
	void setAllowCommentsProp();
	void setEscapeSequenceProp();
	void setCompactProp();

	bool allow_comments;
	bool escape_sequence;
	bool fold_compact;

    ExsciLexerJSON(const ExsciLexerJSON &);
    ExsciLexerJSON &operator=(const ExsciLexerJSON &);
};

#endif
