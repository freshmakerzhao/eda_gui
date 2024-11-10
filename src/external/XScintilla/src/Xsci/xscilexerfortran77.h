// This defines the interface to the XsciLexerFortran77 class.
 


#ifndef XSCILEXERFORTRAN77_H
#define XSCILEXERFORTRAN77_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerFortran77 class encapsulates the Scintilla Fortran77
//! lexer.
class XSCINTILLA_EXPORT XsciLexerFortran77 : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Fortran77 lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A number.
        Number = 2,

        //! A single-quoted string.
        SingleQuotedString = 3,

        //! A double-quoted string.
        DoubleQuotedString = 4,

        //! The end of a line where a string is not closed.
        UnclosedString = 5,

        //! An operator.
        Operator = 6,

        //! An identifier
        Identifier = 7,

        //! A keyword.
        Keyword = 8,

        //! An intrinsic function.
        IntrinsicFunction = 9,

        //! An extended, non-standard or user defined function.
        ExtendedFunction = 10,

        //! A pre-processor block.
        PreProcessor = 11,

        //! An operator in .NAME. format.
        DottedOperator = 12,

        //! A label.
        Label = 13,

        //! A continuation.
        Continuation = 14
    };

    //! Construct a XsciLexerFortran77 with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerFortran77(QObject *parent = 0);

    //! Destroys the XsciLexerFortran77 instance.
    virtual ~XsciLexerFortran77();

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

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

public slots:
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
    //! \sa writeProperties()
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    //! \sa readProperties()
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setCompactProp();

    bool fold_compact;

    XsciLexerFortran77(const XsciLexerFortran77 &);
    XsciLexerFortran77 &operator=(const XsciLexerFortran77 &);
};

#endif
