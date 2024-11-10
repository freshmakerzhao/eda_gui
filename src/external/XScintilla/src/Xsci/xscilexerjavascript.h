// This defines the interface to the XsciLexerJavaScript class.
 


#ifndef XSCILEXERJSCRIPT_H
#define XSCILEXERJSCRIPT_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexercpp.h>


//! \brief The XsciLexerJavaScript class encapsulates the Scintilla JavaScript
//! lexer.
class XSCINTILLA_EXPORT XsciLexerJavaScript : public XsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a XsciLexerJavaScript with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerJavaScript(QObject *parent = 0);

    //! Destroys the XsciLexerJavaScript instance.
    virtual ~XsciLexerJavaScript();

    //! Returns the name of the language.
    const char *language() const;

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
    friend class XsciLexerHTML;

    static const char *keywordClass;

    XsciLexerJavaScript(const XsciLexerJavaScript &);
    XsciLexerJavaScript &operator=(const XsciLexerJavaScript &);
};

#endif
