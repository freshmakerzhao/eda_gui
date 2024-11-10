// This defines the interface to the XsciLexerPO class.
 


#ifndef XSCILEXERPO_H
#define XSCILEXERPO_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerPO class encapsulates the Scintilla PO lexer.
class XSCINTILLA_EXPORT XsciLexerPO : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! PO lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A message identifier.
        MessageId = 2,

        //! The text of a message identifier.
        MessageIdText = 3,

        //! A message string.
        MessageString = 4,

        //! The text of a message string.
        MessageStringText = 5,

        //! A message context.
        MessageContext = 6,

        //! The text of a message context.
        MessageContextText = 7,

        //! The "fuzzy" flag.
        Fuzzy = 8,

        //! A programmer comment.
        ProgrammerComment = 9,

        //! A reference.
        Reference = 10,

        //! A flag.
        Flags = 11,

        //! A message identifier text end-of-line.
        MessageIdTextEOL = 12,

        //! A message string text end-of-line.
        MessageStringTextEOL = 13,

        //! A message context text end-of-line.
        MessageContextTextEOL = 14
    };

    //! Construct a XsciLexerPO with parent \a parent.  \a parent is typically
    //! the XsciScintilla instance.
    XsciLexerPO(QObject *parent = 0);

    //! Destroys the XsciLexerPO instance.
    virtual ~XsciLexerPO();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

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

    XsciLexerPO(const XsciLexerPO &);
    XsciLexerPO &operator=(const XsciLexerPO &);
};

#endif
