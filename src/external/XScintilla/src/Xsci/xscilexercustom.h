// This defines the interface to the XsciLexerCustom class.
 


#ifndef XSCILEXERCUSTOM_H
#define XSCILEXERCUSTOM_H

#include <Xsci/xsciglobal.h>
#include <Xsci/xscilexer.h>


class XsciScintilla;
class XsciStyle;


//! \brief The XsciLexerCustom class is an abstract class used as a base for
//! new language lexers.
//!
//! The advantage of implementing a new lexer this way (as opposed to adding
//! the lexer to the underlying Scintilla code) is that it does not require the
//! XScintilla library to be re-compiled.  It also makes it possible to
//! integrate external lexers.
//!
//! All that is necessary to implement a new lexer is to define appropriate
//! styles and to re-implement the styleText() method.
class XSCINTILLA_EXPORT XsciLexerCustom : public XsciLexer
{
    Q_OBJECT

public:
    //! Construct a XsciLexerCustom with parent \a parent.  \a parent is
    //! typically the XsciScintilla instance.
    XsciLexerCustom(QObject *parent = 0);

    //! Destroy the XSciLexerCustom.
    virtual ~XsciLexerCustom();

    //! The next \a length characters starting from the current styling
    //! position have their style set to style number \a style.  The current
    //! styling position is moved.  The styling position is initially set by
    //! calling startStyling().
    //!
    //! \sa startStyling(), styleText()
    void setStyling(int length, int style);

    //! The next \a length characters starting from the current styling
    //! position have their style set to style \a style.  The current styling
    //! position is moved.  The styling position is initially set by calling
    //! startStyling().
    //!
    //! \sa startStyling(), styleText()
    void setStyling(int length, const XsciStyle &style);

    //! The styling position is set to \a start.  \a styleBits is unused.
    //!
    //! \sa setStyling(), styleBitsNeeded(), styleText()
    void startStyling(int pos, int styleBits = 0);

    //! This is called when the section of text beginning at position \a start
    //! and up to position \a end needs to be styled.  \a start will always be
    //! at the start of a line.  The text is styled by calling startStyling()
    //! followed by one or more calls to setStyling().  It must be
    //! re-implemented by a sub-class.
    //!
    //! \sa setStyling(), startStyling(), XsciScintilla::bytes(),
    //! XsciScintilla::text()
    virtual void styleText(int start, int end) = 0;

    //! \reimp
    virtual void setEditor(XsciScintilla *editor);

    //! \reimp This re-implementation returns 5 as the number of style bits
    //! needed.
    virtual int styleBitsNeeded() const;

private slots:
    void handleStyleNeeded(int pos);

private:
    XsciLexerCustom(const XsciLexerCustom &);
    XsciLexerCustom &operator=(const XsciLexerCustom &);
};

#endif
