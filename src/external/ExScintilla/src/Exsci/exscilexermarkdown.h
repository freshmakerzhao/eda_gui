// This defines the interface to the ExsciLexerMarkdown class.



#ifndef EXSCILEXERMARKDOWN_H
#define EXSCILEXERMARKDOWN_H

#include <QObject>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


//! \brief The ExsciLexerMarkdown class encapsulates the Scintilla Markdown
//! lexer.
class EXSCINTILLA_EXPORT ExsciLexerMarkdown : public ExsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Markdown lexer.

    // Note that some values are omitted (ie. LINE_BEGIN and PRECHAR) as these
    // seem to be internal state information rather than indicating that text
    // should be styled differently.
    enum {
        //! The default.
        Default = 0,

        //! Special (e.g. end-of-line codes if enabled).
        Special = 1,

        //! Strong emphasis using double asterisks.
        StrongEmphasisAsterisks = 2,

        //! Strong emphasis using double underscores.
        StrongEmphasisUnderscores = 3,

        //! Emphasis using single asterisks.
        EmphasisAsterisks = 4,

        //! Emphasis using single underscores.
        EmphasisUnderscores = 5,

        //! A level 1 header.
        Header1 = 6,

        //! A level 2 header.
        Header2 = 7,

        //! A level 3 header.
        Header3 = 8,

        //! A level 4 header.
        Header4 = 9,

        //! A level 5 header.
        Header5 = 10,

        //! A level 6 header.
        Header6 = 11,

        //! Pre-char (up to three indent spaces, e.g. for a sub-list).
        Prechar = 12,

        //! An unordered list item.
        UnorderedListItem = 13,

        //! An ordered list item.
        OrderedListItem = 14,

        //! A block quote.
        BlockQuote = 15,

        //! Strike out.
        StrikeOut = 16,

        //! A horizontal rule.
        HorizontalRule = 17,

        //! A link.
        Link = 18,

        //! Code between backticks.
        CodeBackticks = 19,

        //! Code between double backticks.
        CodeDoubleBackticks = 20,

        //! A code block.
        CodeBlock = 21,
    };

    //! Construct a ExsciLexerMarkdown with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerMarkdown(QObject *parent = 0);

    //! Destroys the ExsciLexerMarkdown instance.
    virtual ~ExsciLexerMarkdown();

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

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    ExsciLexerMarkdown(const ExsciLexerMarkdown &);
    ExsciLexerMarkdown &operator=(const ExsciLexerMarkdown &);
};

#endif
