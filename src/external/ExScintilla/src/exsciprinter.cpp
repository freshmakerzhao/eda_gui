// This module implements the ExsciPrinter class.



#include "Exsci/exsciprinter.h"

#if !defined(QT_NO_PRINTER)

#include <QPrinter>
#include <QPainter>
#include <QStack>

#include "Exsci/exsciscintillabase.h"


// The ctor.
ExsciPrinter::ExsciPrinter(QPrinter::PrinterMode mode)
    : QPrinter(mode), mag(0), wrap(ExsciScintilla::WrapWord)
{
}


// The dtor.
ExsciPrinter::~ExsciPrinter()
{
}


// Format the page before the document text is drawn.
void ExsciPrinter::formatPage(QPainter &, bool, QRect &, int)
{
}


// Print a range of lines to a printer.
int ExsciPrinter::printRange(ExsciScintillaBase *qsb, int from, int to)
{
    // Sanity check.
    if (!qsb)
        return false;

    // Setup the printing area.
    QRect def_area;

    def_area.setX(0);
    def_area.setY(0);
    def_area.setWidth(width());
    def_area.setHeight(height());

    // Get the page range.
    int pgFrom, pgTo;

    pgFrom = fromPage();
    pgTo = toPage();

    // Find the position range.
    long startPos, endPos;

    endPos = qsb->SendScintilla(ExsciScintillaBase::SCI_GETLENGTH);

    startPos = (from > 0 ? qsb -> SendScintilla(ExsciScintillaBase::SCI_POSITIONFROMLINE,from) : 0);

    if (to >= 0)
    {
        long toPos = qsb -> SendScintilla(ExsciScintillaBase::SCI_POSITIONFROMLINE,to + 1);

        if (endPos > toPos)
            endPos = toPos;
    }

    if (startPos >= endPos)
        return false;

    QPainter painter(this);
    bool reverse = (pageOrder() == LastPageFirst);
    bool needNewPage = false;

    qsb -> SendScintilla(ExsciScintillaBase::SCI_SETPRINTMAGNIFICATION,mag);
    qsb -> SendScintilla(ExsciScintillaBase::SCI_SETPRINTWRAPMODE,wrap);

    for (int i = 1; i <= numCopies(); ++i)
    {
        // If we are printing in reverse page order then remember the start
        // position of each page.
        QStack<long> pageStarts;

        int currPage = 1;
        long pos = startPos;

        while (pos < endPos)
        {
            // See if we have finished the requested page range.
            if (pgTo > 0 && pgTo < currPage)
                break;

            // See if we are going to render this page, or just see how much
            // would fit onto it.
            bool render = false;

            if (pgFrom == 0 || pgFrom <= currPage)
            {
                if (reverse)
                    pageStarts.push(pos);
                else
                {
                    render = true;

                    if (needNewPage)
                    {
                        if (!newPage())
                            return false;
                    }
                    else
                        needNewPage = true;
                }
            }

            QRect area = def_area;

            formatPage(painter,render,area,currPage);
            pos = qsb -> SendScintilla(ExsciScintillaBase::SCI_FORMATRANGE,render,&painter,area,pos,endPos);

            ++currPage;
        }

        // All done if we are printing in normal page order.
        if (!reverse)
            continue;

        // Now go through each page on the stack and really print it.
        while (!pageStarts.isEmpty())
        {
            --currPage;

            long ePos = pos;
            pos = pageStarts.pop();

            if (needNewPage)
            {
                if (!newPage())
                    return false;
            }
            else
                needNewPage = true;

            QRect area = def_area;

            formatPage(painter,true,area,currPage);
            qsb->SendScintilla(ExsciScintillaBase::SCI_FORMATRANGE,true,&painter,area,pos,ePos);
        }
    }

    return true;
}


// Set the print magnification in points.
void ExsciPrinter::setMagnification(int magnification)
{
    mag = magnification;
}


// Set the line wrap mode.
void ExsciPrinter::setWrapMode(ExsciScintilla::WrapMode wmode)
{
    wrap = wmode;
}

#endif
