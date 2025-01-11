// This defines the interface to the ExsciLexerYAML class.



#ifndef EXSCILEXERYAML_H
#define EXSCILEXERYAML_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerYAML class encapsulates the Scintilla YAML lexer.
class EXSCINTILLA_EXPORT ExsciLexerYAML : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! YAML lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! An identifier.
        Identifier = 2,

        //! A keyword
        Keyword = 3,

        //! A number.
        Number = 4,

        //! A reference.
        Reference = 5,

        //! A document delimiter.
        DocumentDelimiter = 6,

        //! A text block marker.
        TextBlockMarker = 7,

        //! A syntax error marker.
        SyntaxErrorMarker = 8,

        //! An operator.
        Operator = 9
    };

    //! Construct a ExsciLexerYAML with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerYAML(QObject *parent = 0);

    //! Destroys the ExsciLexerYAML instance.
    virtual ~ExsciLexerYAML();

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

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

public slots:
    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

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

    bool fold_comments;

    ExsciLexerYAML(const ExsciLexerYAML &);
    ExsciLexerYAML &operator=(const ExsciLexerYAML &);
};

#endif
