// This defines the interface to the XsciLexerEDIFACT class.
 


#ifndef XSCILEXEREDIFACT_H
#define XSCILEXEREDIFACT_H

#include <QObject>

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


//! \brief The XsciLexerEDIFACT class encapsulates the Scintilla EDIFACT lexer.
class XSCINTILLA_EXPORT XsciLexerEDIFACT : public XsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! EDIFACT lexer.
    enum {
        //! The default.
        Default = 0,

        //! A segment start.
        SegmentStart = 1,

        //! A segment end.
        SegmentEnd = 2,

        //! An element separator.
        ElementSeparator = 3,

        //! A composite separator.
        CompositeSeparator = 4,

        //! A release separator.
        ReleaseSeparator = 5,

        //! A UNA segment header.
        UNASegmentHeader = 6,

        //! A UNH segment header.
        UNHSegmentHeader = 7,

        //! A bad segment.
        BadSegment = 8,
    };

    //! Construct a XsciLexerEDIFACT with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerEDIFACT(QObject *parent = 0);

    //! Destroys the XsciLexerEDIFACT instance.
    virtual ~XsciLexerEDIFACT();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    XsciLexerEDIFACT(const XsciLexerEDIFACT &);
    XsciLexerEDIFACT &operator=(const XsciLexerEDIFACT &);
};

#endif
