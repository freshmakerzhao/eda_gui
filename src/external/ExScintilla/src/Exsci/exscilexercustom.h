// This defines the interface to the ExsciLexerCustom class.



#ifndef EXSCILEXERCUSTOM_H
#define EXSCILEXERCUSTOM_H

#include <Exsci/exsciglobal.h>
#include <Exsci/exscilexer.h>


class ExsciScintilla;
class ExsciStyle;


//! \brief The ExsciLexerCustom class is an abstract class used as a base for
//! new language lexers.
//!
//! The advantage of implementing a new lexer this way (as opposed to adding
//! the lexer to the underlying Scintilla code) is that it does not require the
//! EXScintilla library to be re-compiled.  It also makes it possible to
//! integrate external lexers.
//!
//! All that is necessary to implement a new lexer is to define appropriate
//! styles and to re-implement the styleText() method.
class EXSCINTILLA_EXPORT ExsciLexerCustom : public ExsciLexer
{
    Q_OBJECT

public:
    //! Construct a ExsciLexerCustom with parent \a parent.  \a parent is
    //! typically the ExsciScintilla instance.
    ExsciLexerCustom(QObject *parent = 0);

    //! Destroy the EXSciLexerCustom.
    virtual ~ExsciLexerCustom();

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
    void setStyling(int length, const ExsciStyle &style);

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
    //! \sa setStyling(), startStyling(), ExsciScintilla::bytes(),
    //! ExsciScintilla::text()
    virtual void styleText(int start, int end) = 0;

    //! \reimp
    virtual void setEditor(ExsciScintilla *editor);

    //! \reimp This re-implementation returns 5 as the number of style bits
    //! needed.
    virtual int styleBitsNeeded() const;

private slots:
    void handleStyleNeeded(int pos);

private:
    ExsciLexerCustom(const ExsciLexerCustom &);
    ExsciLexerCustom &operator=(const ExsciLexerCustom &);
};

#endif
