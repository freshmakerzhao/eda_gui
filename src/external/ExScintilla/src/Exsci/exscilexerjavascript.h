// This defines the interface to the ExsciLexerJavaScript class.



#ifndef EXSCILEXERJSCRIPT_H
#define EXSCILEXERJSCRIPT_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexercpp.h>


//! \brief The ExsciLexerJavaScript class encapsulates the Scintilla JavaScript
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerJavaScript : public ExsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerJavaScript with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerJavaScript(QObject *parent = 0);

    //! Destroys the ExsciLexerJavaScript instance.
    virtual ~ExsciLexerJavaScript();

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
    friend class ExsciLexerHTML;

    static const char *keywordClass;

    ExsciLexerJavaScript(const ExsciLexerJavaScript &);
    ExsciLexerJavaScript &operator=(const ExsciLexerJavaScript &);
};

#endif
