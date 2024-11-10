// This defines the interface to the XsciLexerProperties class.
 


#ifndef XSCILEXERPROPERTIES_H
#define XSCILEXERPROPERTIES_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerProperties class encapsulates the Scintilla
//! Properties lexer.
class XSCINTILLA_EXPORT XsciLexerProperties : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Properties lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A section.
        Section = 2,

        //! An assignment operator.
        Assignment = 3,

        //! A default value.
        DefaultValue = 4,

        //! A key.
        Key = 5
    };

    //! Construct a XsciLexerProperties with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerProperties(QObject *parent = 0);

    //! Destroys the XsciLexerProperties instance.
    virtual ~XsciLexerProperties();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

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

    //! Returns the descriptive name for style number \a style.  If the style
    //! is invalid for this language then an empty QString is returned.  This
    //! is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! If \a enable is true then initial spaces in a line are allowed.  The
    //! default is true.
    //!
    //! \sa initialSpaces()
    void setInitialSpaces(bool enable);

    //! Returns true if initial spaces in a line are allowed.
    //!
    //! \sa setInitialSpaces()
    bool initialSpaces() const {return initial_spaces;}

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
    void setInitialSpacesProp();

    bool fold_compact;
    bool initial_spaces;

    XsciLexerProperties(const XsciLexerProperties &);
    XsciLexerProperties &operator=(const XsciLexerProperties &);
};

#endif
