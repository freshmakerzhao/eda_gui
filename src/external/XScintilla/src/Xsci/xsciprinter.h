// This module defines interface to the XsciPrinter class.
 


#ifndef XSCIPRINTER_H
#define XSCIPRINTER_H

// This is needed for Qt v5.0.0-alpha.
#if defined(B0)
#undef B0
#endif

#include <qprinter.h>

#if !defined(QT_NO_PRINTER)

#include <Xsci/xsciglobal.h>
#include <Xsci/xsciscintilla.h>


QT_BEGIN_NAMESPACE
class QRect;
class QPainter;
QT_END_NAMESPACE

class XsciScintillaBase;


//! \brief The XsciPrinter class is a sub-class of the Qt QPrinter class that
//! is able to print the text of a Scintilla document.
//!
//! The class can be further sub-classed to alter to layout of the text, adding
//! headers and footers for example.
class XSCINTILLA_EXPORT XsciPrinter : public QPrinter
{
public:
    //! Constructs a printer paint device with mode \a mode.
    XsciPrinter(PrinterMode mode = ScreenResolution);

    //! Destroys the XsciPrinter instance.
    virtual ~XsciPrinter();

    //! Format a page, by adding headers and footers for example, before the
    //! document text is drawn on it.  \a painter is the painter to be used to
    //! add customised text and graphics.  \a drawing is true if the page is
    //! actually being drawn rather than being sized.  \a painter drawing
    //! methods must only be called when \a drawing is true.  \a area is the
    //! area of the page that will be used to draw the text.  This should be
    //! modified if it is necessary to reserve space for any customised text or
    //! graphics.  By default the area is relative to the printable area of the
    //! page.  Use QPrinter::setFullPage() because calling printRange() if you
    //! want to try and print over the whole page.  \a pagenr is the number of
    //! the page.  The first page is numbered 1.
    virtual void formatPage(QPainter &painter, bool drawing, QRect &area,
            int pagenr);

    //! Return the number of points to add to each font when printing.
    //!
    //! \sa setMagnification()
    int magnification() const {return mag;}

    //! Sets the number of points to add to each font when printing to \a
    //! magnification.
    //!
    //! \sa magnification()
    virtual void setMagnification(int magnification);

    //! Print a range of lines from the Scintilla instance \a qsb.  \a from is
    //! the first line to print and a negative value signifies the first line
    //! of text.  \a to is the last line to print and a negative value
    //! signifies the last line of text.  true is returned if there was no
    //! error.
    virtual int printRange(XsciScintillaBase *qsb, int from = -1, int to = -1);

    //! Return the line wrap mode used when printing.  The default is
    //! XsciScintilla::WrapWord.
    //!
    //! \sa setWrapMode()
    XsciScintilla::WrapMode wrapMode() const {return wrap;}

    //! Sets the line wrap mode used when printing to \a wmode.
    //!
    //! \sa wrapMode()
    virtual void setWrapMode(XsciScintilla::WrapMode wmode);

private:
    int mag;
    XsciScintilla::WrapMode wrap;

    XsciPrinter(const XsciPrinter &);
    XsciPrinter &operator=(const XsciPrinter &);
};

#endif

#endif
