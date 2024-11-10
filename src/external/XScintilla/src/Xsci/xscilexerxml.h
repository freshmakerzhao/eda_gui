// This defines the interface to the XsciLexerXML class.
 


#ifndef XSCILEXERXML_H
#define XSCILEXERXML_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexerhtml.h>


//! \brief The XsciLexerXML class encapsulates the Scintilla XML lexer.
class XSCINTILLA_EXPORT XsciLexerXML : public XsciLexerHTML
{
    Q_OBJECT

public:
    //! Construct a XsciLexerXML with parent \a parent.  \a parent is typically
    //! the XsciScintilla instance.
    XsciLexerXML(QObject *parent = 0);

    //! Destroys the XsciLexerXML instance.
    virtual ~XsciLexerXML();

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

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a allowed is true then scripts are styled.  The default is true.
    //!
    //! \sa scriptsStyled()
    void setScriptsStyled(bool styled);

    //! Returns true if scripts are styled.
    //!
    //! \sa setScriptsStyled()
    bool scriptsStyled() const;

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
    void setScriptsProp();

    bool scripts;

    XsciLexerXML(const XsciLexerXML &);
    XsciLexerXML &operator=(const XsciLexerXML &);
};

#endif
