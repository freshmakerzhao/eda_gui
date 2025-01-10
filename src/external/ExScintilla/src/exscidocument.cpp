// This module implements the ExsciDocument class.



#include "Exsci/exscidocument.h"
#include "Exsci/exsciscintillabase.h"


// This internal class encapsulates the underlying document and is shared by
// ExsciDocument instances.
class ExsciDocumentP
{
public:
    ExsciDocumentP() : doc(0), nr_displays(0), nr_attaches(1), modified(false) {}

    void *doc;              // The Scintilla document.
    int nr_displays;        // The number of displays.
    int nr_attaches;        // The number of attaches.
    bool modified;          // Set if not at a save point.
};


// The ctor.
ExsciDocument::ExsciDocument()
{
    pdoc = new ExsciDocumentP();
}


// The dtor.
ExsciDocument::~ExsciDocument()
{
    detach();
}


// The copy ctor.
ExsciDocument::ExsciDocument(const ExsciDocument &that)
{
    attach(that);
}


// The assignment operator.
ExsciDocument &ExsciDocument::operator=(const ExsciDocument &that)
{
    if (pdoc != that.pdoc)
    {
        detach();
        attach(that);
    }

    return *this;
}


// Attach an existing document to this one.
void ExsciDocument::attach(const ExsciDocument &that)
{
    ++that.pdoc->nr_attaches;
    pdoc = that.pdoc;
}


// Detach the underlying document.
void ExsciDocument::detach()
{
    if (!pdoc)
        return;

    if (--pdoc->nr_attaches == 0)
    {
        if (pdoc->doc && pdoc->nr_displays == 0)
        {
            ExsciScintillaBase *qsb = ExsciScintillaBase::pool();

            // Release the explicit reference to the document.  If the pool is
            // empty then we just accept the memory leak.
            if (qsb)
                qsb->SendScintilla(ExsciScintillaBase::SCI_RELEASEDOCUMENT, 0,
                        pdoc->doc);
        }

        delete pdoc;
    }

    pdoc = 0;
}


// Undisplay and detach the underlying document.
void ExsciDocument::undisplay(ExsciScintillaBase *qsb)
{
    if (--pdoc->nr_attaches == 0)
        delete pdoc;
    else if (--pdoc->nr_displays == 0)
    {
        // Create an explicit reference to the document to keep it alive.
        qsb->SendScintilla(ExsciScintillaBase::SCI_ADDREFDOCUMENT, 0, pdoc->doc);
    }

    pdoc = 0;
}


// Display the underlying document.
void ExsciDocument::display(ExsciScintillaBase *qsb, const ExsciDocument *from)
{
    void *ndoc = (from ? from->pdoc->doc : 0);

    // SCI_SETDOCPOINTER appears to reset the EOL mode so save and restore it.
    int eol_mode = qsb->SendScintilla(ExsciScintillaBase::SCI_GETEOLMODE);

    qsb->SendScintilla(ExsciScintillaBase::SCI_SETDOCPOINTER, 0, ndoc);
    ndoc = qsb->SendScintillaPtrResult(ExsciScintillaBase::SCI_GETDOCPOINTER);

    qsb->SendScintilla(ExsciScintillaBase::SCI_SETEOLMODE, eol_mode);

    pdoc->doc = ndoc;
    ++pdoc->nr_displays;
}


// Return the modified state of the document.
bool ExsciDocument::isModified() const
{
    return pdoc->modified;
}


// Set the modified state of the document.
void ExsciDocument::setModified(bool m)
{
    pdoc->modified = m;
}
